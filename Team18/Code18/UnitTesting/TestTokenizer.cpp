#include <iostream>
#include <sstream>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "Tokenizer.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace UnitTesting {
    TEST_CLASS(TestTokenizer) {
        public:
        TEST_METHOD(TestTokenizerTokenize) {
            std::stringstream srcStream;
            srcStream << "assign a;\n";
            srcStream << "Select a pattern a(\"normSq\", _\"cenX * cenX\"_)";
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

        TEST_METHOD(TestTokenizerTokenizeSuchThat) {
          std::stringstream srcStream;
          srcStream << "variable v;\n";
          srcStream << "Select v such that Modifies (6, v)";
          spa::Tokenizer tokenizer;
          spa::Stream<spa::Token> tokenStream = tokenizer.tokenize(srcStream);
          bool matchResult = tokenStream.match({
            { spa::TOKEN_NAME, "variable" },
            { spa::TOKEN_NAME, "v" },
            { spa::TOKEN_SEMICOLON, ";" },
            { spa::TOKEN_NAME, "Select" },
            { spa::TOKEN_NAME, "v" },
            { spa::TOKEN_NAME, "such" },
            { spa::TOKEN_NAME, "that" },
            { spa::TOKEN_NAME, "Modifies" },
            { spa::TOKEN_OPEN_BRACKET, "(" },
            { spa::TOKEN_INTEGER, "6" },
            { spa::TOKEN_COMMA, "," },
            { spa::TOKEN_NAME, "v" },
            { spa::TOKEN_CLOSE_BRACKET, ")" },
            });
          Assert::IsTrue(matchResult);
        }
    };
}  // namespace UnitTesting
