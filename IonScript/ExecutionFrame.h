#pragma once

namespace ionscript
{
    typedef std::shared_ptr<struct FrameSymbol> FrameSymbolSptr;

    class ExecutionFrame : boost::noncopyable
    {
    public:
        ExpressionSptr GetSymbol(const std::wstring &id);

        bool AddSymbol(const std::wstring &id, ExpressionSptr value, const bool isNative);

        bool UpdateSymbol(const std::wstring &id, ExpressionSptr value);

    private:
        std::map<std::wstring, FrameSymbolSptr> _symbols;

    };

    typedef std::shared_ptr<ExecutionFrame> ExecutionFrameSptr;
}