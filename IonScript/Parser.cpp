#include "stdafx.h"

#include "Expression.h"
#include "Keywords.h"
#include "Lexer.h"
#include "Parser.h"

using namespace common::log;

namespace ionscript
{
    ParserError::ParserError(const std::wstring &filename, const ParserErrorCode parserErrorCode, const LexemeSptr lx)
        : Filename(filename), PrErrorCode(parserErrorCode), Lx(lx)
    {
        assert(!filename.empty());
        assert(parserErrorCode > ParserErrorCode::Min);
        assert(parserErrorCode < ParserErrorCode::Max);

        assert(lx);
        lx->CheckInvariant();
    }

    std::wstring ParserError::FormatedMessage() const
    {
        if (PrErrorCode == ParserErrorCode::LexerError)
        {
            auto fmt = boost::wformat(L"parser error LE%04d: %s\n%s") % (int)Lx->Error;
            switch (Lx->Error)
            {
                case LexerErrorCode::InvalidSymbolCharacter:
                    fmt % (boost::wformat(L"unexpected character '%s' in symbol '%s'") % Lx->Text.back() % Lx->Text);
                    break;

                case LexerErrorCode::StringNotClosed:
                    fmt % L"string opened but not closed";
                    break;

                case LexerErrorCode::StringUnexpectedNewline:
                    fmt % L"unexpected newline in string";
                    break;

                case LexerErrorCode::UnknownLexeme:
                    fmt % (boost::wformat(L"unknown lexeme '%s'") % Lx->Text);
                    break;

                default:
                    assert(false);
                    break;
            }

            fmt % (boost::wformat(L"in file '%s' at line %d row %d") % Filename % Lx->Line % Lx->Row);

            return fmt.str();
        }

        auto fmt = boost::wformat(L"parser error PE%04d: %s\n%s") % (int)PrErrorCode;

        if (PrErrorCode == ParserErrorCode::ExpressionExpected)
        {
            fmt % (boost::wformat(L"unexpected lexeme '%s', expected '(' or atom") % Lx->Text);
            fmt % (boost::wformat(L"in file '%s' at line %d row %d") % Filename % Lx->Line % Lx->Row);
        }

        return fmt.str();
    }

    Parser::Parser()
    {

    }

    const ParserErrorList& Parser::GetErrors() const
    {
        return _errors;
    }

    bool Parser::Init(const std::wstring &filename, const std::wstring &text)
    {
        assert(!_lexer);
        assert(!filename.empty());

        Info(L"initializing parser with file '%s' (%u chars)", filename, text.length());
        
        _lexer.reset(new Lexer());
        _lexer->Init(text);

        _filename = filename;

        return true;
    }

    ExpressionSptr Parser::Next()
    {
        assert(_lexer);

        if (_lexer->Next())
        {
            return ReadExpression();
        }

        auto lx = _lexer->GetLexeme();
        if (lx)
        {
            AddLexerError(lx);
        }

        return ExpressionSptr();
    }

    void Parser::AddLexerError(LexemeSptr lx)
    {
        assert(lx);
        assert(lx->IsError());

        _errors.push_back(std::make_shared<ParserError>(
            _filename, ParserErrorCode::LexerError, lx
        ));
    }

    void Parser::AddParserError(LexemeSptr lx, const ParserErrorCode errorCode)
    {
        assert(_lexer);
        assert(errorCode > ParserErrorCode::Min);
        assert(errorCode < ParserErrorCode::Max);
        assert(lx);
        assert(!lx->IsError());

        _errors.push_back(std::make_shared<ParserError>(
            _filename, errorCode, lx
        ));
    }

    ExpressionSptr Parser::ReadExpression()
    {
        assert(_lexer);

        auto lx = _lexer->GetLexeme();
        assert(!lx->IsError());

        auto readLx = [&lx, this] 
        {
            if (lx->Type == LexemeType::String)
            {
                return Expression::String(lx->Text);
            }

            if (lx->Type == LexemeType::Integer)
            {
                return Expression::Integer(boost::lexical_cast<__int64>(
                    lx->Text
                ));
            }

            if (lx->Type == LexemeType::Symbol)
            {
                if (lx->Text == keywords::null)
                {
                    return Expression::Null();
                }

                return Expression::Symbol(lx->Text);
            }

            if (lx->Type == LexemeType::LeftBrace)
            {
                return ReadList();
            }

            AddParserError(lx, ParserErrorCode::ExpressionExpected);
            return ExpressionSptr();
        };

        auto expr = readLx();
        if (expr)
        {
            expr->SetSourceLocation(std::make_shared<Expression::SourceLocation>(
                _filename, lx->Line, lx->Row
            ));
        }

        return expr;
    }

    ExpressionSptr Parser::ReadList()
    {
        assert(_lexer);

        auto list = Expression::List();

        for (;;)
        {
            if (!_lexer->Next())
            {
                auto lx = _lexer->GetLexeme();
                if (lx)
                {
                    AddLexerError(lx);
                    return ExpressionSptr();
                }

                AddParserError(lx, ParserErrorCode::UnexpectedEof);
                return ExpressionSptr();
            }

            auto lx = _lexer->GetLexeme();

            if (lx->Type == LexemeType::RightBrace)
            {
                return list;
            }

            auto expr = ReadExpression();
            if (!expr)
            {
                return ExpressionSptr();
            }

            list->ListPushBack(expr);
        }
    }

}