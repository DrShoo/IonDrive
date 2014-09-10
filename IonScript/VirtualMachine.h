#pragma once

#include "CallContext.h"
#include "LibraryContext.h"

namespace ionscript
{
    typedef std::shared_ptr<class ExecutionFrame> ExecutionFrameSptr;
    typedef std::unique_ptr<class Parser> ParserUptr;
    typedef std::shared_ptr<struct ParserError> ParserErrorSptr;
    typedef std::list<ParserErrorSptr> ParserErrorList;
    typedef std::shared_ptr<class VirtualMachine> VirtualMachineSptr;

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

    class VirtualMachine : 
        public ICallContext,
        public ILibraryContext,
        public std::enable_shared_from_this<VirtualMachine>,
        boost::noncopyable
    {
    public:
        static VirtualMachineSptr Create();

        ~VirtualMachine();

        #pragma region ICallContext 

        virtual void CallRuntimeError(const RuntimeErrorCode errorCode, const std::wstring &what) override;

        virtual bool CallSetSymbolValue(const std::wstring &id, ExpressionSptr value) override;

        virtual bool CallSymbolExists(const std::wstring &id) override;

        #pragma endregion

        #pragma region ILibraryContext 

        virtual bool LibraryBindNativeMacro(const std::wstring &id, NativeMacroImpl impl, CallableSignatureSptr signature) override;

        #pragma endregion

        ExpressionSptr EvaluateFile(const std::wstring &filename, const std::wstring &text);

        ExpressionSptr EvaluateSymbol(const std::wstring &id);

        const ParserErrorList& GetParserErrors() const;

        const RuntimeErrorList& GetRuntimeErrors() const;

        bool Init();

    private:
        VirtualMachine();

        ExpressionSptr _currentRootExpression;

        ExpressionSptr _currentCaller;

        ExecutionFrameSptr _globalFrame;

        ParserUptr _parser;

        ParserErrorList _parserErrors;

        RuntimeErrorList _runtimeErrors;

        std::stack<ExecutionFrameSptr> _stack;

        ExpressionSptr EvaluateExpression(ExpressionSptr expr);

        ExpressionSptr EvaluateList(ExpressionSptr list);

        ExpressionSptr EvaluateList_Call(ExpressionSptr callable, ExpressionSptr params);

    };
}