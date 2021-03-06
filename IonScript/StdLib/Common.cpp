#include "stdafx.h"

#include "../Defs.h"
#include "../Expression.h"
#include "../CallContext.h"
#include "../RuntimeError.h"

#include "Common.h"

namespace ionscript
{
    

    namespace stdlib
    {

        CallableSignatureSptr SetQ_Signature()
        {
            return CallableSignatureSptr();
        }

        ExpressionSptr SetQ(ICallContext &c, ExpressionSptr params)
        {
            if (params->ListGetSize() < 2)
            {
                c.CallRuntimeError(RuntimeErrorCode::InvalidArguments, L"too few parameters for 'setq', should be at least two");
                return ExpressionSptr();
            }

            const auto toSet = params->ListHead();
            if (toSet->GetType() != ExpressionType::Symbol)
            {
                c.CallRuntimeError(RuntimeErrorCode::InvalidArguments, L"first argument for 'setq' should be a symbol, but was '%s'", toSet->ToString());
                return ExpressionSptr();
            }

            const auto &id = toSet->SymbolGetId();
            if (!c.CallSymbolExists(id))
            {
                c.CallRuntimeError(RuntimeErrorCode::InvalidArguments, L"symbol '%s' was not defined", id);
                return ExpressionSptr();
            }

            auto value = params->ListTail();
            if (value->ListGetSize() == 1)
            {
                value = value->ListHead();
            }

            if (!c.CallSetSymbolValue(id, value))
            {
                c.CallRuntimeError(RuntimeErrorCode::InvalidArguments, L"symbol '%s' can't be set for unknown reason", id);
                return ExpressionSptr();
            }

            return toSet;
        }

    }
}