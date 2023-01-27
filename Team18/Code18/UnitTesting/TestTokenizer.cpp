#include "stdafx.h"
#include "CppUnitTest.h"

#include "Tokenizer.h"

#include <iostream>
#include <sstream>

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace UnitTesting {
    TEST_CLASS(TestTokenizer) {
        public:
        TEST_METHOD(TestTokenizerTokenize) {
            std::stringstream srcStream;
            srcStream << "assign a;\nSelect a pattern a(\"normSq\", _\"cenX * cenX\"_)";
            spa::Tokenizer tokenizer;
            spa::Stream<spa::Token> tokenStream = tokenizer.tokenize(srcStream);
            bool matchResult = tokenStream.match({
                { spa::TOKEN_NAME, "assign" },
                { spa::TOKEN_NAME, "a" },
                { spa::TOKEN_SEMICOLON, ";" },
                { spa::TOKEN_NAME, "Select" },
                { spa::TOKEN_NAME, "a" },
                { spa::TOKEN_NAME, "pattern" },
                { spa::TOKEN_NAME, "a" },
                { spa::TOKEN_OPEN_BRACKET, "(" },
                { spa::TOKEN_DOUBLE_QUOTES, "\"" },
                { spa::TOKEN_NAME, "normSq" },
                { spa::TOKEN_DOUBLE_QUOTES, "\"" },
                { spa::TOKEN_COMMA, "," },
                { spa::TOKEN_UNDERSCORE, "_" },
                { spa::TOKEN_DOUBLE_QUOTES, "\"" },
                { spa::TOKEN_NAME, "cenX" },
                { spa::TOKEN_MULTIPLY, "*" },
                { spa::TOKEN_NAME, "cenX" },
                { spa::TOKEN_DOUBLE_QUOTES, "\"" },
                { spa::TOKEN_UNDERSCORE, "_" },
                { spa::TOKEN_CLOSE_BRACKET, ")" },
            });
            Assert::IsTrue(matchResult);
        }
    };
}
