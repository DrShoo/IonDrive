#pragma once

namespace ionscript
{
    class ExecutionFrame : boost::noncopyable
    {
    public:
        ExpressionSptr GetSymbol(const std::wstring &id);

        bool AddSymbol(const std::wstring &id, ExpressionSptr value);

        bool UpdateSymbol(const std::wstring &id, ExpressionSptr value);

    private:
        std::map<std::wstring, ExpressionSptr> _symbols;

    };

    typedef std::shared_ptr<ExecutionFrame> ExecutionFrameSptr;
}