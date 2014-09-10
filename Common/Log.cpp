#include "stdafx.h"

#include "Log.h"

using namespace boost::chrono;
using namespace boost::gregorian;
using namespace boost::iostreams;
namespace pt = boost::posix_time;

namespace
{
    struct LogMessage : boost::noncopyable
    {
        LogMessage(const std::wstring &message)
            : Message(message)
        {
            assert(!message.empty());
        }

        const std::wstring Message;
    };

    typedef std::shared_ptr<LogMessage> LogMessageSptr;

    std::unique_ptr<boost::thread> _thread;

    boost::recursive_mutex _messageMutex;

    std::queue<LogMessageSptr> _messages;

    boost::condition_variable_any _wakeLogger;

    stream_buffer<wfile_sink> _logFile;

    bool _stop = false;

    void WriteThread();

    void WriteTsLogMessage(const std::wstring &type, const std::wstring &msg);
}

namespace common
{

    namespace log
    {
        #define COMMON_LOG_HELPER_BODY(NAME, TAG)   \
            void NAME(const boost::wformat &msg)    \
            {                                       \
                NAME(msg.str());                    \
            }                                       \
                                                    \
            void NAME(const std::wstring &msg)      \
            {                                       \
                WriteTsLogMessage(TAG, msg);        \
            }

        COMMON_LOG_HELPER_BODY(Error, L"ERROR");
        COMMON_LOG_HELPER_BODY(Fatal, L"FATAL");
        COMMON_LOG_HELPER_BODY(Info, L"INFO");
        COMMON_LOG_HELPER_BODY(Trace, L"TRACE");

        void Initialize()
        {
            boost::recursive_mutex::scoped_lock lock(_messageMutex);

            assert(!_thread);
            _thread.reset(new boost::thread(WriteThread));
        }

        void Uninitialize()
        {
            {
                boost::recursive_mutex::scoped_lock lock(_messageMutex);
                _stop = true;
            }

            _wakeLogger.notify_one();

            const auto timeout = seconds(5);
            if (!_thread->try_join_for(timeout))
            {
                assert(false);
                _thread->interrupt();
            }

            _thread.reset();
        }

        void Write(const std::wstring &msg)
        {
            {
                boost::recursive_mutex::scoped_lock lock(_messageMutex);

                assert(!_stop);

                _messages.push(std::make_shared<LogMessage>(
                    msg
                ));
            }

            _wakeLogger.notify_one();
        }

        void Write(const boost::wformat &msg)
        {
            Write(msg.str());
        }

    }

}

namespace
{
    bool WriteNextMessage()
    {
        LogMessageSptr message;

        {
            boost::recursive_mutex::scoped_lock lock(_messageMutex);

            if (_messages.empty())
            {
                return false;
            }

            message = _messages.front();
            _messages.pop();
        }

        if (!_logFile.is_open())
        {
            assert(_logFile.is_open());
            return true;
        }
        
        _logFile->write(message->Message.c_str(), message->Message.length());
        _logFile->flush();

        return true;
    }

    void WriteThread()
    {
        boost::recursive_mutex::scoped_lock lock(_messageMutex);

        ::SetThreadPriority(::GetCurrentThread(), THREAD_PRIORITY_IDLE);

        assert(!_logFile.is_open());
        _logFile.open("iondrive.log");

        for (;;)
        {
            while (!_stop && _messages.empty())
            {
                _wakeLogger.wait(lock);
            }

            while (WriteNextMessage()) {}

            if (_stop)
            {
                _logFile.close();
                return;
            }
        }
    }

    void WriteTsLogMessage(const std::wstring &type, const std::wstring &msg)
    {
        assert(!type.empty());

        boost::wformat fmt(L"%s - [%s]: %s\n");
        auto localtime = to_simple_wstring(pt::second_clock::local_time().time_of_day());
        common::log::Write(fmt, localtime, type, msg);
    }
}