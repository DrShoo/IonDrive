#pragma once

namespace ionscript
{
    enum class ExpressionType
    {
        Invalid,
        Min,

        Integer,
        Real,
        Boolean,
        String,
        NativeMacro,

        Symbol,
        List,
        Null,

        Max
    };

    typedef std::shared_ptr<class Expression> ExpressionSptr;
    typedef std::weak_ptr<class Expression> ExpressionWptr;
    typedef std::deque<ExpressionSptr> ExpressionSptrList;
    typedef std::function<ExpressionSptr(struct ICallContext&, ExpressionSptr)> NativeMacroImpl;

    class Expression : boost::noncopyable
    {
    public:
        struct SourceLocation : boost::noncopyable
        {
            SourceLocation(const std::wstring &filename, const int line, const int row);

            const std::wstring Filename;
            const int Line;
            const int Row;
        };

        typedef std::shared_ptr<SourceLocation> SourceLocationSptr;

        static ExpressionSptr Boolean(const bool value);

        static ExpressionSptr Integer(const __int64 value);

        static ExpressionSptr List();

        static ExpressionSptr NativeMacro(NativeMacroImpl impl);

        static ExpressionSptr Null();

        static ExpressionSptr Real(const double value);

        static ExpressionSptr String(const std::wstring &value);

        static ExpressionSptr Symbol(const std::wstring &id);
        
        void Assign(const Expression &rhs);

        SourceLocationSptr GetSourceLocation() const;

        void SetSourceLocation(SourceLocationSptr sourceLocation);

        ExpressionType GetType() const;

        int IntegerGet() const;

        bool IsCallable() const;

        bool IsTerminal() const;

        ExpressionSptr MakeCopy() const;

        ExpressionSptr ListAt(const int index) const;

        ExpressionSptrList::const_iterator ListBegin() const;

        ExpressionSptrList::const_iterator ListEnd() const;

        void ListAssign(const ExpressionSptrList::const_iterator &from, const ExpressionSptrList::const_iterator &to);

        int ListGetSize() const;

        ExpressionSptr ListHead() const;

        void ListPushBack(ExpressionSptr exp);

        ExpressionSptr ListTail() const;

        ExpressionSptr NativeMacroCall(struct ICallContext &c, const ExpressionSptr params);

        const std::wstring &SymbolGetId() const;

        const std::wstring ToString() const;

    private:
        Expression();

        ExpressionType _type = ExpressionType::Invalid;

        boost::optional<__int64> _integer;

        boost::optional<double> _real;

        boost::optional<bool> _boolean;

        boost::optional<std::wstring> _string;

        boost::optional<std::wstring> _symbol;

        boost::optional<ExpressionSptrList> _list;

        NativeMacroImpl _nativeMacro;

        SourceLocationSptr _sourceLocation;
    };

}