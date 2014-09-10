#include "stdafx.h"

#include "Keywords.h"
#include "Expression.h"

namespace ionscript
{

    Expression::SourceLocation::SourceLocation(const std::wstring &filename, const int line, const int row)
        : Filename(filename), Line(line), Row(row)
    {
        assert(!filename.empty());
        assert(line > 0);
        assert(row > 0);
    }

    ExpressionSptr Expression::Boolean(const bool value)
    {
        ExpressionSptr e(new Expression);
        e->_type = ExpressionType::Boolean;
        e->_boolean = value;
        return e;
    }

    ExpressionSptr Expression::Integer(const __int64 value)
    {
        ExpressionSptr e(new Expression);
        e->_type = ExpressionType::Integer;
        e->_integer = value;
        return e;
    }

    ExpressionSptr Expression::NativeMacro(NativeMacroImpl impl)
    {
        assert(impl);

        ExpressionSptr e(new Expression);
        e->_type = ExpressionType::NativeMacro;
        e->_nativeMacro = impl;
        return e;
    }

    ExpressionSptr Expression::Null()
    {
        ExpressionSptr e(new Expression);
        e->_type = ExpressionType::Null;
        return e;
    }

    ExpressionSptr Expression::List()
    {
        ExpressionSptr e(new Expression);
        e->_type = ExpressionType::List;
        e->_list = ExpressionSptrList();
        return e;
    }

    ExpressionSptr Expression::Real(const double value)
    {
        ExpressionSptr e(new Expression);
        e->_type = ExpressionType::Real;
        e->_real = value;
        return e;
    }

    ExpressionSptr Expression::String(const std::wstring &value)
    {
        ExpressionSptr e(new Expression);
        e->_type = ExpressionType::String;
        e->_string = value;
        return e;
    }

    ExpressionSptr Expression::Symbol(const std::wstring &id)
    {
        ExpressionSptr e(new Expression);
        e->_type = ExpressionType::Symbol;
        e->_symbol = id;
        return e;
    }

    Expression::Expression()
    {

    }

    Expression::SourceLocationSptr Expression::GetSourceLocation() const
    {
        return _sourceLocation;
    }

    void Expression::SetSourceLocation(SourceLocationSptr sourceLocation)
    {
        assert(!_sourceLocation);
        _sourceLocation = sourceLocation;
    }

    const std::wstring &Expression::SymbolGetId() const
    {
        assert(_type == ExpressionType::Symbol);
        assert(_symbol);

        return *_symbol;
    }

    const std::wstring Expression::ToString() const
    {
        switch (GetType())
        {
            case ExpressionType::Boolean:
                return *_boolean ? L"true" : L"false";

            case ExpressionType::Integer:
                return boost::lexical_cast<std::wstring>(*_integer);

            case ExpressionType::NativeMacro:
                return L"#native-macro";

            case ExpressionType::Null:
                return keywords::null;

            case ExpressionType::Real:
                return boost::lexical_cast<std::wstring>(*_real);

            case ExpressionType::String:
                return L"'" + *_string + L"'";

            case ExpressionType::Symbol:
                return *_symbol;

            case ExpressionType::List:
                {
                    std::wstring result = L"(";

                    for (const auto &expr : *_list)
                    {
                        assert(expr);
                        result += expr->ToString();
                        result += L" ";
                    }

                    boost::algorithm::trim_right(result);
                    result += L")";

                    return result;
                }

            default:
                assert(!"unexpected expression type");
                return L"#unknown";
        }
    }

    bool Expression::IsCallable() const
    {
        switch (GetType())
        {
            case ExpressionType::NativeMacro:
                return true;

            default:
                return false;
        }
    }

    bool Expression::IsTerminal() const
    {
        switch (GetType())
        {
            case ExpressionType::Boolean:
            case ExpressionType::Integer:
            case ExpressionType::Real:
            case ExpressionType::String:
            case ExpressionType::Null:
            case ExpressionType::NativeMacro:
                return true;

            default:
                return false;
        }
    }

    ExpressionType Expression::GetType() const
    {
        assert(_type > ExpressionType::Min);
        assert(_type < ExpressionType::Max);

        return _type;
    }

    ExpressionSptr Expression::MakeCopy() const
    {
        switch (GetType())
        {
            case ExpressionType::Boolean:
                return Expression::Boolean(*_boolean);

            case ExpressionType::Integer:
                return Expression::Integer(*_integer);

            case ExpressionType::Real:
                return Expression::Real(*_real);

            case ExpressionType::String:
                return Expression::String(*_string);

            case ExpressionType::Null:
                return Expression::Null();

            case ExpressionType::Symbol:
                return Expression::Symbol(*_symbol);

            case ExpressionType::List:
            {
                auto copy = Expression::List();

                for (const auto &item : *_list)
                {
                    copy->ListPushBack(item->MakeCopy());
                }

                return copy;
            }
        }

        assert(false);
        return ExpressionSptr();
    }

    ExpressionSptrList::const_iterator Expression::ListBegin() const
    {
        assert(_type == ExpressionType::List);
        assert(_list);

        return _list->begin();
    }

    ExpressionSptrList::const_iterator Expression::ListEnd() const
    {
        assert(_type == ExpressionType::List);
        assert(_list);

        return _list->end();
    }

    void Expression::ListAssign(const ExpressionSptrList::const_iterator &from, const ExpressionSptrList::const_iterator &to)
    {
        assert(_type == ExpressionType::List);
        assert(_list);
        assert(_list->empty());

        _list->assign(from, to);
    }

    int Expression::ListGetSize() const
    {
        assert(_type == ExpressionType::List);
        assert(_list);

        return _list->size();
    }

    ExpressionSptr Expression::ListHead() const
    {
        assert(_type == ExpressionType::List);
        assert(_list);

        return _list->front();
    }

    void Expression::ListPushBack(ExpressionSptr exp)
    {
        assert(exp);
        assert(_type == ExpressionType::List);
        assert(_list);

        _list->push_back(exp);
    }

    ExpressionSptr Expression::ListTail() const
    {
        assert(_type == ExpressionType::List);
        assert(_list);

        auto expr = List();
        expr->ListAssign(_list->begin() + 1, _list->end());

        return expr;
    }

    ExpressionSptr Expression::NativeMacroCall(ICallContext &c, const ExpressionSptr params)
    {
        assert(params);
        assert(_type == ExpressionType::NativeMacro);
        assert(_nativeMacro);

        return _nativeMacro(c, params);
    }

}