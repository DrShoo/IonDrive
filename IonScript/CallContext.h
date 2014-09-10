#pragma once

namespace ionscript
{
    enum class RuntimeErrorCode;

    struct ICallContext
    {
        virtual void CallRuntimeError(const RuntimeErrorCode errorCode, const std::wstring &what) = 0;

        virtual bool CallSetSymbolValue(const std::wstring &id, ExpressionSptr value) = 0;

        virtual bool CallSymbolExists(const std::wstring &id) = 0;

        #pragma region Helpers

        void CallRuntimeError(const RuntimeErrorCode errorCode, const boost::wformat &what)
        {
            CallRuntimeError(errorCode, what.str());
        }

        template<class T, class ...Args>
        void CallRuntimeError(const RuntimeErrorCode errorCode, boost::wformat &fmt, const T &arg, Args... rest)
        {
            fmt % arg;
            CallRuntimeError(errorCode, fmt, rest...);
        }
        
        template<class T, class ...Args>
        void CallRuntimeError(const RuntimeErrorCode errorCode, const std::wstring &fmt, const T &arg, Args... rest)
        {
            CallRuntimeError(errorCode, boost::wformat(fmt) % arg, rest...);
        }

        #pragma endregion

    };

}