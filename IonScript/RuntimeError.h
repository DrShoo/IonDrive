#pragma once

namespace ionscript
{

    enum class RuntimeErrorCode
    {
        Invalid,
        Min,

        NotCallable,
        UnknownSymbol,
        InvalidArguments,
        GeneralError,

        Max
    };

    struct RuntimeError : boost::noncopyable
    {
        RuntimeError(const ExpressionSptr rootExpression, const ExpressionSptr errorExpression, const RuntimeErrorCode code, const std::wstring &customErrorMessage);

        const std::wstring FormatedMessage() const;

        const RuntimeErrorCode Code;
        const ExpressionSptr RootExpression;
        const ExpressionSptr ErrorExpression;
        const std::wstring UserErrorMessage;
    };

    typedef std::shared_ptr<RuntimeError> RuntimeErrorSptr;
    typedef std::list<RuntimeErrorSptr> RuntimeErrorList;

}