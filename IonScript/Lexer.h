#pragma once

#include "Defs.h"

namespace ionscript
{
    enum class LexerErrorCode
    {
        Invalid,
        Min,

        NoError,
        UnknownLexeme,
        InvalidSymbolCharacter,
        StringNotClosed,
        StringUnexpectedNewline,

        Max
    };

    enum class LexemeType
    {
        Invalid,
        Min,

        Error,
        LeftBrace,
        RightBrace,
        Symbol,
        String,
        Quote,

        Max
    };

    struct Lexeme : boost::noncopyable
    {
        Lexeme(const LexemeType type, const std::wstring &text, const int line, const int row, const LexerErrorCode error);
        Lexeme(const std::wstring &text, const int line, const int row, const LexerErrorCode error);

        void CheckInvariant() const;
        bool IsError() const;
        
        const LexemeType Type;
        const std::wstring Text;
        const int Line;
        const int Row;
        const LexerErrorCode Error;
    };

    typedef std::shared_ptr<Lexeme> LexemeSptr;

    class Lexer : boost::noncopyable
    {
    public:
        Lexer();
        
        int GetCurrentLine() const;

        int GetCurrentRow() const;

        LexemeSptr GetLexeme();

        void Init(const std::wstring &text);

        bool Next();

    private:
        void EatSpace();

        std::wstring GetLastLexemeText(const LexemeType type) const;

        LexemeSptr NewPlainLexeme(const LexemeType type);

        LexemeSptr NewErrorLexeme(const LexerErrorCode error);
        
        LexemeSptr TryReadSimple();

        LexemeSptr TryReadString();

        LexemeSptr TryReadSymbol();

        std::wstring _text;

        unsigned _beginCursor;

        unsigned _currentCursor;

        int _beginLine;
        int _beginRow;

        int _currentLine;
        int _currentRow;

        LexemeSptr _lexeme;
    };
}