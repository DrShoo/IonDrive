#include "stdafx.h"

#include "Lexer.h"

namespace
{
    bool issymbeg(const wchar_t ch)
    {
        return (::iswalpha(ch) != 0);
    }

    bool issymbody(const wchar_t ch)
    {
        return (::iswalpha(ch) != 0) || (ch == L'-') || (ch == L'_');
    }

    bool isstrbegend(const wchar_t ch)
    {
        return ((ch == L'"') || (ch == L'\''));
    }
}

namespace ionscript
{

    Lexeme::Lexeme(const LexemeType type, const std::wstring &text, const int line, const int row, const LexerErrorCode error)
        : Type(type)
        , Text(text)
        , Line(line)
        , Row(row)
        , Error(error)
    {
        CheckInvariant();
    }

    Lexeme::Lexeme(const std::wstring &text, const int line, const int row, const LexerErrorCode error)
        : Lexeme(LexemeType::Error, text, line, row, error)
    {
    }

    void Lexeme::CheckInvariant() const
    {
        assert(Type >= LexemeType::Min);
        assert(Type <= LexemeType::Max);
        assert(Error >= LexerErrorCode::Min);
        assert(Error <= LexerErrorCode::Max);
        assert(Line >= 1);
        assert(Row >= 1);
    }

    bool Lexeme::IsError() const
    {
        CheckInvariant();
        return (Type == LexemeType::Error);
    }

    Lexer::Lexer()
    {
        _beginCursor = std::numeric_limits<unsigned>::max();
        _currentCursor = std::numeric_limits<unsigned>::max();
        _beginLine = -1;
        _beginRow = -1;
        _currentLine = -1;
        _currentRow = -1;
    }

    int Lexer::GetCurrentLine() const
    {
        assert(_currentLine >= 1);
        return _currentLine;
    }

    int Lexer::GetCurrentRow() const
    {
        assert(_currentRow >= 1);
        return _currentRow;
    }

    LexemeSptr Lexer::GetLexeme()
    {
        return _lexeme;
    }

    void Lexer::Init(const std::wstring &text)
    {
        assert(_beginCursor == -1);

        _text = text;
        _beginCursor = 0;
        _currentCursor = 0;
        _beginLine = 1;
        _beginRow = 1;
        _currentLine = 1;
        _currentRow = 1;
    }

    bool Lexer::Next()
    {
        assert(_currentCursor >= 0);
        assert(_currentCursor <= _text.length());

        EatSpace();

        if (_currentCursor == _text.length())
        {
            _lexeme.reset();
            return false; 
        }

        _beginCursor = _currentCursor;
        _beginLine = _currentLine;
        _beginRow = _currentRow;

        const std::function<LexemeSptr()> readers[] = {
            [&] { return TryReadSimple(); },
            [&] { return TryReadSymbol(); },
            [&] { return TryReadString(); }
        };

        for (const auto &reader : readers)
        {
            auto nextLexeme = reader();
            if (nextLexeme)
            {
                _lexeme = nextLexeme;
                return !_lexeme->IsError();
            }
        }

        _lexeme = NewErrorLexeme(LexerErrorCode::UnknownLexeme);
        return false;
    }

    void Lexer::EatSpace()
    {
        for (;; ++_currentCursor)
        {
            assert(_currentCursor >= 0);
            assert(_currentCursor <= (_text.length() + 1));

            if (_currentCursor == _text.length())
            {
                return;
            }

            const auto ch = _text[_currentCursor];

            if (ch == L'\r')
            {
                continue;
            }

            if (ch == L'\n')
            {
                _currentRow = 1;
                ++_currentLine;
                continue;
            }

            if (::iswspace(ch))
            { 
                ++_currentRow;
                continue;
            }

            return;
        }
    }

    std::wstring Lexer::GetLastLexemeText(const LexemeType type) const
    {
        assert(_beginCursor >= 0);
        assert(_beginCursor <= (_text.length() + 1));
        assert(_beginCursor <= _currentCursor);
        assert(type >= LexemeType::Min);
        assert(type <= LexemeType::Max);

        const auto afterEof = (_beginCursor == (_text.length() + 1));
        if (afterEof)
        {
            return L"";
        }

        auto count = (_currentCursor - _beginCursor);
        if ((type == LexemeType::Error) && 
            (_currentCursor < _text.length()))
        {
            ++count;
        }

        return _text.substr(_beginCursor, count);
    }

    LexemeSptr Lexer::NewPlainLexeme(const LexemeType type)
    {
        assert(_beginLine >= 1);
        assert(_beginRow >= 1);

        return std::make_shared<Lexeme>(type, GetLastLexemeText(type), _beginLine, _beginRow, LexerErrorCode::NoError);
    }

    LexemeSptr Lexer::NewErrorLexeme(const LexerErrorCode error)
    {
        assert(_beginLine >= 1);
        assert(_beginRow >= 1);

        return std::make_shared<Lexeme>(GetLastLexemeText(LexemeType::Error), _beginLine, _beginRow, error);
    }

    LexemeSptr Lexer::TryReadSimple()
    {
        assert(_currentCursor <= _text.length());

        if (_text[_currentCursor] == L'(')
        {
            ++_currentCursor;
            ++_currentRow;
            return NewPlainLexeme(LexemeType::LeftBrace);
        }

        if (_text[_currentCursor] == L')')
        {
            ++_currentCursor;
            ++_currentRow;
            return NewPlainLexeme(LexemeType::RightBrace);
        }

        if (_text[_currentCursor] == L'\'')
        {
            ++_currentCursor;
            ++_currentRow;
            return NewPlainLexeme(LexemeType::Quote);
        }

        return LexemeSptr();
    }

    LexemeSptr Lexer::TryReadString()
    {
        if (!isstrbegend(_text[_currentCursor]))
        {
            return LexemeSptr();
        }

        ++_currentRow;
        ++_currentCursor;

        for (;; ++_currentRow, ++_currentCursor)
        {
            if (_currentCursor == _text.length())
            {
                return NewErrorLexeme(LexerErrorCode::StringNotClosed);
            }

            const auto ch = _text[_currentCursor];
            if (ch == L'\n')
            {
                return NewErrorLexeme(LexerErrorCode::StringUnexpectedNewline);
            }

            if (isstrbegend(ch))
            {
                ++_currentRow;
                ++_currentCursor;
                break;
            }
        }

        return NewPlainLexeme(LexemeType::String);
    }

    LexemeSptr Lexer::TryReadSymbol()
    {
        if (!issymbeg(_text[_currentCursor]))
        {
            return LexemeSptr();
        }

        for (;; ++_currentRow, ++_currentCursor)
        {
            const auto ch = _text[_currentCursor];
            if (issymbody(ch))
            { 
                continue;
            }

            const bool ended = ::iswspace(ch) || (ch == L')');
            if (ended)
            {
                break;
            }

            return NewErrorLexeme(LexerErrorCode::InvalidSymbolCharacter);
        }

        return NewPlainLexeme(LexemeType::Symbol);
    }

}