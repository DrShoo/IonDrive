#pragma once

namespace ionscript
{
    typedef std::shared_ptr<class Expression> ExpressionSptr;

    enum class ParserErrorCode
    {
        Invalid = 0,
        Min,
        ExpressionExpected,
        UnexpectedEof,
        LexerError,
        Max
    };

    struct ParserError : boost::noncopyable
    {
        ParserError(const std::wstring &filename, const ParserErrorCode parserErrorCode, const LexemeSptr lx);

        std::wstring FormatedMessage() const;

        const std::wstring Filename;
        const ParserErrorCode PrErrorCode;
        const LexemeSptr Lx;
    };

    typedef std::shared_ptr<ParserError> ParserErrorSptr;

    typedef std::list<ParserErrorSptr> ParserErrorList;

    class Parser : boost::noncopyable
    {
    public:
        Parser();

        const ParserErrorList& GetErrors() const;

        bool Init(const std::wstring &filename, const std::wstring &text);

        ExpressionSptr Next();

    private:
        ParserErrorList _errors;

        std::wstring _filename;

        std::unique_ptr<class Lexer> _lexer;
        
        void AddLexerError(LexemeSptr lx);

        void AddParserError(LexemeSptr lx, const ParserErrorCode errorCode);

        ExpressionSptr ReadExpression();

        ExpressionSptr ReadList();

    };

    typedef std::unique_ptr<Parser> ParserUptr;

}
