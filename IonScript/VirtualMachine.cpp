#include "stdafx.h"

#include "StdLib/Init.h"

#include "Expression.h"
#include "Lexer.h"
#include "Parser.h"
#include "ExecutionFrame.h"
#include "VirtualMachine.h"

namespace ionscript
{
    namespace logger = common::log;

    RuntimeError::RuntimeError(
        const ExpressionSptr rootExpression, 
        const ExpressionSptr errorExpression, 
        const RuntimeErrorCode code, 
        const std::wstring &customErrorMessage)
        : RootExpression(rootExpression), ErrorExpression(errorExpression), Code(code), UserErrorMessage(customErrorMessage)
    {
        assert(rootExpression);
        assert(errorExpression);
        assert(code > RuntimeErrorCode::Min);
        assert(code < RuntimeErrorCode::Max);
    }
  
    const std::wstring RuntimeError::FormatedMessage() const
    {
        auto fmt = boost::wformat(L"runtime error RE%04d: %s\n%s%s") % (int)Code;

        switch (Code)
        {
            case RuntimeErrorCode::NotCallable:
                assert(ErrorExpression);
                assert(RootExpression);
                fmt % (boost::wformat(L"symbol '%s' does not bound to callable") % ErrorExpression->ToString());
                break;

            case RuntimeErrorCode::UnknownSymbol:
                assert(ErrorExpression);
                assert(RootExpression);
                fmt % (boost::wformat(L"unknown symbol '%s'") % ErrorExpression->ToString());
                break;

            case RuntimeErrorCode::InvalidArguments:
                assert(ErrorExpression);
                assert(RootExpression);
                {
                    const auto userMsg = (UserErrorMessage.empty() ? L"" : (L"\n" + UserErrorMessage));
                    fmt % (boost::wformat(L"invalid arguments for '%s'%s") % ErrorExpression->ToString() % userMsg);
                }
                break;

            case RuntimeErrorCode::GeneralError:
                assert(!UserErrorMessage.empty());
                fmt % UserErrorMessage;
                break;

            default:
                assert(!"unexpected RuntimeErrorCode value");
                break;
        }

        const auto location = RootExpression->GetSourceLocation();
        if (location)
        {
            fmt % (boost::wformat(L"in file '%s' at line %d row %d ") % location->Filename % location->Line % location->Row);
        }
        else
        {
            fmt % L"";
        }

        fmt % (boost::wformat(L"in expression\n%s") % RootExpression->ToString());

        return fmt.str();
    }

    VirtualMachineSptr VirtualMachine::Create()
    {
        return VirtualMachineSptr(new VirtualMachine);
    }

    VirtualMachine::VirtualMachine()
    {

    }

    VirtualMachine::~VirtualMachine()
    {

    }

    bool VirtualMachine::Init()
    {
        assert(_stack.empty());
        assert(!_globalFrame);
        if (!_stack.empty() || _globalFrame)
        {
            return false;
        }

        _globalFrame = std::make_shared<ExecutionFrame>();
        _stack.push(_globalFrame);

        stdlib::Init(*this);

        return true;
    }

    #pragma region ICallContext 

    void VirtualMachine::CallRuntimeError(const RuntimeErrorCode errorCode, const std::wstring &what)
    {
        assert(errorCode > RuntimeErrorCode::Min);
        assert(errorCode < RuntimeErrorCode::Max);

        _runtimeErrors.push_back(std::make_shared<RuntimeError>(
            _currentRootExpression, _currentCaller, errorCode, what
        ));
    }

    bool VirtualMachine::CallSetSymbolValue(const std::wstring &id, ExpressionSptr value)
    {
        id;
        value;
        return false;
    }

    bool VirtualMachine::CallSymbolExists(const std::wstring &id)
    {
        id;
        return false;
    }

    #pragma endregion

    #pragma region ILibraryContext 

    bool VirtualMachine::LibraryBindNativeMacro(const std::wstring &id, NativeMacroImpl impl, CallableSignatureSptr signature)
    {
        assert(!id.empty());
        assert(impl);
        assert(_globalFrame);

        logger::Trace(
            L"binding native macro to symbol '%s'%s", 
            id, 
            signature ? L" (with signature)" : L"");

        auto expr = Expression::NativeMacro(impl);
        _globalFrame->SetSymbol(id, expr);

        return true;
    }

    #pragma endregion

    ExpressionSptr VirtualMachine::EvaluateFile(const std::wstring &filename, const std::wstring &text)
    {
        assert(!filename.empty());

        _parserErrors.clear();
        _runtimeErrors.clear();

        _parser.reset(new Parser);
        _parser->Init(filename, text);

        ExpressionSptr expr;
        while (expr = _parser->Next())
        {
            _currentRootExpression = expr;

            expr = EvaluateExpression(expr);
            if (!expr)
            {
                assert(!_runtimeErrors.empty());
                break;
            }
        }

        _parserErrors = _parser->GetErrors();
        if (_parserErrors.empty() && _runtimeErrors.empty())
        {
            assert(expr);
            return expr;
        }

        assert(!expr);
        return ExpressionSptr();
    }

    ExpressionSptr VirtualMachine::EvaluateSymbol(const std::wstring &id)
    {
        assert(!id.empty());
        assert(_globalFrame);
        assert(!_stack.empty());

        auto symbol = _stack.top()->GetSymbol(id);

        const auto hasGlobalFrameOnly = (_stack.size() == 1);
        if (symbol || hasGlobalFrameOnly)
        {
            return symbol;
        }

        return _globalFrame->GetSymbol(id);
    }

    const ParserErrorList& VirtualMachine::GetParserErrors() const
    {
        return _parserErrors;
    }

    const RuntimeErrorList& VirtualMachine::GetRuntimeErrors() const
    {
        return _runtimeErrors;
    }

    ExpressionSptr VirtualMachine::EvaluateExpression(ExpressionSptr expr)
    {
        assert(expr);

        if (expr->IsTerminal())
        {
            return expr->MakeCopy();
        }

        if (expr->GetType() == ExpressionType::Symbol)
        {
            return EvaluateSymbol(expr->SymbolGetId());
        }

        if (expr->GetType() == ExpressionType::List)
        {
            return EvaluateList(expr);
        }

        assert(!"unexpected expression type");
        return Expression::Null();
    }

    ExpressionSptr VirtualMachine::EvaluateList(ExpressionSptr list)
    {
        assert(list);
        assert(list->GetType() == ExpressionType::List);

        if (list->ListGetSize() == 0) 
        {
            return Expression::List();
        }

        ExpressionSptr head = *list->ListBegin();
        ExpressionSptr tail = list->ListTail();

        ExpressionSptr headValue = EvaluateExpression(head);
        if (!headValue)
        {
            _runtimeErrors.push_back(
                std::make_shared<RuntimeError>(_currentRootExpression, head, RuntimeErrorCode::UnknownSymbol, L"")
            );
            return ExpressionSptr();
        }

        if (!headValue->IsCallable())
        {
            _runtimeErrors.push_back(
                std::make_shared<RuntimeError>(_currentRootExpression, head, RuntimeErrorCode::NotCallable, L"")
            );
            return ExpressionSptr();
        }

        _currentCaller = head;

        return EvaluateList_Call(headValue, tail);
    }

    ExpressionSptr VirtualMachine::EvaluateList_Call(ExpressionSptr callable, ExpressionSptr params)
    {
        assert(callable);
        assert(params);
        assert(callable->IsCallable());
        assert(params->GetType() == ExpressionType::List);

        switch (callable->GetType())
        {
            case ExpressionType::NativeMacro:
                return callable->NativeMacroCall(*this, params);

            default:
                assert(!"unexpected callable type");
                return ExpressionSptr();
                break;
        }
    }

}