#pragma once

namespace common
{

    namespace logger
    {

        void Debug(const std::wstring &function, const std::wstring &file, const int line, const std::wstring &msg);

        void Initialize();

        void Uninitialize();

        template<class ...Args>
        void Debug(const std::wstring &function, const std::wstring &file, const int line, const std::wstring &fmt, Args... rest)
        {

        }

        #define COMMON_LOG_HELPER_HEAD(NAME)                                                             \
            void NAME(const boost::wformat &msg);                                                        \
                                                                                                         \
            void NAME(const std::wstring &msg);                                                          \
                                                                                                         \
            template<class T, class ...Args>                                                             \
            void NAME(boost::wformat &fmt, const T &arg, Args... rest)                                   \
            {                                                                                            \
                fmt % arg;                                                                               \
                NAME(fmt, rest...);                                                                      \
            }                                                                                            \
                                                                                                         \
            template<class T, class ...Args>                                                             \
            void NAME(const std::wstring &fmt, const T &arg, Args... rest)                               \
            {                                                                                            \
                NAME(boost::wformat(fmt) % arg, rest...);                                                \
            }

        COMMON_LOG_HELPER_HEAD(Error);
        COMMON_LOG_HELPER_HEAD(Fatal);
        COMMON_LOG_HELPER_HEAD(Info);
        COMMON_LOG_HELPER_HEAD(Trace);
        COMMON_LOG_HELPER_HEAD(Write);

    }

}