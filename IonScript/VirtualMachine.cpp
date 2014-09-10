#include "stdafx.h"

#include "StdLib/Init.h"

#include "Defs.h"
#include "Expression.h"
#include "Lexer.h"
#include "Parser.h"
#include "RuntimeError.h"
#include "ExecutionFrame.h"
#include "VirtualMachine.h"

namespace ionscript
{
    namespace logger = common::log;

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
        assert(!id.empty());
        
        if (!EvaluateSymbol(id))
        {
            return false;
        }

        if (_stack.top()->UpdateSymbol(id, value))
        {
            return true;
        }

        const auto hasGlobalFrameOnly = (_stack.size() == 1);
        if (hasGlobalFrameOnly)
        {
            return false;
        }

        return _globalFrame->UpdateSymbol(id, value);
    }

    bool VirtualMachine::CallSymbolExists(const std::wstring &id)
    {
        assert(!id.empty());
        return (bool)EvaluateSymbol(id);
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
        if (!_globalFrame->AddSymbol(id, expr, false))
        {
            return false;
        }

        return true;
    }

    bool VirtualMachine::LibraryBindNativeVariable(const std::wstring &id, ExpressionSptr value, VariableChangedCallback callback)
    {
        assert(!id.empty());
        assert(value);
        assert(_globalFrame);

        logger::Trace(L"binding native variable to symbol '%s', initial value '%s'", id, value->ToString());

        return _globalFrame->AddSymbol(id, value, true);
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