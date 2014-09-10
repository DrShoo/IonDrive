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

    bool ExecutionFrame::AddSymbol(const std::wstring &id, ExpressionSptr value)
    {
        assert(!id.empty());
        assert(value);

        if (_symbols.count(id) > 0)
        {
            return false;
        }

        _symbols[id] = value;

        return true;
    }

    bool ExecutionFrame::UpdateSymbol(const std::wstring &id, ExpressionSptr value)
    {
        assert(!id.empty());
        assert(value);

        auto i = _symbols.find(id);
        if (i == _symbols.end())
        {
            return false; 
        }

        i->second = value;

        return true;
    }

}

