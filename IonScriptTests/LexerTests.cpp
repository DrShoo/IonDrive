#include "stdafx.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Microsoft
{
    using namespace ionscript;

    namespace VisualStudio
    {
        namespace CppUnitTestFramework
        {
            template<> static std::wstring ToString<LexerErrorCode>(const LexerErrorCode& t) { RETURN_WIDE_STRING((int)t); }
            template<> static std::wstring ToString<LexemeType>(const LexemeType& t) { RETURN_WIDE_STRING((int)t); }
        }
    }
}

namespace IonScriptTests
{		
    using namespace ionscript;

	TEST_CLASS(LexerTests)
	{
	public:
		
		TEST_METHOD(TestLexemes)
		{
            Lexer lexer;
            lexer.Init(L"(printf\r\n'hello world!')");

            Assert::IsTrue(lexer.Next());
            auto lexeme = lexer.GetLexeme();
            Assert::IsTrue((bool)lexeme);
            Assert::AreEqual(LexemeType::LeftBrace, lexeme->Type);
            Assert::AreEqual<std::wstring>(L"(", lexeme->Text);
            Assert::AreEqual(1, lexeme->Line);
            Assert::AreEqual(1, lexeme->Row);
            Assert::AreEqual(LexerErrorCode::NoError, lexeme->Error);

            Assert::IsTrue(lexer.Next());
            lexeme = lexer.GetLexeme();
            Assert::IsTrue((bool)lexeme);
            Assert::AreEqual(LexemeType::Symbol, lexeme->Type);
            Assert::AreEqual<std::wstring>(L"printf", lexeme->Text);
            Assert::AreEqual(1, lexeme->Line);
            Assert::AreEqual(2, lexeme->Row);
            Assert::AreEqual(LexerErrorCode::NoError, lexeme->Error);

            Assert::IsTrue(lexer.Next());
            lexeme = lexer.GetLexeme();
            Assert::IsTrue((bool)lexeme);
            Assert::AreEqual(LexemeType::String, lexeme->Type);
            Assert::AreEqual<std::wstring>(L"'hello world!'", lexeme->Text);
            Assert::AreEqual(2, lexeme->Line);
            Assert::AreEqual(1, lexeme->Row);
            Assert::AreEqual(LexerErrorCode::NoError, lexeme->Error);

            Assert::IsTrue(lexer.Next());
            lexeme = lexer.GetLexeme();
            Assert::IsTrue((bool)lexeme);
            Assert::AreEqual(LexemeType::RightBrace, lexeme->Type);
            Assert::AreEqual<std::wstring>(L")", lexeme->Text);
            Assert::AreEqual(2, lexeme->Line);
            Assert::AreEqual(15, lexeme->Row);
            Assert::AreEqual(LexerErrorCode::NoError, lexeme->Error);

            Assert::IsFalse(lexer.Next());
            lexeme = lexer.GetLexeme();
            Assert::IsFalse((bool)lexeme);
		}

	};
}