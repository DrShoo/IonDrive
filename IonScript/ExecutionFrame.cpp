#include "stdafx.h"

#include "Expression.h"
#include "ExecutionFrame.h"

namespace ionscript
{

    ExpressionSptr ExecutionFrame::GetSymbol(const std::wstring &id)
    {
        const auto &i = _symbols.find(id);
        if (i != _symbols.end())
        {
            assert(i->second);
            return i->second;
        }

        return ExpressionSptr();
    }

    void ExecutionFrame::SetSymbol(const std::wstring &id, ExpressionSptr value)
    {
        assert(!id.empty());
        assert(value);

        _symbols[id] = value;
    }

}

