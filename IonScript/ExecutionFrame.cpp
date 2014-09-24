#include "stdafx.h"

#include "Expression.h"
#include "ExecutionFrame.h"

namespace logger = common::logger;

namespace ionscript
{
    struct FrameSymbol : boost::noncopyable
    {
        FrameSymbol(const bool isNative, const ExpressionSptr value)
            : IsNative(isNative), Value(value)
        {
        }

        const bool IsNative;
        ExpressionSptr Value;
    };

    ExpressionSptr ExecutionFrame::GetSymbol(const std::wstring &id)
    {
        const auto &i = _symbols.find(id);
        if (i != _symbols.end())
        {
            assert(i->second);
            return i->second->Value;
        }

        return ExpressionSptr();
    }

    bool ExecutionFrame::AddSymbol(const std::wstring &id, ExpressionSptr value, const bool isNative)
    {
        assert(!id.empty());
        assert(value);

        if (_symbols.count(id) > 0)
        {
            logger::Error(L"failed to add symbol '%s', symbol already exists", id, value->ToString());
            return false;
        }

        _symbols[id] = std::make_shared<FrameSymbol>(isNative, value);

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

        auto &fs = i->second;

        if (fs->IsNative)
        {
            fs->Value->Assign(*value);
        }
        else
        {
            fs->Value = value;
        }

        return true;
    }

}

