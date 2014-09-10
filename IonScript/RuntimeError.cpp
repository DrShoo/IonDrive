#include "stdafx.h"

#include "Defs.h"
#include "Expression.h"
#include "RuntimeError.h"

namespace ionscript
{

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

}