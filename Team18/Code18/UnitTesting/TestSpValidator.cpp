#include <iostream>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "SpValidator.h"
#include "PKB.h"
#include "SP.h"
#include "Token.h"

namespace spa {
class Token;
}

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace UnitTesting {
    TEST_CLASS(TestSpValidator) {
        //  procedure proc {x = 1;}
        TEST_METHOD(TestSpValidatorValidProcOne) {
            spa::Stream<spa::Token> tokens;
            std::vector<spa::Token> expectedTokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "proc"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({ spa::TOKEN_NAME, "x" });
            tokens.pushBack({ spa::TOKEN_EQUAL, "=" });
            tokens.pushBack({ spa::TOKEN_INTEGER, "1" });
            tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});

            expectedTokens.push_back({ spa::TOKEN_PROCEDURE, "procedure" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "proc" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "x" });
            expectedTokens.push_back({ spa::TOKEN_EQUAL, "=" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "1" });
            expectedTokens.push_back({ spa::TOKEN_SEMICOLON, ";" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            spa::SpValidator validator(tokens);
            Assert::IsTrue(validator.validateGrammar());
            Assert::IsTrue(validator.getUpdatedStream().match(expectedTokens));
        }

        //  procedure print {x = 1;}
        TEST_METHOD(TestSpValidatorValidProcTwo) {
            spa::Stream<spa::Token> tokens;
            std::vector<spa::Token> expectedTokens;
            tokens.pushBack({ spa::TOKEN_NAME, "procedure" });
            tokens.pushBack({ spa::TOKEN_NAME, "print" });
            tokens.pushBack({ spa::TOKEN_OPEN_BRACE, "{" });
            tokens.pushBack({ spa::TOKEN_NAME, "x" });
            tokens.pushBack({ spa::TOKEN_EQUAL, "=" });
            tokens.pushBack({ spa::TOKEN_INTEGER, "1" });
            tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
            tokens.pushBack({ spa::TOKEN_CLOSE_BRACE, "}" });

            expectedTokens.push_back({ spa::TOKEN_PROCEDURE, "procedure" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "print" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "x" });
            expectedTokens.push_back({ spa::TOKEN_EQUAL, "=" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "1" });
            expectedTokens.push_back({ spa::TOKEN_SEMICOLON, ";" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            spa::SpValidator validator(tokens);
            Assert::IsTrue(validator.validateGrammar());
            Assert::IsTrue(validator.getUpdatedStream().match(expectedTokens));
        }

        //  procedure print {x = 1;}
        TEST_METHOD(TestSpValidatorValidProcThree) {
            spa::Stream<spa::Token> tokens;
            std::vector<spa::Token> expectedTokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "print"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({ spa::TOKEN_NAME, "x" });
            tokens.pushBack({ spa::TOKEN_EQUAL, "=" });
            tokens.pushBack({ spa::TOKEN_INTEGER, "1" });
            tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});

            expectedTokens.push_back({ spa::TOKEN_PROCEDURE, "procedure" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "print" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "x" });
            expectedTokens.push_back({ spa::TOKEN_EQUAL, "=" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "1" });
            expectedTokens.push_back({ spa::TOKEN_SEMICOLON, ";" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            spa::SpValidator validator(tokens);
            Assert::IsTrue(validator.validateGrammar());
            Assert::IsTrue(validator.getUpdatedStream().match(expectedTokens));
        }

        //  procedure 123 { x = z + v; }
        TEST_METHOD(TestSpValidatorInvalidProcOne) {
            spa::Stream<spa::Token> tokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_INTEGER, "123"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "x"});
            tokens.pushBack({spa::TOKEN_EQUAL, "="});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_PLUS, "+"});
            tokens.pushBack({spa::TOKEN_NAME, "v"});
            tokens.pushBack({spa::TOKEN_SEMICOLON, ";"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            spa::SpValidator validator(tokens);
            Assert::IsFalse(validator.validateGrammar());
        }

        //  procedure{}
        TEST_METHOD(TestSpValidatorInvalidProcTwo) {
            spa::Stream<spa::Token> tokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            spa::SpValidator validator(tokens);
            Assert::IsFalse(validator.validateGrammar());
        }

        //  procedure 1{}
        TEST_METHOD(TestSpValidatorInvalidProcThree) {
            spa::Stream<spa::Token> tokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_INTEGER, "1"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            spa::SpValidator validator(tokens);
            Assert::IsFalse(validator.validateGrammar());
        }

        //  procedure proc {;}
        TEST_METHOD(TestSpValidatorInvalidProcFour) {
            spa::Stream<spa::Token> tokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "proc"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_SEMICOLON, ";"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            spa::SpValidator validator(tokens);
            Assert::IsFalse(validator.validateGrammar());
        }

        //  procedure proc ;
        TEST_METHOD(TestSpValidatorInvalidProcFive) {
            spa::Stream<spa::Token> tokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "proc"});
            tokens.pushBack({spa::TOKEN_SEMICOLON, ";"});
            spa::SpValidator validator(tokens);
            Assert::IsFalse(validator.validateGrammar());
        }

        //  procedure ;
        TEST_METHOD(TestSpValidatorInvalidProcSix) {
            spa::Stream<spa::Token> tokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_SEMICOLON, ";"});
            spa::SpValidator validator(tokens);
        }

        //  proc { x = z + v ;}
        TEST_METHOD(TestSpValidatorInvalidProcSeven) {
            spa::Stream<spa::Token> tokens;
            tokens.pushBack({spa::TOKEN_NAME, "proc"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "x"});
            tokens.pushBack({spa::TOKEN_EQUAL, "="});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_NAME, "+"});
            tokens.pushBack({spa::TOKEN_NAME, "v"});
            tokens.pushBack({spa::TOKEN_SEMICOLON, ";"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});

            spa::SpValidator validator(tokens);
            Assert::IsFalse(validator.validateGrammar());
        }

        //  procedure proc 12 {}
        TEST_METHOD(TestSpValidatorInvalidProcEight) {
            spa::Stream<spa::Token> tokens;
            tokens.pushBack({spa::TOKEN_NAME, "proc"});
            tokens.pushBack({spa::TOKEN_INTEGER, "12"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            spa::SpValidator validator(tokens);
            Assert::IsFalse(validator.validateGrammar());
        }

        //  procedure proc {
        TEST_METHOD(TestSpValidatorInvalidProcNine) {
            spa::Stream<spa::Token> tokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "proc"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            spa::SpValidator validator(tokens);
            Assert::IsFalse(validator.validateGrammar());
        }

        //  procedure proc }
        TEST_METHOD(TestSpValidatorInvalidProcTen) {
            spa::Stream<spa::Token> tokens;
            tokens.pushBack({ spa::TOKEN_NAME, "procedure" });
            tokens.pushBack({ spa::TOKEN_NAME, "proc" });
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            spa::SpValidator validator(tokens);
            Assert::IsFalse(validator.validateGrammar());
        }

        //  procedure proc{ () }
        TEST_METHOD(TestSpValidatorInvalidProcEleven) {
            spa::Stream<spa::Token> tokens;
            tokens.pushBack({ spa::TOKEN_NAME, "procedure" });
            tokens.pushBack({ spa::TOKEN_NAME, "proc" });
            tokens.pushBack({ spa::TOKEN_OPEN_BRACE, "{" });
            tokens.pushBack({ spa::TOKEN_OPEN_BRACKET, "(" });
            tokens.pushBack({ spa::TOKEN_CLOSE_BRACKET, ")" });
            tokens.pushBack({ spa::TOKEN_CLOSE_BRACE, "}" });
            spa::SpValidator validator(tokens);
            Assert::IsFalse(validator.validateGrammar());
        }

        //  procedure proc {}
        TEST_METHOD(TestSpValidatorInvalidProcTwelve) {
            spa::Stream<spa::Token> tokens;
            tokens.pushBack({ spa::TOKEN_NAME, "procedure" });
            tokens.pushBack({ spa::TOKEN_NAME, "proc" });
            tokens.pushBack({ spa::TOKEN_OPEN_BRACE, "{" });
            tokens.pushBack({ spa::TOKEN_CLOSE_BRACE, "}" });
            spa::SpValidator validator(tokens);
            Assert::IsFalse(validator.validateGrammar());
        }

        // procedure a { x = 1; } procedure b { x = 1; } procedure proc { read print;}
        TEST_METHOD(TestSpValidatorValidReadOne) {
            spa::Stream<spa::Token> tokens;
            std::vector<spa::Token> expectedTokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "a"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({ spa::TOKEN_NAME, "x" });
            tokens.pushBack({ spa::TOKEN_EQUAL, "=" });
            tokens.pushBack({ spa::TOKEN_INTEGER, "1" });
            tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "b"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({ spa::TOKEN_NAME, "x" });
            tokens.pushBack({ spa::TOKEN_EQUAL, "=" });
            tokens.pushBack({ spa::TOKEN_INTEGER, "1" });
            tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "proc"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "read"});
            tokens.pushBack({spa::TOKEN_NAME, "print"});
            tokens.pushBack({spa::TOKEN_SEMICOLON, ";"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});

            expectedTokens.push_back({ spa::TOKEN_PROCEDURE, "procedure" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "a" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "x" });
            expectedTokens.push_back({ spa::TOKEN_EQUAL, "=" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "1" });
            expectedTokens.push_back({ spa::TOKEN_SEMICOLON, ";" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            expectedTokens.push_back({ spa::TOKEN_PROCEDURE, "procedure" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "b" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "x" });
            expectedTokens.push_back({ spa::TOKEN_EQUAL, "=" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "1" });
            expectedTokens.push_back({ spa::TOKEN_SEMICOLON, ";" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            expectedTokens.push_back({ spa::TOKEN_PROCEDURE, "procedure" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "proc" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_READ, "read" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "print" });
            expectedTokens.push_back({ spa::TOKEN_SEMICOLON, ";" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            spa::SpValidator validator(tokens);
            Assert::IsTrue(validator.validateGrammar());
            Assert::IsTrue(validator.getUpdatedStream().match(expectedTokens));
        }

        //  procedure proc { read print;}
        TEST_METHOD(TestSpValidatorValidReadTwo) {
            spa::Stream<spa::Token> tokens;
            std::vector<spa::Token> expectedTokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "proc"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "read"});
            tokens.pushBack({spa::TOKEN_NAME, "print"});
            tokens.pushBack({spa::TOKEN_SEMICOLON, ";"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});

            expectedTokens.push_back({ spa::TOKEN_PROCEDURE, "procedure" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "proc" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_READ, "read" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "print" });
            expectedTokens.push_back({ spa::TOKEN_SEMICOLON, ";" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            spa::SpValidator validator(tokens);
            Assert::IsTrue(validator.validateGrammar());
            Assert::IsTrue(validator.getUpdatedStream().match(expectedTokens));
        }

        //  procedure proc { read read;}
        TEST_METHOD(TestSpValidatorValidReadThree) {
            spa::Stream<spa::Token> tokens;
            std::vector<spa::Token> expectedTokens;
            tokens.pushBack({ spa::TOKEN_NAME, "procedure" });
            tokens.pushBack({ spa::TOKEN_NAME, "proc" });
            tokens.pushBack({ spa::TOKEN_OPEN_BRACE, "{" });
            tokens.pushBack({ spa::TOKEN_NAME, "read" });
            tokens.pushBack({ spa::TOKEN_NAME, "read" });
            tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
            tokens.pushBack({ spa::TOKEN_CLOSE_BRACE, "}" });

            expectedTokens.push_back({ spa::TOKEN_PROCEDURE, "procedure" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "proc" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_READ, "read" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "read" });
            expectedTokens.push_back({ spa::TOKEN_SEMICOLON, ";" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            spa::SpValidator validator(tokens);
            Assert::IsTrue(validator.validateGrammar());
            Assert::IsTrue(validator.getUpdatedStream().match(expectedTokens));
        }

        // procedure proc { read 1;}
        TEST_METHOD(TestSpValidatorInvalidReadOne) {
            spa::Stream<spa::Token> tokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "proc"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "read"});
            tokens.pushBack({spa::TOKEN_INTEGER, "1"});
            tokens.pushBack({spa::TOKEN_SEMICOLON, ";"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});

            spa::SpValidator validator(tokens);
            Assert::IsFalse(validator.validateGrammar());
        }

        // procedure proc { read ab!c;}
        TEST_METHOD(TestSpValidatorInvalidReadTwo) {
            spa::Stream<spa::Token> tokens;
            tokens.pushBack({ spa::TOKEN_NAME, "procedure" });
            tokens.pushBack({ spa::TOKEN_NAME, "proc" });
            tokens.pushBack({ spa::TOKEN_OPEN_BRACE, "{" });
            tokens.pushBack({ spa::TOKEN_NAME, "read" });
            tokens.pushBack({ spa::TOKEN_NAME, "ab" });
            tokens.pushBack({ spa::TOKEN_BOOL_NOT, "!" });
            tokens.pushBack({ spa::TOKEN_NAME, "c" });
            tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
            tokens.pushBack({ spa::TOKEN_CLOSE_BRACE, "}" });

            spa::SpValidator validator(tokens);
            Assert::IsFalse(validator.validateGrammar());
        }


        //  procedure proc { print call;}
        TEST_METHOD(TestSpValidatorValidPrintOne) {
            spa::Stream<spa::Token> tokens;
            std::vector<spa::Token> expectedTokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "proc"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "print"});
            tokens.pushBack({spa::TOKEN_NAME, "call"});
            tokens.pushBack({spa::TOKEN_SEMICOLON, ";"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});

            expectedTokens.push_back({ spa::TOKEN_PROCEDURE, "procedure" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "proc" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_PRINT, "print" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "call" });
            expectedTokens.push_back({ spa::TOKEN_SEMICOLON, ";" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            spa::SpValidator validator(tokens);
            Assert::IsTrue(validator.validateGrammar());
            Assert::IsTrue(validator.getUpdatedStream().match(expectedTokens));
        }

        //  procedure proc { print x;}
        TEST_METHOD(TestSpValidatorValidPrintTwo) {
            spa::Stream<spa::Token> tokens;
            std::vector<spa::Token> expectedTokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "proc"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "print"});
            tokens.pushBack({spa::TOKEN_NAME, "x"});
            tokens.pushBack({spa::TOKEN_SEMICOLON, ";"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});

            expectedTokens.push_back({ spa::TOKEN_PROCEDURE, "procedure" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "proc" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_PRINT, "print" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "x" });
            expectedTokens.push_back({ spa::TOKEN_SEMICOLON, ";" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            spa::SpValidator validator(tokens);
            Assert::IsTrue(validator.validateGrammar());
            Assert::IsTrue(validator.getUpdatedStream().match(expectedTokens));
        }

        //  procedure proc { print 2z;}
        TEST_METHOD(TestSpValidatorInvalidPrintOne) {
            spa::Stream<spa::Token> tokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "proc"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "print"});
            tokens.pushBack({spa::TOKEN_INTEGER, "2"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_SEMICOLON, ";"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});

            spa::SpValidator validator(tokens);
            Assert::IsFalse(validator.validateGrammar());
        }

        //  assign
        //  procedure z { x = 1;}
        TEST_METHOD(TestSpValidatorAssignOne) {
            spa::Stream<spa::Token> tokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "x"});
            tokens.pushBack({spa::TOKEN_EQUAL, "="});
            tokens.pushBack({spa::TOKEN_INTEGER, "1"});
            tokens.pushBack({spa::TOKEN_SEMICOLON, ";"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});

            spa::SpValidator validator(tokens);
            Assert::IsTrue(validator.validateGrammar());
        }

        //  procedure z { x = a;}
        TEST_METHOD(TestSpValidatorAssignTwo) {
            spa::Stream<spa::Token> tokens;
            std::vector<spa::Token> expectedTokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "x"});
            tokens.pushBack({spa::TOKEN_EQUAL, "="});
            tokens.pushBack({spa::TOKEN_NAME, "a"});
            tokens.pushBack({spa::TOKEN_SEMICOLON, ";"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});

            expectedTokens.push_back({ spa::TOKEN_PROCEDURE, "procedure" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "z" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "x" });
            expectedTokens.push_back({ spa::TOKEN_EQUAL, "=" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "a" });
            expectedTokens.push_back({ spa::TOKEN_SEMICOLON, ";" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            spa::SpValidator validator(tokens);
            Assert::IsTrue(validator.validateGrammar());
            Assert::IsTrue(validator.getUpdatedStream().match(expectedTokens));
        }

        //  procedure z { x = a * 12 + 5;}
        TEST_METHOD(TestSpValidatorAssignThree) {
            spa::Stream<spa::Token> tokens;
            std::vector<spa::Token> expectedTokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "x"});
            tokens.pushBack({spa::TOKEN_EQUAL, "="});
            tokens.pushBack({spa::TOKEN_NAME, "a"});
            tokens.pushBack({spa::TOKEN_MULTIPLY, "*"});
            tokens.pushBack({spa::TOKEN_INTEGER, "12"});
            tokens.pushBack({spa::TOKEN_PLUS, "+"});
            tokens.pushBack({spa::TOKEN_INTEGER, "5"});
            tokens.pushBack({spa::TOKEN_SEMICOLON, ";"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});

            expectedTokens.push_back({ spa::TOKEN_PROCEDURE, "procedure" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "z" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "x" });
            expectedTokens.push_back({ spa::TOKEN_EQUAL, "=" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "a" });
            expectedTokens.push_back({ spa::TOKEN_MULTIPLY, "*" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "12" });
            expectedTokens.push_back({ spa::TOKEN_PLUS, "+" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "5" });
            expectedTokens.push_back({ spa::TOKEN_SEMICOLON, ";" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            spa::SpValidator validator(tokens);
            Assert::IsTrue(validator.validateGrammar());
            Assert::IsTrue(validator.getUpdatedStream().match(expectedTokens));
        }

        //  procedure z { x = a % 12 + 5;}
        TEST_METHOD(TestSpValidatorAssignFour) {
            spa::Stream<spa::Token> tokens;
            std::vector<spa::Token> expectedTokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "x"});
            tokens.pushBack({spa::TOKEN_EQUAL, "="});
            tokens.pushBack({spa::TOKEN_NAME, "a"});
            tokens.pushBack({spa::TOKEN_MODULO, "%"});
            tokens.pushBack({spa::TOKEN_INTEGER, "12"});
            tokens.pushBack({spa::TOKEN_PLUS, "+"});
            tokens.pushBack({spa::TOKEN_INTEGER, "5"});
            tokens.pushBack({spa::TOKEN_SEMICOLON, ";"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});

            expectedTokens.push_back({ spa::TOKEN_PROCEDURE, "procedure" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "z" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "x" });
            expectedTokens.push_back({ spa::TOKEN_EQUAL, "=" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "a" });
            expectedTokens.push_back({ spa::TOKEN_MODULO, "%" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "12" });
            expectedTokens.push_back({ spa::TOKEN_PLUS, "+" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "5" });
            expectedTokens.push_back({ spa::TOKEN_SEMICOLON, ";" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            spa::SpValidator validator(tokens);
            Assert::IsTrue(validator.validateGrammar());
            Assert::IsTrue(validator.getUpdatedStream().match(expectedTokens));
        }

        //  procedure z { x = x + 5;}
        TEST_METHOD(TestSpValidatorAssignFive) {
            spa::Stream<spa::Token> tokens;
            std::vector<spa::Token> expectedTokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "x"});
            tokens.pushBack({spa::TOKEN_EQUAL, "="});
            tokens.pushBack({spa::TOKEN_NAME, "x"});
            tokens.pushBack({spa::TOKEN_PLUS, "+"});
            tokens.pushBack({spa::TOKEN_INTEGER, "5"});
            tokens.pushBack({spa::TOKEN_SEMICOLON, ";"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});

            expectedTokens.push_back({ spa::TOKEN_PROCEDURE, "procedure" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "z" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "x" });
            expectedTokens.push_back({ spa::TOKEN_EQUAL, "=" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "x" });
            expectedTokens.push_back({ spa::TOKEN_PLUS, "+" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "5" });
            expectedTokens.push_back({ spa::TOKEN_SEMICOLON, ";" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            spa::SpValidator validator(tokens);
            Assert::IsTrue(validator.validateGrammar());
            Assert::IsTrue(validator.getUpdatedStream().match(expectedTokens));
        }

        // procedure z { x x = a;}
        TEST_METHOD(TestSpValidatorInvalidAssignOne) {
            spa::Stream<spa::Token> tokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "x"});
            tokens.pushBack({spa::TOKEN_NAME, "x"});
            tokens.pushBack({spa::TOKEN_EQUAL, "="});
            tokens.pushBack({spa::TOKEN_NAME, "a"});
            tokens.pushBack({spa::TOKEN_SEMICOLON, ";"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            spa::SpValidator validator(tokens);
            Assert::IsFalse(validator.validateGrammar());
        }

        // procedure z { x = 1 a;}
        TEST_METHOD(TestSpValidatorInvalidAssignTwo) {
            spa::Stream<spa::Token> tokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "x"});
            tokens.pushBack({spa::TOKEN_EQUAL, "="});
            tokens.pushBack({spa::TOKEN_INTEGER, "1"});
            tokens.pushBack({spa::TOKEN_NAME, "a"});
            tokens.pushBack({spa::TOKEN_SEMICOLON, ";"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            spa::SpValidator validator(tokens);
            Assert::IsFalse(validator.validateGrammar());
        }

        // procedure z { x = 1 1;}
        TEST_METHOD(TestSpValidatorInvalidAssignThree) {
            spa::Stream<spa::Token> tokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "x"});
            tokens.pushBack({spa::TOKEN_EQUAL, "="});
            tokens.pushBack({spa::TOKEN_INTEGER, "1"});
            tokens.pushBack({spa::TOKEN_INTEGER, "1"});
            tokens.pushBack({spa::TOKEN_SEMICOLON, ";"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            spa::SpValidator validator(tokens);
            Assert::IsFalse(validator.validateGrammar());
        }


        //  procedure z { x = 1 %;}
        TEST_METHOD(TestSpValidatorInvalidAssignFour) {
            spa::Stream<spa::Token> tokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "x"});
            tokens.pushBack({spa::TOKEN_EQUAL, "="});
            tokens.pushBack({spa::TOKEN_INTEGER, "1"});
            tokens.pushBack({spa::TOKEN_MODULO, "%"});
            tokens.pushBack({spa::TOKEN_SEMICOLON, ";"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            spa::SpValidator validator(tokens);
            Assert::IsFalse(validator.validateGrammar());
        }

        //  procedure z { 10 = x;}
        TEST_METHOD(TestSpValidatorInvalidAssignFive) {
            spa::Stream<spa::Token> tokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_INTEGER, "10"});
            tokens.pushBack({spa::TOKEN_EQUAL, "="});
            tokens.pushBack({spa::TOKEN_NAME, "x"});
            tokens.pushBack({spa::TOKEN_SEMICOLON, ";"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});

            spa::SpValidator validator(tokens);
            Assert::IsFalse(validator.validateGrammar());
        }

        //  expr
        //  procedure z { x = (5/ 1) - 6;}
        TEST_METHOD(TestSpValidatorValidExprOne) {
            spa::Stream<spa::Token> tokens;
            std::vector<spa::Token> expectedTokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "x"});
            tokens.pushBack({spa::TOKEN_EQUAL, "="});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_INTEGER, "5"});
            tokens.pushBack({spa::TOKEN_DIVIDE, "/"});
            tokens.pushBack({spa::TOKEN_INTEGER, "1"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_MINUS, "-"});
            tokens.pushBack({spa::TOKEN_INTEGER, "6"});
            tokens.pushBack({spa::TOKEN_SEMICOLON, ";"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});

            expectedTokens.push_back({ spa::TOKEN_PROCEDURE, "procedure" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "z" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "x" });
            expectedTokens.push_back({ spa::TOKEN_EQUAL, "=" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "5" });
            expectedTokens.push_back({ spa::TOKEN_DIVIDE, "/" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "1" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_MINUS, "-" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "6" });
            expectedTokens.push_back({ spa::TOKEN_SEMICOLON, ";" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            spa::SpValidator validator(tokens);
            Assert::IsTrue(validator.validateGrammar());
            Assert::IsTrue(validator.getUpdatedStream().match(expectedTokens));
        }

        //  procedure z { x = (5 * 10);}
        TEST_METHOD(TestSpValidatorValidExprTwo) {
            spa::Stream<spa::Token> tokens;
            std::vector<spa::Token> expectedTokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "x"});
            tokens.pushBack({spa::TOKEN_EQUAL, "="});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_INTEGER, "5"});
            tokens.pushBack({spa::TOKEN_MULTIPLY, "*"});
            tokens.pushBack({spa::TOKEN_INTEGER, "10"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_SEMICOLON, ";"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});

            expectedTokens.push_back({ spa::TOKEN_PROCEDURE, "procedure" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "z" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "x" });
            expectedTokens.push_back({ spa::TOKEN_EQUAL, "=" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "5" });
            expectedTokens.push_back({ spa::TOKEN_MULTIPLY, "*" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "10" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_SEMICOLON, ";" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            spa::SpValidator validator(tokens);
            Assert::IsTrue(validator.validateGrammar());
            Assert::IsTrue(validator.getUpdatedStream().match(expectedTokens));
        }

        //  procedure z { x = ( 8 + 5 * 10);}
        TEST_METHOD(TestSpValidatorValidExprThree) {
            spa::Stream<spa::Token> tokens;
            std::vector<spa::Token> expectedTokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "x"});
            tokens.pushBack({spa::TOKEN_EQUAL, "="});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_INTEGER, "8"});
            tokens.pushBack({spa::TOKEN_PLUS, "+"});
            tokens.pushBack({spa::TOKEN_INTEGER, "5"});
            tokens.pushBack({spa::TOKEN_MULTIPLY, "*"});
            tokens.pushBack({spa::TOKEN_INTEGER, "10"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_SEMICOLON, ";"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});

            expectedTokens.push_back({ spa::TOKEN_PROCEDURE, "procedure" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "z" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "x" });
            expectedTokens.push_back({ spa::TOKEN_EQUAL, "=" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "8" });
            expectedTokens.push_back({ spa::TOKEN_PLUS, "+" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "5" });
            expectedTokens.push_back({ spa::TOKEN_MULTIPLY, "*" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "10" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_SEMICOLON, ";" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            spa::SpValidator validator(tokens);
            Assert::IsTrue(validator.validateGrammar());
            Assert::IsTrue(validator.getUpdatedStream().match(expectedTokens));
        }

        //  procedure z { x = ( (8 % 5) * 10);}
        TEST_METHOD(TestSpValidatorValidExprFour) {
            spa::Stream<spa::Token> tokens;
            std::vector<spa::Token> expectedTokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "x"});
            tokens.pushBack({spa::TOKEN_EQUAL, "="});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_INTEGER, "8"});
            tokens.pushBack({spa::TOKEN_MODULO, "%"});
            tokens.pushBack({spa::TOKEN_INTEGER, "5"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_MULTIPLY, "*"});
            tokens.pushBack({spa::TOKEN_INTEGER, "10"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_SEMICOLON, ";"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});

            expectedTokens.push_back({ spa::TOKEN_PROCEDURE, "procedure" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "z" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "x" });
            expectedTokens.push_back({ spa::TOKEN_EQUAL, "=" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "8" });
            expectedTokens.push_back({ spa::TOKEN_MODULO, "%" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "5" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_MULTIPLY, "*" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "10" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_SEMICOLON, ";" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            spa::SpValidator validator(tokens);
            Assert::IsTrue(validator.validateGrammar());
            Assert::IsTrue(validator.getUpdatedStream().match(expectedTokens));
        }

        //  procedure z { x = 8 % (5 * 10);}
        TEST_METHOD(TestSpValidatorValidExprFive) {
            spa::Stream<spa::Token> tokens;
            std::vector<spa::Token> expectedTokens;
            tokens.pushBack({spa::TOKEN_PROCEDURE, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "x"});
            tokens.pushBack({spa::TOKEN_EQUAL, "="});
            tokens.pushBack({spa::TOKEN_INTEGER, "8"});
            tokens.pushBack({spa::TOKEN_MODULO, "%"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_INTEGER, "5"});
            tokens.pushBack({spa::TOKEN_MULTIPLY, "*"});
            tokens.pushBack({spa::TOKEN_INTEGER, "10"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_SEMICOLON, ";"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});

            expectedTokens.push_back({ spa::TOKEN_PROCEDURE, "procedure" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "z" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "x" });
            expectedTokens.push_back({ spa::TOKEN_EQUAL, "=" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "8" });
            expectedTokens.push_back({ spa::TOKEN_MODULO, "%" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "5" });
            expectedTokens.push_back({ spa::TOKEN_MULTIPLY, "*" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "10" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_SEMICOLON, ";" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            spa::SpValidator validator(tokens);
            Assert::IsTrue(validator.validateGrammar());
            Assert::IsTrue(validator.getUpdatedStream().match(expectedTokens));
        }

        //  procedure z { x = 8 % (5);}
        TEST_METHOD(TestSpValidatorValidExprSix) {
            spa::Stream<spa::Token> tokens;
            std::vector<spa::Token> expectedTokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "x"});
            tokens.pushBack({spa::TOKEN_EQUAL, "="});
            tokens.pushBack({spa::TOKEN_INTEGER, "8"});
            tokens.pushBack({spa::TOKEN_MODULO, "%"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_INTEGER, "5"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_SEMICOLON, ";"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});

            expectedTokens.push_back({ spa::TOKEN_PROCEDURE, "procedure" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "z" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "x" });
            expectedTokens.push_back({ spa::TOKEN_EQUAL, "=" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "8" });
            expectedTokens.push_back({ spa::TOKEN_MODULO, "%" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "5" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_SEMICOLON, ";" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            spa::SpValidator validator(tokens);
            Assert::IsTrue(validator.validateGrammar());
            Assert::IsTrue(validator.getUpdatedStream().match(expectedTokens));
        }

        //  procedure z { x = 8 % ((5));}
        TEST_METHOD(TestSpValidatorValidExprSeven) {
            spa::Stream<spa::Token> tokens;
            std::vector<spa::Token> expectedTokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "x"});
            tokens.pushBack({spa::TOKEN_EQUAL, "="});
            tokens.pushBack({spa::TOKEN_INTEGER, "8"});
            tokens.pushBack({spa::TOKEN_MODULO, "%"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_INTEGER, "5"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_SEMICOLON, ";"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});

            expectedTokens.push_back({ spa::TOKEN_PROCEDURE, "procedure" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "z" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "x" });
            expectedTokens.push_back({ spa::TOKEN_EQUAL, "=" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "8" });
            expectedTokens.push_back({ spa::TOKEN_MODULO, "%" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "5" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_SEMICOLON, ";" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            spa::SpValidator validator(tokens);
            Assert::IsTrue(validator.validateGrammar());
            Assert::IsTrue(validator.getUpdatedStream().match(expectedTokens));
        }


        //  procedure z { x = 8 - (5 / ( 10 + 10))}
        TEST_METHOD(TestSpValidatorValidExprEight) {
            spa::Stream<spa::Token> tokens;
            std::vector<spa::Token> expectedTokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "x"});
            tokens.pushBack({spa::TOKEN_EQUAL, "="});
            tokens.pushBack({spa::TOKEN_INTEGER, "8"});
            tokens.pushBack({spa::TOKEN_MINUS, "-"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_INTEGER, "5"});
            tokens.pushBack({spa::TOKEN_DIVIDE, "/"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_INTEGER, "10"});
            tokens.pushBack({spa::TOKEN_PLUS, "+ "});
            tokens.pushBack({spa::TOKEN_INTEGER, "10"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_SEMICOLON, ";"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});

            expectedTokens.push_back({ spa::TOKEN_PROCEDURE, "procedure" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "z" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "x" });
            expectedTokens.push_back({ spa::TOKEN_EQUAL, "=" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "8" });
            expectedTokens.push_back({ spa::TOKEN_MINUS, "-" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "5" });
            expectedTokens.push_back({ spa::TOKEN_DIVIDE, "/" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "10" });
            expectedTokens.push_back({ spa::TOKEN_PLUS, "+ " });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "10" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_SEMICOLON, ";" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            spa::SpValidator validator(tokens);
            Assert::IsTrue(validator.validateGrammar());
            Assert::IsTrue(validator.getUpdatedStream().match(expectedTokens));
        }

        //  procedure z { x = 8 + (5 / ( 10 + 10))}
        TEST_METHOD(TestSpValidatorValidExprNine) {
            spa::Stream<spa::Token> tokens;
            std::vector<spa::Token> expectedTokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "x"});
            tokens.pushBack({spa::TOKEN_EQUAL, "="});
            tokens.pushBack({spa::TOKEN_INTEGER, "8"});
            tokens.pushBack({spa::TOKEN_PLUS, "+"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_INTEGER, "5"});
            tokens.pushBack({spa::TOKEN_DIVIDE, "/"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_INTEGER, "10"});
            tokens.pushBack({spa::TOKEN_PLUS, "+ "});
            tokens.pushBack({spa::TOKEN_INTEGER, "10"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_SEMICOLON, ";"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});

            expectedTokens.push_back({ spa::TOKEN_PROCEDURE, "procedure" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "z" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "x" });
            expectedTokens.push_back({ spa::TOKEN_EQUAL, "=" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "8" });
            expectedTokens.push_back({ spa::TOKEN_PLUS, "+" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "5" });
            expectedTokens.push_back({ spa::TOKEN_DIVIDE, "/" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "10" });
            expectedTokens.push_back({ spa::TOKEN_PLUS, "+ " });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "10" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_SEMICOLON, ";" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            spa::SpValidator validator(tokens);
            Assert::IsTrue(validator.validateGrammar());
            Assert::IsTrue(validator.getUpdatedStream().match(expectedTokens));
        }

        //  procedure z { x = 8 - ((5) + (10) * (2 % (1 + 2)));}
        TEST_METHOD(TestSpValidatorValidExprTen) {
            spa::Stream<spa::Token> tokens;
            std::vector<spa::Token> expectedTokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "x"});
            tokens.pushBack({spa::TOKEN_EQUAL, "="});
            tokens.pushBack({spa::TOKEN_INTEGER, "8"});
            tokens.pushBack({spa::TOKEN_MINUS, "%"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_INTEGER, "5"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_PLUS, "+"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_INTEGER, "10"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_MULTIPLY, "*"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_INTEGER, "2"});
            tokens.pushBack({spa::TOKEN_MODULO, "%"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_INTEGER, "1"});
            tokens.pushBack({spa::TOKEN_PLUS, "+"});
            tokens.pushBack({spa::TOKEN_INTEGER, "2"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_SEMICOLON, ";"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});

            expectedTokens.push_back({ spa::TOKEN_PROCEDURE, "procedure" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "z" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "x" });
            expectedTokens.push_back({ spa::TOKEN_EQUAL, "=" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "8" });
            expectedTokens.push_back({ spa::TOKEN_MINUS, "%" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "5" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_PLUS, "+" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "10" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_MULTIPLY, "*" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "2" });
            expectedTokens.push_back({ spa::TOKEN_MODULO, "%" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "1" });
            expectedTokens.push_back({ spa::TOKEN_PLUS, "+" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "2" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_SEMICOLON, ";" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            spa::SpValidator validator(tokens);
            Assert::IsTrue(validator.validateGrammar());
            Assert::IsTrue(validator.getUpdatedStream().match(expectedTokens));
        }


        //  procedure z { x = 1 + ((8)) + (4 / (1 + 1 + ((1))
        //  + (1) + (5 / (1 + 2)))) + (1 + ((1)) + (1) +
        //  (10 / (1 + 1 + (((1) * (3 / (1 + 2)))) + (1))));
        TEST_METHOD(TestSpValidatorValidExprEleven) {
            spa::Stream<spa::Token> tokens;
            std::vector<spa::Token> expectedTokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({ spa::TOKEN_NAME, "x" });
            tokens.pushBack({ spa::TOKEN_EQUAL, "=" });
            tokens.pushBack({spa::TOKEN_INTEGER, "1"});
            tokens.pushBack({spa::TOKEN_PLUS, "+"});
            tokens.pushBack({ spa::TOKEN_OPEN_BRACKET, "(" });
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_INTEGER, "8"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({ spa::TOKEN_CLOSE_BRACKET, ")" });
            tokens.pushBack({spa::TOKEN_PLUS, "+"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_INTEGER, "4"});
            tokens.pushBack({spa::TOKEN_DIVIDE, "/"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_INTEGER, "1"});
            tokens.pushBack({spa::TOKEN_PLUS, "+"});
            tokens.pushBack({ spa::TOKEN_INTEGER, "1" });
            tokens.pushBack({ spa::TOKEN_PLUS, "+" });
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_INTEGER, "1"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({ spa::TOKEN_PLUS, "+" });
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_INTEGER, "1"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_PLUS, "+"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_INTEGER, "5"});
            tokens.pushBack({spa::TOKEN_DIVIDE, "/"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_INTEGER, "1"});
            tokens.pushBack({spa::TOKEN_PLUS, "+"});
            tokens.pushBack({spa::TOKEN_INTEGER, "2"});
            tokens.pushBack({ spa::TOKEN_CLOSE_BRACKET, ")" });
            tokens.pushBack({ spa::TOKEN_CLOSE_BRACKET, ")" });
            tokens.pushBack({ spa::TOKEN_CLOSE_BRACKET, ")" });
            tokens.pushBack({ spa::TOKEN_CLOSE_BRACKET, ")" });
            tokens.pushBack({ spa::TOKEN_PLUS, "+" });
            tokens.pushBack({ spa::TOKEN_OPEN_BRACKET, "(" });
            tokens.pushBack({ spa::TOKEN_INTEGER, "1" });
            tokens.pushBack({ spa::TOKEN_PLUS, "+" });
            tokens.pushBack({ spa::TOKEN_OPEN_BRACKET, "(" });
            tokens.pushBack({ spa::TOKEN_OPEN_BRACKET, "(" });
            tokens.pushBack({ spa::TOKEN_INTEGER, "1" });
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_PLUS, "+"});
            tokens.pushBack({ spa::TOKEN_OPEN_BRACKET, "(" });
            tokens.pushBack({ spa::TOKEN_INTEGER, "1" });
            tokens.pushBack({ spa::TOKEN_CLOSE_BRACKET, ")" });
            tokens.pushBack({spa::TOKEN_PLUS, "+"});
            tokens.pushBack({ spa::TOKEN_OPEN_BRACKET, "(" });
            tokens.pushBack({ spa::TOKEN_INTEGER, "10" });
            tokens.pushBack({spa::TOKEN_DIVIDE, "/"});
            tokens.pushBack({ spa::TOKEN_OPEN_BRACKET, "(" });
            tokens.pushBack({spa::TOKEN_INTEGER, "1"});
            tokens.pushBack({spa::TOKEN_PLUS, "+"});
            tokens.pushBack({spa::TOKEN_INTEGER, "1"});
            tokens.pushBack({spa::TOKEN_PLUS, "+"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_INTEGER, "1"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_MULTIPLY, "*"});
            tokens.pushBack({ spa::TOKEN_OPEN_BRACKET, "(" });
            tokens.pushBack({ spa::TOKEN_INTEGER, "3" });
            tokens.pushBack({spa::TOKEN_DIVIDE, "/"});
            tokens.pushBack({ spa::TOKEN_OPEN_BRACKET, "(" });
            tokens.pushBack({ spa::TOKEN_INTEGER, "1" });
            tokens.pushBack({ spa::TOKEN_PLUS, "+" });
            tokens.pushBack({ spa::TOKEN_INTEGER, "2" });
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({ spa::TOKEN_CLOSE_BRACKET, ")" });
            tokens.pushBack({ spa::TOKEN_CLOSE_BRACKET, ")" });
            tokens.pushBack({spa::TOKEN_PLUS, "+"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_INTEGER, "1"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({ spa::TOKEN_CLOSE_BRACKET, ")" });
            tokens.pushBack({spa::TOKEN_SEMICOLON, ";"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});

            expectedTokens.push_back({ spa::TOKEN_PROCEDURE, "procedure" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "z" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "x" });
            expectedTokens.push_back({ spa::TOKEN_EQUAL, "=" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "1" });
            expectedTokens.push_back({ spa::TOKEN_PLUS, "+" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "8" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_PLUS, "+" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "4" });
            expectedTokens.push_back({ spa::TOKEN_DIVIDE, "/" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "1" });
            expectedTokens.push_back({ spa::TOKEN_PLUS, "+" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "1" });
            expectedTokens.push_back({ spa::TOKEN_PLUS, "+" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "1" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_PLUS, "+" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "1" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_PLUS, "+" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "5" });
            expectedTokens.push_back({ spa::TOKEN_DIVIDE, "/" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "1" });
            expectedTokens.push_back({ spa::TOKEN_PLUS, "+" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "2" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_PLUS, "+" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "1" });
            expectedTokens.push_back({ spa::TOKEN_PLUS, "+" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "1" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_PLUS, "+" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "1" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_PLUS, "+" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "10" });
            expectedTokens.push_back({ spa::TOKEN_DIVIDE, "/" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "1" });
            expectedTokens.push_back({ spa::TOKEN_PLUS, "+" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "1" });
            expectedTokens.push_back({ spa::TOKEN_PLUS, "+" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "1" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_MULTIPLY, "*" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "3" });
            expectedTokens.push_back({ spa::TOKEN_DIVIDE, "/" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "1" });
            expectedTokens.push_back({ spa::TOKEN_PLUS, "+" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "2" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_PLUS, "+" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "1" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_SEMICOLON, ";" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            spa::SpValidator validator(tokens);
            Assert::IsTrue(validator.validateGrammar());
            Assert::IsTrue(validator.getUpdatedStream().match(expectedTokens));
        }

        //  procedure z { x = 8 % ((5) / ( 10 + 10))}
        TEST_METHOD(TestSpValidatorValidExprTwelve) {
            spa::Stream<spa::Token> tokens;
            std::vector<spa::Token> expectedTokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "x"});
            tokens.pushBack({spa::TOKEN_EQUAL, "="});
            tokens.pushBack({spa::TOKEN_INTEGER, "8"});
            tokens.pushBack({spa::TOKEN_MODULO, "%"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_INTEGER, "5"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_DIVIDE, "/"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_INTEGER, "10"});
            tokens.pushBack({spa::TOKEN_PLUS, "+ "});
            tokens.pushBack({spa::TOKEN_INTEGER, "10"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_SEMICOLON, ";"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});

            expectedTokens.push_back({ spa::TOKEN_PROCEDURE, "procedure" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "z" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "x" });
            expectedTokens.push_back({ spa::TOKEN_EQUAL, "=" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "8" });
            expectedTokens.push_back({ spa::TOKEN_MODULO, "%" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "5" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_DIVIDE, "/" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "10" });
            expectedTokens.push_back({ spa::TOKEN_PLUS, "+ " });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "10" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_SEMICOLON, ";" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            spa::SpValidator validator(tokens);
            Assert::IsTrue(validator.validateGrammar());
            Assert::IsTrue(validator.getUpdatedStream().match(expectedTokens));
        }

        //  procedure z { x = 8 % ((5)  + (2 / ( 10 + 10)))}
        TEST_METHOD(TestSpValidatorValidExprThirteen) {
            spa::Stream<spa::Token> tokens;
            std::vector<spa::Token> expectedTokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "x"});
            tokens.pushBack({spa::TOKEN_EQUAL, "="});
            tokens.pushBack({spa::TOKEN_INTEGER, "8"});
            tokens.pushBack({spa::TOKEN_MODULO, "%"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_INTEGER, "5"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_PLUS, "+ "});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_INTEGER, "2"});
            tokens.pushBack({spa::TOKEN_DIVIDE, "/"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_INTEGER, "10"});
            tokens.pushBack({spa::TOKEN_PLUS, "+ "});
            tokens.pushBack({spa::TOKEN_INTEGER, "10"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_SEMICOLON, ";"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});

            expectedTokens.push_back({ spa::TOKEN_PROCEDURE, "procedure" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "z" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "x" });
            expectedTokens.push_back({ spa::TOKEN_EQUAL, "=" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "8" });
            expectedTokens.push_back({ spa::TOKEN_MODULO, "%" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "5" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_PLUS, "+ " });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "2" });
            expectedTokens.push_back({ spa::TOKEN_DIVIDE, "/" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "10" });
            expectedTokens.push_back({ spa::TOKEN_PLUS, "+ " });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "10" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_SEMICOLON, ";" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            spa::SpValidator validator(tokens);
            Assert::IsTrue(validator.validateGrammar());
            Assert::IsTrue(validator.getUpdatedStream().match(expectedTokens));
        }

        //  procedure z { x = 8 % ((5)  * (2 / ( 10 + 10)))}
        TEST_METHOD(TestSpValidatorValidExprFourteen) {
            spa::Stream<spa::Token> tokens;
            std::vector<spa::Token> expectedTokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "x"});
            tokens.pushBack({spa::TOKEN_EQUAL, "="});
            tokens.pushBack({spa::TOKEN_INTEGER, "8"});
            tokens.pushBack({spa::TOKEN_MODULO, "%"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_INTEGER, "5"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_MULTIPLY, "*"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_INTEGER, "2"});
            tokens.pushBack({spa::TOKEN_DIVIDE, "/"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_INTEGER, "10"});
            tokens.pushBack({spa::TOKEN_PLUS, "+ "});
            tokens.pushBack({spa::TOKEN_INTEGER, "10"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_SEMICOLON, ";"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});

            expectedTokens.push_back({ spa::TOKEN_PROCEDURE, "procedure" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "z" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "x" });
            expectedTokens.push_back({ spa::TOKEN_EQUAL, "=" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "8" });
            expectedTokens.push_back({ spa::TOKEN_MODULO, "%" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "5" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_MULTIPLY, "*" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "2" });
            expectedTokens.push_back({ spa::TOKEN_DIVIDE, "/" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "10" });
            expectedTokens.push_back({ spa::TOKEN_PLUS, "+ " });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "10" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_SEMICOLON, ";" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            spa::SpValidator validator(tokens);
            Assert::IsTrue(validator.validateGrammar());
            Assert::IsTrue(validator.getUpdatedStream().match(expectedTokens));
        }


        //  procedure z { ) (10 + 1;}
        TEST_METHOD(TestSpValidatorInvalidExprOne) {
            spa::Stream<spa::Token> tokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_INTEGER, "10"});
            tokens.pushBack({spa::TOKEN_PLUS, "+"});
            tokens.pushBack({spa::TOKEN_INTEGER, "1"});
            tokens.pushBack({spa::TOKEN_SEMICOLON, ";"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            spa::SpValidator validator(tokens);
            Assert::IsFalse(validator.validateGrammar());
        }

        //  procedure z { ) 10 + 1 (;}
        TEST_METHOD(TestSpValidatorInvalidExprTwo) {
            spa::Stream<spa::Token> tokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_INTEGER, "10"});
            tokens.pushBack({spa::TOKEN_PLUS, "+"});
            tokens.pushBack({spa::TOKEN_INTEGER, "1"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_SEMICOLON, ";"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            spa::SpValidator validator(tokens);
            Assert::IsFalse(validator.validateGrammar());
        }

        //  procedure z { ) 10 + a (;}
        TEST_METHOD(TestSpValidatorInvalidExprThree) {
            spa::Stream<spa::Token> tokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_INTEGER, "10"});
            tokens.pushBack({spa::TOKEN_PLUS, "+"});
            tokens.pushBack({spa::TOKEN_NAME, "a"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_SEMICOLON, ";"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            spa::SpValidator validator(tokens);
            Assert::IsFalse(validator.validateGrammar());
        }

        //  procedure z { ) 10 + ( 4 ;}
        TEST_METHOD(TestSpValidatorInvalidExprFour) {
            spa::Stream<spa::Token> tokens;
            tokens.pushBack({spa::TOKEN_PROCEDURE, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_INTEGER, "10"});
            tokens.pushBack({spa::TOKEN_PLUS, "+"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_INTEGER, "4"});
            tokens.pushBack({spa::TOKEN_SEMICOLON, ";"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            spa::SpValidator validator(tokens);
            Assert::IsFalse(validator.validateGrammar());
        }

        //  procedure z { ) 10 +  4 ;}
        TEST_METHOD(TestSpValidatorInvalidExprFive) {
            spa::Stream<spa::Token> tokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_INTEGER, "10"});
            tokens.pushBack({spa::TOKEN_PLUS, "+"});
            tokens.pushBack({spa::TOKEN_INTEGER, "4"});
            tokens.pushBack({spa::TOKEN_SEMICOLON, ";"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            spa::SpValidator validator(tokens);
            Assert::IsFalse(validator.validateGrammar());
        }


        //  procedure z {  10 + ( 4 ;}
        TEST_METHOD(TestSpValidatorInvalidExprSix) {
            spa::Stream<spa::Token> tokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_INTEGER, "10"});
            tokens.pushBack({spa::TOKEN_PLUS, "+"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_INTEGER, "4"});
            tokens.pushBack({spa::TOKEN_SEMICOLON, ";"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            spa::SpValidator validator(tokens);
            Assert::IsFalse(validator.validateGrammar());
        }

        //  procedure z { () 10 + 4 ;}
        TEST_METHOD(TestSpValidatorInvalidExprSeven) {
            spa::Stream<spa::Token> tokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_INTEGER, "10"});
            tokens.pushBack({spa::TOKEN_PLUS, "+"});
            tokens.pushBack({spa::TOKEN_INTEGER, "4"});
            tokens.pushBack({spa::TOKEN_SEMICOLON, ";"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            spa::SpValidator validator(tokens);
            Assert::IsFalse(validator.validateGrammar());
        }

        //  procedure z { x = 8 % ((5) / ( 10 + 10));}
        TEST_METHOD(TestSpValidatorInvalidExprEight) {
            spa::Stream<spa::Token> tokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "x"});
            tokens.pushBack({spa::TOKEN_EQUAL, "="});
            tokens.pushBack({spa::TOKEN_INTEGER, "8"});
            tokens.pushBack({spa::TOKEN_MODULO, "%"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_INTEGER, "5"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_MULTIPLY, "/"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_INTEGER, "10"});
            tokens.pushBack({spa::TOKEN_PLUS, "+ "});
            tokens.pushBack({spa::TOKEN_INTEGER, "10"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_SEMICOLON, ";"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            spa::SpValidator validator(tokens);
            Assert::IsFalse(validator.validateGrammar());
        }

        //  while
        //  procedure z { while(y >= 5){X = 1;}}
        TEST_METHOD(TestSpValidatorValidWhileOne) {
            spa::Stream<spa::Token> tokens;
            std::vector<spa::Token> expectedTokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "while"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_NAME, "y"});
            tokens.pushBack({spa::TOKEN_COND_GTE, " >= "});
            tokens.pushBack({spa::TOKEN_INTEGER, "5"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({ spa::TOKEN_NAME, "x" });
            tokens.pushBack({ spa::TOKEN_EQUAL, "=" });
            tokens.pushBack({ spa::TOKEN_INTEGER, "1" });
            tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});

            expectedTokens.push_back({ spa::TOKEN_PROCEDURE, "procedure" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "z" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_WHILE, "while" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "y" });
            expectedTokens.push_back({ spa::TOKEN_COND_GTE, " >= " });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "5" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "x" });
            expectedTokens.push_back({ spa::TOKEN_EQUAL, "=" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "1" });
            expectedTokens.push_back({ spa::TOKEN_SEMICOLON, ";" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            spa::SpValidator validator(tokens);
            Assert::IsTrue(validator.validateGrammar());
            Assert::IsTrue(validator.getUpdatedStream().match(expectedTokens));
        }

        //  procedure z { while(y >= 5{}}
        TEST_METHOD(TestSpValidatorInvalidWhileOne) {
            spa::Stream<spa::Token> tokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "while"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_NAME, "y"});
            tokens.pushBack({spa::TOKEN_COND_GTE, " >= "});
            tokens.pushBack({spa::TOKEN_INTEGER, "5"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            spa::SpValidator validator(tokens);
            Assert::IsFalse(validator.validateGrammar());
        }

        //  procedure z { while(y >= 5{};}
        TEST_METHOD(TestSpValidatorInvalidWhileTwo) {
            spa::Stream<spa::Token> tokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "while"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_NAME, "y"});
            tokens.pushBack({spa::TOKEN_COND_GTE, " >= "});
            tokens.pushBack({spa::TOKEN_INTEGER, "5"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            tokens.pushBack({spa::TOKEN_SEMICOLON, ";"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            spa::SpValidator validator(tokens);
            Assert::IsFalse(validator.validateGrammar());
        }

        //  procedure z { while y >= 5{}}
        TEST_METHOD(TestSpValidatorInvalidWhileThree) {
            spa::Stream<spa::Token> tokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "while"});
            tokens.pushBack({spa::TOKEN_NAME, "y"});
            tokens.pushBack({spa::TOKEN_COND_GTE, " >= "});
            tokens.pushBack({spa::TOKEN_INTEGER, "5"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});

            spa::SpValidator validator(tokens);
            Assert::IsFalse(validator.validateGrammar());
        }

        //  procedure z { while(){}}
        TEST_METHOD(TestSpValidatorInvalidWhileFour) {
            spa::Stream<spa::Token> tokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "while"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            spa::SpValidator validator(tokens);
            Assert::IsFalse(validator.validateGrammar());
        }


        //  if-then-else

        //  procedure z { if(y == 5) then { X = 3; } else { x = 2;}}
        TEST_METHOD(TestSpValidatorValidIfOne) {
            spa::Stream<spa::Token> tokens;
            std::vector<spa::Token> expectedTokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "if"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_NAME, "y"});
            tokens.pushBack({spa::TOKEN_COND_EQ, " == "});
            tokens.pushBack({spa::TOKEN_INTEGER, "5"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_NAME, "then"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "x"});
            tokens.pushBack({spa::TOKEN_EQUAL, " = "});
            tokens.pushBack({spa::TOKEN_INTEGER, "3"});
            tokens.pushBack({spa::TOKEN_SEMICOLON, ";"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            tokens.pushBack({spa::TOKEN_NAME, "else"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "x"});
            tokens.pushBack({spa::TOKEN_EQUAL, " = "});
            tokens.pushBack({spa::TOKEN_INTEGER, "2"});
            tokens.pushBack({spa::TOKEN_SEMICOLON, ";"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});

            expectedTokens.push_back({ spa::TOKEN_PROCEDURE, "procedure" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "z" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_IF, "if" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "y" });
            expectedTokens.push_back({ spa::TOKEN_COND_EQ, " == " });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "5" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_THEN, "then" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "x" });
            expectedTokens.push_back({ spa::TOKEN_EQUAL, " = " });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "3" });
            expectedTokens.push_back({ spa::TOKEN_SEMICOLON, ";" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            expectedTokens.push_back({ spa::TOKEN_ELSE, "else" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "x" });
            expectedTokens.push_back({ spa::TOKEN_EQUAL, " = " });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "2" });
            expectedTokens.push_back({ spa::TOKEN_SEMICOLON, ";" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            spa::SpValidator validator(tokens);
            Assert::IsTrue(validator.validateGrammar());
            Assert::IsTrue(validator.getUpdatedStream().match(expectedTokens));
        }

        //  procedure z { if(y == 5) then { x = 2;} else { z = 2;}}
        TEST_METHOD(TestSpValidatorValidIfTwo) {
            spa::Stream<spa::Token> tokens;
            std::vector<spa::Token> expectedTokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "if"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_NAME, "y"});
            tokens.pushBack({spa::TOKEN_COND_EQ, " == "});
            tokens.pushBack({spa::TOKEN_INTEGER, "5"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_NAME, "then"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({ spa::TOKEN_NAME, "x" });
            tokens.pushBack({ spa::TOKEN_EQUAL, " = " });
            tokens.pushBack({ spa::TOKEN_INTEGER, "2" });
            tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            tokens.pushBack({spa::TOKEN_NAME, "else"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_EQUAL, "="});
            tokens.pushBack({spa::TOKEN_INTEGER, "2"});
            tokens.pushBack({spa::TOKEN_SEMICOLON, ";"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});

            expectedTokens.push_back({ spa::TOKEN_PROCEDURE, "procedure" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "z" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_IF, "if" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "y" });
            expectedTokens.push_back({ spa::TOKEN_COND_EQ, " == " });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "5" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_THEN, "then" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "x" });
            expectedTokens.push_back({ spa::TOKEN_EQUAL, " = " });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "2" });
            expectedTokens.push_back({ spa::TOKEN_SEMICOLON, ";" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            expectedTokens.push_back({ spa::TOKEN_ELSE, "else" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "z" });
            expectedTokens.push_back({ spa::TOKEN_EQUAL, "=" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "2" });
            expectedTokens.push_back({ spa::TOKEN_SEMICOLON, ";" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            spa::SpValidator validator(tokens);
            Assert::IsTrue(validator.validateGrammar());
            Assert::IsTrue(validator.getUpdatedStream().match(expectedTokens));
        }

        //  procedure z { if() then {} else {}}
        TEST_METHOD(TestSpValidatorInvalidIfOne) {
            spa::Stream<spa::Token> tokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "if"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_NAME, "then"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            tokens.pushBack({spa::TOKEN_NAME, "else"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});

            spa::SpValidator validator(tokens);
            Assert::IsFalse(validator.validateGrammar());
        }

        //  procedure z { if() then { x = 2; } else { z = 2; }}
        TEST_METHOD(TestSpValidatorInvalidIfTwo) {
            spa::Stream<spa::Token> tokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "if"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_NAME, "then"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "x"});
            tokens.pushBack({spa::TOKEN_EQUAL, "="});
            tokens.pushBack({spa::TOKEN_INTEGER, "2"});
            tokens.pushBack({spa::TOKEN_SEMICOLON, ";"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            tokens.pushBack({spa::TOKEN_NAME, "else"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_EQUAL, "="});
            tokens.pushBack({spa::TOKEN_INTEGER, "2"});
            tokens.pushBack({spa::TOKEN_SEMICOLON, ";"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});

            spa::SpValidator validator(tokens);
            Assert::IsFalse(validator.validateGrammar());
        }

        //  procedure z { if() then {x = 2;}}
        TEST_METHOD(TestSpValidatorInvalidIfThree) {
            spa::Stream<spa::Token> tokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "if"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_NAME, "then"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "x"});
            tokens.pushBack({spa::TOKEN_EQUAL, "="});
            tokens.pushBack({spa::TOKEN_INTEGER, "2"});
            tokens.pushBack({spa::TOKEN_SEMICOLON, ";"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});

            spa::SpValidator validator(tokens);
            Assert::IsFalse(validator.validateGrammar());
        }

        //  procedure z { if(y == 5) then {z = 2} else {}}
        TEST_METHOD(TestSpValidatorInvalidIfFour) {
            spa::Stream<spa::Token> tokens;
            std::vector<spa::Token> expectedTokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "if"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_NAME, "y"});
            tokens.pushBack({spa::TOKEN_COND_EQ, " == "});
            tokens.pushBack({spa::TOKEN_INTEGER, "5"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_NAME, "then"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_EQUAL, "="});
            tokens.pushBack({spa::TOKEN_INTEGER, "2"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            tokens.pushBack({spa::TOKEN_NAME, "else"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});

            spa::SpValidator validator(tokens);
            Assert::IsFalse(validator.validateGrammar());
        }

        //  procedure z { if(y >= 5) else {} then {} }
        TEST_METHOD(TestSpValidatorInvalidIfFive) {
            spa::Stream<spa::Token> tokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "if"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_NAME, "y"});
            tokens.pushBack({spa::TOKEN_COND_GTE, " >= "});
            tokens.pushBack({spa::TOKEN_INTEGER, "5"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_NAME, "else"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            tokens.pushBack({spa::TOKEN_NAME, "then"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});

            spa::SpValidator validator(tokens);
            Assert::IsFalse(validator.validateGrammar());
        }


        //  procedure z {if () {} else {} then {}}
        TEST_METHOD(TestSpValidatorInvalidIfSix) {
            spa::Stream<spa::Token> tokens;
            tokens.pushBack({ spa::TOKEN_NAME, "procedure" });
            tokens.pushBack({ spa::TOKEN_NAME, "z" });
            tokens.pushBack({ spa::TOKEN_OPEN_BRACE, "{" });
            tokens.pushBack({ spa::TOKEN_NAME, "if" });
            tokens.pushBack({ spa::TOKEN_OPEN_BRACKET, "(" });
            tokens.pushBack({ spa::TOKEN_CLOSE_BRACKET, ")" });
            tokens.pushBack({ spa::TOKEN_OPEN_BRACE, "{" });
            tokens.pushBack({ spa::TOKEN_CLOSE_BRACE, "}" });
            tokens.pushBack({ spa::TOKEN_NAME, "else" });
            tokens.pushBack({ spa::TOKEN_OPEN_BRACE, "{" });
            tokens.pushBack({ spa::TOKEN_CLOSE_BRACE, "}" });
            tokens.pushBack({ spa::TOKEN_NAME, "then" });
            tokens.pushBack({ spa::TOKEN_OPEN_BRACE, "{" });
            tokens.pushBack({ spa::TOKEN_CLOSE_BRACE, "}" });
            tokens.pushBack({ spa::TOKEN_CLOSE_BRACE, "}" });
            spa::SpValidator validator(tokens);
            Assert::IsFalse(validator.validateGrammar());
            }

        //  procedure z {if () {} else {} }
        TEST_METHOD(TestSpValidatorInvalidIfSeven) {
            spa::Stream<spa::Token> tokens;
            tokens.pushBack({ spa::TOKEN_NAME, "procedure" });
            tokens.pushBack({ spa::TOKEN_NAME, "z" });
            tokens.pushBack({ spa::TOKEN_OPEN_BRACE, "{" });
            tokens.pushBack({ spa::TOKEN_NAME, "if" });
            tokens.pushBack({ spa::TOKEN_OPEN_BRACKET, "(" });
            tokens.pushBack({ spa::TOKEN_CLOSE_BRACKET, ")" });
            tokens.pushBack({ spa::TOKEN_OPEN_BRACE, "{" });
            tokens.pushBack({ spa::TOKEN_CLOSE_BRACE, "}" });
            tokens.pushBack({ spa::TOKEN_NAME, "else" });
            tokens.pushBack({ spa::TOKEN_OPEN_BRACE, "{" });
            tokens.pushBack({ spa::TOKEN_CLOSE_BRACE, "}" });
            tokens.pushBack({ spa::TOKEN_CLOSE_BRACE, "}" });
            spa::SpValidator validator(tokens);
            Assert::IsFalse(validator.validateGrammar());
        }

        //  procedure z { if(y == 5) then {z = 2;} else {}}
        TEST_METHOD(TestSpValidatorInvalidIfEight) {
            spa::Stream<spa::Token> tokens;
            tokens.pushBack({ spa::TOKEN_PROCEDURE, "procedure" });
            tokens.pushBack({ spa::TOKEN_NAME, "z" });
            tokens.pushBack({ spa::TOKEN_OPEN_BRACE, "{" });
            tokens.pushBack({ spa::TOKEN_IF, "if" });
            tokens.pushBack({ spa::TOKEN_OPEN_BRACKET, "(" });
            tokens.pushBack({ spa::TOKEN_NAME, "y" });
            tokens.pushBack({ spa::TOKEN_COND_EQ, " == " });
            tokens.pushBack({ spa::TOKEN_INTEGER, "5" });
            tokens.pushBack({ spa::TOKEN_CLOSE_BRACKET, ")" });
            tokens.pushBack({ spa::TOKEN_THEN, "then" });
            tokens.pushBack({ spa::TOKEN_OPEN_BRACE, "{" });
            tokens.pushBack({ spa::TOKEN_NAME, "z" });
            tokens.pushBack({ spa::TOKEN_EQUAL, "=" });
            tokens.pushBack({ spa::TOKEN_INTEGER, "2" });
            tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
            tokens.pushBack({ spa::TOKEN_CLOSE_BRACE, "}" });
            tokens.pushBack({ spa::TOKEN_ELSE, "else" });
            tokens.pushBack({ spa::TOKEN_OPEN_BRACE, "{" });
            tokens.pushBack({ spa::TOKEN_CLOSE_BRACE, "}" });
            tokens.pushBack({ spa::TOKEN_CLOSE_BRACE, "}" });
            spa::SpValidator validator(tokens);
            Assert::IsFalse(validator.validateGrammar());
        }

        //  procedure z { if(y == 5) then {} else {z = 2;}}
        TEST_METHOD(TestSpValidatorInvalidIfNine) {
            spa::Stream<spa::Token> tokens;
            tokens.pushBack({ spa::TOKEN_NAME, "procedure" });
            tokens.pushBack({ spa::TOKEN_NAME, "z" });
            tokens.pushBack({ spa::TOKEN_OPEN_BRACE, "{" });
            tokens.pushBack({ spa::TOKEN_NAME, "if" });
            tokens.pushBack({ spa::TOKEN_OPEN_BRACKET, "(" });
            tokens.pushBack({ spa::TOKEN_NAME, "y" });
            tokens.pushBack({ spa::TOKEN_COND_EQ, " == " });
            tokens.pushBack({ spa::TOKEN_INTEGER, "5" });
            tokens.pushBack({ spa::TOKEN_CLOSE_BRACKET, ")" });
            tokens.pushBack({ spa::TOKEN_NAME, "then" });
            tokens.pushBack({ spa::TOKEN_OPEN_BRACE, "{" });
            tokens.pushBack({ spa::TOKEN_CLOSE_BRACE, "}" });
            tokens.pushBack({ spa::TOKEN_NAME, "else" });
            tokens.pushBack({ spa::TOKEN_OPEN_BRACE, "{" });
            tokens.pushBack({ spa::TOKEN_NAME, "z" });
            tokens.pushBack({ spa::TOKEN_EQUAL, "=" });
            tokens.pushBack({ spa::TOKEN_INTEGER, "2" });
            tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
            tokens.pushBack({ spa::TOKEN_CLOSE_BRACE, "}" });
            tokens.pushBack({ spa::TOKEN_CLOSE_BRACE, "}" });

            spa::SpValidator validator(tokens);
            Assert::IsFalse(validator.validateGrammar());
        }

        //  procedure z { if(y == 5) then {} else {}}
        TEST_METHOD(TestSpValidatorInvalidIfThen) {
            spa::Stream<spa::Token> tokens;
            tokens.pushBack({ spa::TOKEN_NAME, "procedure" });
            tokens.pushBack({ spa::TOKEN_NAME, "z" });
            tokens.pushBack({ spa::TOKEN_OPEN_BRACE, "{" });
            tokens.pushBack({ spa::TOKEN_NAME, "if" });
            tokens.pushBack({ spa::TOKEN_OPEN_BRACKET, "(" });
            tokens.pushBack({ spa::TOKEN_NAME, "y" });
            tokens.pushBack({ spa::TOKEN_COND_EQ, " == " });
            tokens.pushBack({ spa::TOKEN_INTEGER, "5" });
            tokens.pushBack({ spa::TOKEN_CLOSE_BRACKET, ")" });
            tokens.pushBack({ spa::TOKEN_NAME, "then" });
            tokens.pushBack({ spa::TOKEN_OPEN_BRACE, "{" });
            tokens.pushBack({ spa::TOKEN_CLOSE_BRACE, "}" });
            tokens.pushBack({ spa::TOKEN_NAME, "else" });
            tokens.pushBack({ spa::TOKEN_OPEN_BRACE, "{" });
            tokens.pushBack({ spa::TOKEN_CLOSE_BRACE, "}" });
            tokens.pushBack({ spa::TOKEN_CLOSE_BRACE, "}" });
            spa::SpValidator validator(tokens);
            Assert::IsFalse(validator.validateGrammar());
        }


        //  nested while-if
        //  procedure z { while( y == 10) { q = 5; if (z > 2) then { x = 10;} else{ x = 3;}}}
        TEST_METHOD(TestSpValidatorNestedWhileIfOne) {
            spa::Stream<spa::Token> tokens;
            std::vector<spa::Token> expectedTokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "while"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_NAME, "y"});
            tokens.pushBack({spa::TOKEN_COND_EQ, " == "});
            tokens.pushBack({spa::TOKEN_INTEGER, "10"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "q"});
            tokens.pushBack({spa::TOKEN_EQUAL, "="});
            tokens.pushBack({spa::TOKEN_INTEGER, "5"});
            tokens.pushBack({spa::TOKEN_SEMICOLON, ";"});
            tokens.pushBack({spa::TOKEN_NAME, "if"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_COND_GT, ">"});
            tokens.pushBack({spa::TOKEN_INTEGER, "2"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_NAME, "then"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "x"});
            tokens.pushBack({spa::TOKEN_EQUAL, "="});
            tokens.pushBack({spa::TOKEN_INTEGER, "10"});
            tokens.pushBack({spa::TOKEN_SEMICOLON, ";"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            tokens.pushBack({spa::TOKEN_NAME, "else"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "x"});
            tokens.pushBack({spa::TOKEN_EQUAL, "="});
            tokens.pushBack({spa::TOKEN_INTEGER, "3"});
            tokens.pushBack({spa::TOKEN_SEMICOLON, ";"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});

            expectedTokens.push_back({ spa::TOKEN_PROCEDURE, "procedure" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "z" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_WHILE, "while" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "y" });
            expectedTokens.push_back({ spa::TOKEN_COND_EQ, " == " });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "10" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "q" });
            expectedTokens.push_back({ spa::TOKEN_EQUAL, "=" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "5" });
            expectedTokens.push_back({ spa::TOKEN_SEMICOLON, ";" });
            expectedTokens.push_back({ spa::TOKEN_IF, "if" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "z" });
            expectedTokens.push_back({ spa::TOKEN_COND_GT, ">" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "2" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_THEN, "then" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "x" });
            expectedTokens.push_back({ spa::TOKEN_EQUAL, "=" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "10" });
            expectedTokens.push_back({ spa::TOKEN_SEMICOLON, ";" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            expectedTokens.push_back({ spa::TOKEN_ELSE, "else" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "x" });
            expectedTokens.push_back({ spa::TOKEN_EQUAL, "=" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "3" });
            expectedTokens.push_back({ spa::TOKEN_SEMICOLON, ";" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            spa::SpValidator validator(tokens);
            Assert::IsTrue(validator.validateGrammar());
            Assert::IsTrue(validator.getUpdatedStream().match(expectedTokens));
        }

        //  Test rel expr
        //  procedure z { while( "rel_expr" ) {"stmtLst"} }

        //  procedure z { while( x < 5 ) {X = 3;}}
        TEST_METHOD(TestSpValidatorRelExprOne) {
            spa::Stream<spa::Token> tokens;
            std::vector<spa::Token> expectedTokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "while"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_NAME, "x"});
            tokens.pushBack({spa::TOKEN_COND_LT, " < "});
            tokens.pushBack({spa::TOKEN_INTEGER, "5"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({ spa::TOKEN_NAME, "x" });
            tokens.pushBack({ spa::TOKEN_EQUAL, "=" });
            tokens.pushBack({ spa::TOKEN_INTEGER, "3" });
            tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});

            expectedTokens.push_back({ spa::TOKEN_PROCEDURE, "procedure" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "z" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_WHILE, "while" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "x" });
            expectedTokens.push_back({ spa::TOKEN_COND_LT, " < " });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "5" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "x" });
            expectedTokens.push_back({ spa::TOKEN_EQUAL, "=" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "3" });
            expectedTokens.push_back({ spa::TOKEN_SEMICOLON, ";" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            spa::SpValidator validator(tokens);
            Assert::IsTrue(validator.validateGrammar());
            Assert::IsTrue(validator.getUpdatedStream().match(expectedTokens));
        }

        //  procedure z { while( x <= 5 ) {X = 3;}}
        TEST_METHOD(TestSpValidatorRelExprTwo) {
            spa::Stream<spa::Token> tokens;
            std::vector<spa::Token> expectedTokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "while"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_NAME, "x"});
            tokens.pushBack({spa::TOKEN_COND_LTE, " <= "});
            tokens.pushBack({spa::TOKEN_INTEGER, "5"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({ spa::TOKEN_NAME, "x" });
            tokens.pushBack({ spa::TOKEN_EQUAL, "=" });
            tokens.pushBack({ spa::TOKEN_INTEGER, "3" });
            tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});

            expectedTokens.push_back({ spa::TOKEN_PROCEDURE, "procedure" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "z" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_WHILE, "while" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "x" });
            expectedTokens.push_back({ spa::TOKEN_COND_LTE, " <= " });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "5" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "x" });
            expectedTokens.push_back({ spa::TOKEN_EQUAL, "=" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "3" });
            expectedTokens.push_back({ spa::TOKEN_SEMICOLON, ";" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            spa::SpValidator validator(tokens);
            Assert::IsTrue(validator.validateGrammar());
            Assert::IsTrue(validator.getUpdatedStream().match(expectedTokens));
        }

        //  procedure z { while( x <= ( y * 5 + b * c )) {X = 3;}}
        TEST_METHOD(TestSpValidatorRelExprThree) {
            spa::Stream<spa::Token> tokens;
            std::vector<spa::Token> expectedTokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "while"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_NAME, "x"});
            tokens.pushBack({spa::TOKEN_COND_LTE, " <= "});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_NAME, "y"});
            tokens.pushBack({spa::TOKEN_MULTIPLY, "*"});
            tokens.pushBack({spa::TOKEN_INTEGER, "5"});
            tokens.pushBack({spa::TOKEN_PLUS, "+"});
            tokens.pushBack({spa::TOKEN_NAME, "b"});
            tokens.pushBack({spa::TOKEN_MULTIPLY, "*"});
            tokens.pushBack({spa::TOKEN_NAME, "c"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({ spa::TOKEN_NAME, "x" });
            tokens.pushBack({ spa::TOKEN_EQUAL, "=" });
            tokens.pushBack({ spa::TOKEN_INTEGER, "3" });
            tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});

            expectedTokens.push_back({ spa::TOKEN_PROCEDURE, "procedure" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "z" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_WHILE, "while" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "x" });
            expectedTokens.push_back({ spa::TOKEN_COND_LTE, " <= " });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "y" });
            expectedTokens.push_back({ spa::TOKEN_MULTIPLY, "*" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "5" });
            expectedTokens.push_back({ spa::TOKEN_PLUS, "+" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "b" });
            expectedTokens.push_back({ spa::TOKEN_MULTIPLY, "*" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "c" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "x" });
            expectedTokens.push_back({ spa::TOKEN_EQUAL, "=" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "3" });
            expectedTokens.push_back({ spa::TOKEN_SEMICOLON, ";" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            spa::SpValidator validator(tokens);
            Assert::IsTrue(validator.validateGrammar());
            Assert::IsTrue(validator.getUpdatedStream().match(expectedTokens));
        }

        //  procedure z { while( 2 == 2) {X = 3;}}
        TEST_METHOD(TestSpValidatorRelExprFour) {
            spa::Stream<spa::Token> tokens;
            std::vector<spa::Token> expectedTokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "while"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_INTEGER, "2"});
            tokens.pushBack({spa::TOKEN_COND_EQ, " == "});
            tokens.pushBack({spa::TOKEN_INTEGER, "2"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({ spa::TOKEN_NAME, "x" });
            tokens.pushBack({ spa::TOKEN_EQUAL, "=" });
            tokens.pushBack({ spa::TOKEN_INTEGER, "3" });
            tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});

            expectedTokens.push_back({ spa::TOKEN_PROCEDURE, "procedure" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "z" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_WHILE, "while" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "2" });
            expectedTokens.push_back({ spa::TOKEN_COND_EQ, " == " });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "2" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "x" });
            expectedTokens.push_back({ spa::TOKEN_EQUAL, "=" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "3" });
            expectedTokens.push_back({ spa::TOKEN_SEMICOLON, ";" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            spa::SpValidator validator(tokens);
            Assert::IsTrue(validator.validateGrammar());
            Assert::IsTrue(validator.getUpdatedStream().match(expectedTokens));
        }

        //  procedure z { while( (x+2) > 2) {X = 3;}}
        TEST_METHOD(TestSpValidatorRelExprFive) {
            spa::Stream<spa::Token> tokens;
            std::vector<spa::Token> expectedTokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "while"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_NAME, "x"});
            tokens.pushBack({spa::TOKEN_PLUS, "+"});
            tokens.pushBack({spa::TOKEN_INTEGER, "2"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_COND_GT, " > "});
            tokens.pushBack({spa::TOKEN_INTEGER, "2"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({ spa::TOKEN_NAME, "x" });
            tokens.pushBack({ spa::TOKEN_EQUAL, "=" });
            tokens.pushBack({ spa::TOKEN_INTEGER, "3" });
            tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});

            expectedTokens.push_back({ spa::TOKEN_PROCEDURE, "procedure" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "z" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_WHILE, "while" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "x" });
            expectedTokens.push_back({ spa::TOKEN_PLUS, "+" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "2" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_COND_GT, " > " });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "2" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "x" });
            expectedTokens.push_back({ spa::TOKEN_EQUAL, "=" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "3" });
            expectedTokens.push_back({ spa::TOKEN_SEMICOLON, ";" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            spa::SpValidator validator(tokens);
            Assert::IsTrue(validator.validateGrammar());
            Assert::IsTrue(validator.getUpdatedStream().match(expectedTokens));
        }

        //  procedure z { while (x == 2) { v = 1;}}
        TEST_METHOD(TestSpValidatorRelExprSix) {
            spa::Stream<spa::Token> tokens;
            std::vector<spa::Token> expectedTokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "while"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_NAME, "x"});
            tokens.pushBack({spa::TOKEN_COND_EQ, "=="});
            tokens.pushBack({spa::TOKEN_INTEGER, "2"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({ spa::TOKEN_NAME, "v" });
            tokens.pushBack({ spa::TOKEN_EQUAL, "=" });
            tokens.pushBack({ spa::TOKEN_INTEGER, "1" });
            tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});

            expectedTokens.push_back({ spa::TOKEN_PROCEDURE, "procedure" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "z" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_WHILE, "while" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "x" });
            expectedTokens.push_back({ spa::TOKEN_COND_EQ, "==" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "2" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "v" });
            expectedTokens.push_back({ spa::TOKEN_EQUAL, "=" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "1" });
            expectedTokens.push_back({ spa::TOKEN_SEMICOLON, ";" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            spa::SpValidator validator(tokens);
            Assert::IsTrue(validator.validateGrammar());
            Assert::IsTrue(validator.getUpdatedStream().match(expectedTokens));
        }

        //  procedure z { while( x > 5 ) {X = ;}}
        TEST_METHOD(TestSpValidatorInvalidRelExprOne) {
            spa::Stream<spa::Token> tokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "while"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_NAME, "x"});
            tokens.pushBack({spa::TOKEN_COND_GT, " > "});
            tokens.pushBack({spa::TOKEN_INTEGER, "5"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({ spa::TOKEN_NAME, "x" });
            tokens.pushBack({ spa::TOKEN_EQUAL, "=" });
            tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});

            spa::SpValidator validator(tokens);
            Assert::IsFalse(validator.validateGrammar());
        }

        //  procedure z { while( x >= 5 ) {}}
        TEST_METHOD(TestSpValidatorInvalidRelExprTwo) {
            spa::Stream<spa::Token> tokens;
            tokens.pushBack({ spa::TOKEN_NAME, "procedure" });
            tokens.pushBack({ spa::TOKEN_NAME, "z" });
            tokens.pushBack({ spa::TOKEN_OPEN_BRACE, "{" });
            tokens.pushBack({ spa::TOKEN_NAME, "while" });
            tokens.pushBack({ spa::TOKEN_OPEN_BRACKET, "(" });
            tokens.pushBack({ spa::TOKEN_NAME, "x" });
            tokens.pushBack({ spa::TOKEN_COND_GTE, " >= " });
            tokens.pushBack({ spa::TOKEN_INTEGER, "5" });
            tokens.pushBack({ spa::TOKEN_CLOSE_BRACKET, ")" });
            tokens.pushBack({ spa::TOKEN_OPEN_BRACE, "{" });
            tokens.pushBack({ spa::TOKEN_CLOSE_BRACE, "}" });
            tokens.pushBack({ spa::TOKEN_CLOSE_BRACE, "}" });

            spa::SpValidator validator(tokens);
            Assert::IsFalse(validator.validateGrammar());
        }

        //  procedure z { while ( 5 > = 12 % 6 ){ x = 3; }}
        TEST_METHOD(TestSpValidatorCondExprOne) {
            spa::Stream<spa::Token> tokens;
            std::vector<spa::Token> expectedTokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "while"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_INTEGER, "5"});
            tokens.pushBack({spa::TOKEN_COND_GTE, " >= "});
            tokens.pushBack({spa::TOKEN_INTEGER, "12"});
            tokens.pushBack({spa::TOKEN_MODULO, "%"});
            tokens.pushBack({spa::TOKEN_INTEGER, "6"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({ spa::TOKEN_NAME, "x" });
            tokens.pushBack({ spa::TOKEN_EQUAL, "=" });
            tokens.pushBack({ spa::TOKEN_INTEGER, "3" });
            tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});

            expectedTokens.push_back({ spa::TOKEN_PROCEDURE, "procedure" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "z" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_WHILE, "while" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "5" });
            expectedTokens.push_back({ spa::TOKEN_COND_GTE, " >= " });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "12" });
            expectedTokens.push_back({ spa::TOKEN_MODULO, "%" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "6" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "x" });
            expectedTokens.push_back({ spa::TOKEN_EQUAL, "=" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "3" });
            expectedTokens.push_back({ spa::TOKEN_SEMICOLON, ";" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            spa::SpValidator validator(tokens);
            Assert::IsTrue(validator.validateGrammar());
            Assert::IsTrue(validator.getUpdatedStream().match(expectedTokens));
        }


        //  procedure z { while ( ! ( 5 >= 12 )){ x = 3; }}
        TEST_METHOD(TestSpValidatorCondExprTwo) {
            spa::Stream<spa::Token> tokens;
            std::vector<spa::Token> expectedTokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "while"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_BOOL_NOT, "!"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_INTEGER, "5"});
            tokens.pushBack({spa::TOKEN_COND_GTE, " >= "});
            tokens.pushBack({spa::TOKEN_INTEGER, "12"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({ spa::TOKEN_NAME, "x" });
            tokens.pushBack({ spa::TOKEN_EQUAL, "=" });
            tokens.pushBack({ spa::TOKEN_INTEGER, "3" });
            tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});

            expectedTokens.push_back({ spa::TOKEN_PROCEDURE, "procedure" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "z" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_WHILE, "while" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_BOOL_NOT, "!" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "5" });
            expectedTokens.push_back({ spa::TOKEN_COND_GTE, " >= " });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "12" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "x" });
            expectedTokens.push_back({ spa::TOKEN_EQUAL, "=" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "3" });
            expectedTokens.push_back({ spa::TOKEN_SEMICOLON, ";" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            spa::SpValidator validator(tokens);
            Assert::IsTrue(validator.validateGrammar());
            Assert::IsTrue(validator.getUpdatedStream().match(expectedTokens));
        }

        //  procedure z { while ( ! ( ! (5 >= 12 ))){ x = 3; }}
        TEST_METHOD(TestSpValidatorCondExprThree) {
            spa::Stream<spa::Token> tokens;
            std::vector<spa::Token> expectedTokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "while"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_BOOL_NOT, "!"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_BOOL_NOT, "!"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_NAME, "5"});
            tokens.pushBack({spa::TOKEN_COND_GTE, " >= "});
            tokens.pushBack({spa::TOKEN_INTEGER, "12"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({ spa::TOKEN_NAME, "x" });
            tokens.pushBack({ spa::TOKEN_EQUAL, "=" });
            tokens.pushBack({ spa::TOKEN_INTEGER, "3" });
            tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});

            expectedTokens.push_back({ spa::TOKEN_PROCEDURE, "procedure" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "z" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_WHILE, "while" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_BOOL_NOT, "!" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_BOOL_NOT, "!" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "5" });
            expectedTokens.push_back({ spa::TOKEN_COND_GTE, " >= " });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "12" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "x" });
            expectedTokens.push_back({ spa::TOKEN_EQUAL, "=" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "3" });
            expectedTokens.push_back({ spa::TOKEN_SEMICOLON, ";" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            spa::SpValidator validator(tokens);
            Assert::IsTrue(validator.validateGrammar());
            Assert::IsTrue(validator.getUpdatedStream().match(expectedTokens));
        }


        //  procedure z { while ( (! (x >= 12 )) && ( 12 == 12 )){ x = 3; }}
        TEST_METHOD(TestSpValidatorCondExprFour) {
            spa::Stream<spa::Token> tokens;
            std::vector<spa::Token> expectedTokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "while"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_BOOL_NOT, "!"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_NAME, "x"});
            tokens.pushBack({spa::TOKEN_COND_GTE, " >= "});
            tokens.pushBack({spa::TOKEN_INTEGER, "12"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_BOOL_AND, "&&"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_INTEGER, "12"});
            tokens.pushBack({spa::TOKEN_COND_EQ, " == "});
            tokens.pushBack({spa::TOKEN_INTEGER, "12"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({ spa::TOKEN_NAME, "x" });
            tokens.pushBack({ spa::TOKEN_EQUAL, "=" });
            tokens.pushBack({ spa::TOKEN_INTEGER, "3" });
            tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});

            expectedTokens.push_back({ spa::TOKEN_PROCEDURE, "procedure" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "z" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_WHILE, "while" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_BOOL_NOT, "!" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "x" });
            expectedTokens.push_back({ spa::TOKEN_COND_GTE, " >= " });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "12" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_BOOL_AND, "&&" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "12" });
            expectedTokens.push_back({ spa::TOKEN_COND_EQ, " == " });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "12" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "x" });
            expectedTokens.push_back({ spa::TOKEN_EQUAL, "=" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "3" });
            expectedTokens.push_back({ spa::TOKEN_SEMICOLON, ";" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            spa::SpValidator validator(tokens);
            Assert::IsTrue(validator.validateGrammar());
            Assert::IsTrue(validator.getUpdatedStream().match(expectedTokens));
        }

        //  procedure z { while ( (x >= 12 ) && ( 12 == 12 )){ x = 3; }}
        TEST_METHOD(TestSpValidatorCondExprFive) {
            spa::Stream<spa::Token> tokens;
            std::vector<spa::Token> expectedTokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "while"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_NAME, "x"});
            tokens.pushBack({spa::TOKEN_COND_GTE, " >= "});
            tokens.pushBack({spa::TOKEN_INTEGER, "12"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_BOOL_AND, "&&"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_INTEGER, "12"});
            tokens.pushBack({spa::TOKEN_COND_EQ, " == "});
            tokens.pushBack({spa::TOKEN_INTEGER, "12"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({ spa::TOKEN_NAME, "x" });
            tokens.pushBack({ spa::TOKEN_EQUAL, "=" });
            tokens.pushBack({ spa::TOKEN_INTEGER, "3" });
            tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});

            expectedTokens.push_back({ spa::TOKEN_PROCEDURE, "procedure" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "z" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_WHILE, "while" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "x" });
            expectedTokens.push_back({ spa::TOKEN_COND_GTE, " >= " });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "12" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_BOOL_AND, "&&" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "12" });
            expectedTokens.push_back({ spa::TOKEN_COND_EQ, " == " });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "12" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "x" });
            expectedTokens.push_back({ spa::TOKEN_EQUAL, "=" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "3" });
            expectedTokens.push_back({ spa::TOKEN_SEMICOLON, ";" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            spa::SpValidator validator(tokens);
            Assert::IsTrue(validator.validateGrammar());
            Assert::IsTrue(validator.getUpdatedStream().match(expectedTokens));
        }

        //  procedure z { while ( (x == 12 ) && ( 12 == 12 )){ x = 3;}}
        TEST_METHOD(TestSpValidatorCondExprSix) {
            spa::Stream<spa::Token> tokens;
            std::vector<spa::Token> expectedTokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "while"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_NAME, "x"});
            tokens.pushBack({spa::TOKEN_COND_EQ, " == "});
            tokens.pushBack({spa::TOKEN_INTEGER, "12"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_BOOL_AND, "&&"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_INTEGER, "12"});
            tokens.pushBack({spa::TOKEN_COND_EQ, " == "});
            tokens.pushBack({spa::TOKEN_INTEGER, "12"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({ spa::TOKEN_NAME, "x" });
            tokens.pushBack({ spa::TOKEN_EQUAL, "=" });
            tokens.pushBack({ spa::TOKEN_INTEGER, "3" });
            tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});

            expectedTokens.push_back({ spa::TOKEN_PROCEDURE, "procedure" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "z" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_WHILE, "while" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "x" });
            expectedTokens.push_back({ spa::TOKEN_COND_EQ, " == " });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "12" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_BOOL_AND, "&&" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "12" });
            expectedTokens.push_back({ spa::TOKEN_COND_EQ, " == " });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "12" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "x" });
            expectedTokens.push_back({ spa::TOKEN_EQUAL, "=" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "3" });
            expectedTokens.push_back({ spa::TOKEN_SEMICOLON, ";" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            spa::SpValidator validator(tokens);
            Assert::IsTrue(validator.validateGrammar());
            Assert::IsTrue(validator.getUpdatedStream().match(expectedTokens));
        }


        //  procedure z { while ( (x == 12 ) || ( 12 == 12 )){ x = 3; }}
        TEST_METHOD(TestSpValidatorCondExprSeven) {
            spa::Stream<spa::Token> tokens;
            std::vector<spa::Token> expectedTokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "while"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_NAME, "x"});
            tokens.pushBack({spa::TOKEN_COND_EQ, " == "});
            tokens.pushBack({spa::TOKEN_INTEGER, "12"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_BOOL_OR, "||"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_INTEGER, "12"});
            tokens.pushBack({spa::TOKEN_COND_EQ, " == "});
            tokens.pushBack({spa::TOKEN_INTEGER, "12"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({ spa::TOKEN_NAME, "x" });
            tokens.pushBack({ spa::TOKEN_EQUAL, "=" });
            tokens.pushBack({ spa::TOKEN_INTEGER, "3" });
            tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});

            expectedTokens.push_back({ spa::TOKEN_PROCEDURE, "procedure" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "z" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_WHILE, "while" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "x" });
            expectedTokens.push_back({ spa::TOKEN_COND_EQ, " == " });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "12" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_BOOL_OR, "||" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "12" });
            expectedTokens.push_back({ spa::TOKEN_COND_EQ, " == " });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "12" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "x" });
            expectedTokens.push_back({ spa::TOKEN_EQUAL, "=" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "3" });
            expectedTokens.push_back({ spa::TOKEN_SEMICOLON, ";" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            spa::SpValidator validator(tokens);
            Assert::IsTrue(validator.validateGrammar());
            Assert::IsTrue(validator.getUpdatedStream().match(expectedTokens));
        }


        //  procedure z { while ((x == 12 ) || (!( y == 12 ))){ x = 3; }}
        TEST_METHOD(TestSpValidatorCondExpEight) {
            spa::Stream<spa::Token> tokens;
            std::vector<spa::Token> expectedTokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "while"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_NAME, "x"});
            tokens.pushBack({spa::TOKEN_COND_EQ, " == "});
            tokens.pushBack({spa::TOKEN_INTEGER, "12"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_BOOL_OR, "||"});
            tokens.pushBack({ spa::TOKEN_OPEN_BRACKET, "(" });
            tokens.pushBack({spa::TOKEN_BOOL_NOT, "!"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_NAME, "y"});
            tokens.pushBack({spa::TOKEN_COND_EQ, " == "});
            tokens.pushBack({spa::TOKEN_INTEGER, "12"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({ spa::TOKEN_CLOSE_BRACKET, ")" });
            tokens.pushBack({ spa::TOKEN_CLOSE_BRACKET, ")" });
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({ spa::TOKEN_NAME, "x" });
            tokens.pushBack({ spa::TOKEN_EQUAL, "=" });
            tokens.pushBack({ spa::TOKEN_INTEGER, "3" });
            tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});

            expectedTokens.push_back({ spa::TOKEN_PROCEDURE, "procedure" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "z" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_WHILE, "while" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "x" });
            expectedTokens.push_back({ spa::TOKEN_COND_EQ, " == " });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "12" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_BOOL_OR, "||" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_BOOL_NOT, "!" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "y" });
            expectedTokens.push_back({ spa::TOKEN_COND_EQ, " == " });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "12" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "x" });
            expectedTokens.push_back({ spa::TOKEN_EQUAL, "=" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "3" });
            expectedTokens.push_back({ spa::TOKEN_SEMICOLON, ";" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            spa::SpValidator validator(tokens);
            Assert::IsTrue(validator.validateGrammar());
            Assert::IsTrue(validator.getUpdatedStream().match(expectedTokens));
        }

        //  procedure z { while ((x == 12 ) || ( y != 12 )){ x = 3; }}
        TEST_METHOD(TestSpValidatorCondExpNine) {
            spa::Stream<spa::Token> tokens;
            std::vector<spa::Token> expectedTokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "while"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_NAME, "x"});
            tokens.pushBack({spa::TOKEN_COND_EQ, " == "});
            tokens.pushBack({spa::TOKEN_INTEGER, "12"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_BOOL_OR, "||"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_NAME, "y"});
            tokens.pushBack({spa::TOKEN_COND_NEQ, " != "});
            tokens.pushBack({spa::TOKEN_INTEGER, "12"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({ spa::TOKEN_CLOSE_BRACKET, ")" });
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({ spa::TOKEN_NAME, "x" });
            tokens.pushBack({ spa::TOKEN_EQUAL, "=" });
            tokens.pushBack({ spa::TOKEN_INTEGER, "3" });
            tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});

            expectedTokens.push_back({ spa::TOKEN_PROCEDURE, "procedure" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "z" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_WHILE, "while" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "x" });
            expectedTokens.push_back({ spa::TOKEN_COND_EQ, " == " });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "12" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_BOOL_OR, "||" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "y" });
            expectedTokens.push_back({ spa::TOKEN_COND_NEQ, " != " });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "12" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "x" });
            expectedTokens.push_back({ spa::TOKEN_EQUAL, "=" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "3" });
            expectedTokens.push_back({ spa::TOKEN_SEMICOLON, ";" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            spa::SpValidator validator(tokens);
            Assert::IsTrue(validator.validateGrammar());
            Assert::IsTrue(validator.getUpdatedStream().match(expectedTokens));
        }

        //  procedure z { while (12 ==  (3 % ( 20 - 12 ))){ x = 3;}}
        TEST_METHOD(TestSpValidatorCondExptTen) {
            spa::Stream<spa::Token> tokens;
            std::vector<spa::Token> expectedTokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "while"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_INTEGER, "12"});
            tokens.pushBack({spa::TOKEN_COND_EQ, " == "});
            tokens.pushBack({ spa::TOKEN_OPEN_BRACKET, "(" });
            tokens.pushBack({spa::TOKEN_INTEGER, "3"});
            tokens.pushBack({spa::TOKEN_MODULO, "%"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_INTEGER, "20"});
            tokens.pushBack({spa::TOKEN_MINUS, "-"});
            tokens.pushBack({spa::TOKEN_INTEGER, "12"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({ spa::TOKEN_CLOSE_BRACKET, ")" });
            tokens.pushBack({ spa::TOKEN_CLOSE_BRACKET, ")" });
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({ spa::TOKEN_NAME, "x" });
            tokens.pushBack({ spa::TOKEN_EQUAL, "=" });
            tokens.pushBack({ spa::TOKEN_INTEGER, "3" });
            tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            tokens.pushBack({ spa::TOKEN_CLOSE_BRACE, "}" });

            expectedTokens.push_back({ spa::TOKEN_PROCEDURE, "procedure" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "z" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_WHILE, "while" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "12" });
            expectedTokens.push_back({ spa::TOKEN_COND_EQ, " == " });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "3" });
            expectedTokens.push_back({ spa::TOKEN_MODULO, "%" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "20" });
            expectedTokens.push_back({ spa::TOKEN_MINUS, "-" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "12" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "x" });
            expectedTokens.push_back({ spa::TOKEN_EQUAL, "=" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "3" });
            expectedTokens.push_back({ spa::TOKEN_SEMICOLON, ";" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            spa::SpValidator validator(tokens);
            Assert::IsTrue(validator.validateGrammar());
            Assert::IsTrue(validator.getUpdatedStream().match(expectedTokens));
        }

        //  procedure z { while ( (!( x == 12 )) || ((y != 12) && ( 2 == 2))) { X = 3; }}
        TEST_METHOD(TestSpValidatorCondExpEleven) {
            spa::Stream<spa::Token> tokens;
            std::vector<spa::Token> expectedTokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "while"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_BOOL_NOT, "!"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_NAME, "x"});
            tokens.pushBack({spa::TOKEN_COND_EQ, " == "});
            tokens.pushBack({spa::TOKEN_INTEGER, "12"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_BOOL_OR, "||"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({ spa::TOKEN_OPEN_BRACKET, "(" });
            tokens.pushBack({spa::TOKEN_NAME, "y"});
            tokens.pushBack({spa::TOKEN_COND_NEQ, " != "});
            tokens.pushBack({spa::TOKEN_INTEGER, "12"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_BOOL_AND, " && "});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_INTEGER, "2"});
            tokens.pushBack({spa::TOKEN_COND_EQ, " == "});
            tokens.pushBack({spa::TOKEN_INTEGER, "2"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({ spa::TOKEN_CLOSE_BRACKET, ")" });
            tokens.pushBack({ spa::TOKEN_CLOSE_BRACKET, ")" });
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({ spa::TOKEN_NAME, "x" });
            tokens.pushBack({ spa::TOKEN_EQUAL, "=" });
            tokens.pushBack({ spa::TOKEN_INTEGER, "3" });
            tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});

            expectedTokens.push_back({ spa::TOKEN_PROCEDURE, "procedure" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "z" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_WHILE, "while" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_BOOL_NOT, "!" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "x" });
            expectedTokens.push_back({ spa::TOKEN_COND_EQ, " == " });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "12" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_BOOL_OR, "||" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "y" });
            expectedTokens.push_back({ spa::TOKEN_COND_NEQ, " != " });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "12" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_BOOL_AND, " && " });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "2" });
            expectedTokens.push_back({ spa::TOKEN_COND_EQ, " == " });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "2" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "x" });
            expectedTokens.push_back({ spa::TOKEN_EQUAL, "=" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "3" });
            expectedTokens.push_back({ spa::TOKEN_SEMICOLON, ";" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            spa::SpValidator validator(tokens);
            Assert::IsTrue(validator.validateGrammar());
            Assert::IsTrue(validator.getUpdatedStream().match(expectedTokens));
        }

        //  procedure z { while ( (!( x == 12 )) || (( y != 12 ) && ( a > 3 ))) { X = 3; }}
        TEST_METHOD(TestSpValidatorCondExpTwelve) {
            spa::Stream<spa::Token> tokens;
            std::vector<spa::Token> expectedTokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "while"});
            tokens.pushBack({ spa::TOKEN_OPEN_BRACKET, "(" });
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_BOOL_NOT, "!"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_NAME, "x"});
            tokens.pushBack({spa::TOKEN_COND_EQ, " == "});
            tokens.pushBack({spa::TOKEN_INTEGER, "12"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({ spa::TOKEN_CLOSE_BRACKET, ")" });
            tokens.pushBack({spa::TOKEN_BOOL_OR, "||"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({ spa::TOKEN_OPEN_BRACKET, "(" });
            tokens.pushBack({spa::TOKEN_NAME, "y"});
            tokens.pushBack({spa::TOKEN_COND_NEQ, " != "});
            tokens.pushBack({spa::TOKEN_INTEGER, "12"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_BOOL_AND, " && "});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_NAME, "a"});
            tokens.pushBack({spa::TOKEN_COND_GT, " > "});
            tokens.pushBack({spa::TOKEN_INTEGER, "3"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({ spa::TOKEN_CLOSE_BRACKET, ")" });
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({ spa::TOKEN_NAME, "x" });
            tokens.pushBack({ spa::TOKEN_EQUAL, "=" });
            tokens.pushBack({ spa::TOKEN_INTEGER, "3" });
            tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            expectedTokens.push_back({ spa::TOKEN_PROCEDURE, "procedure" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "z" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_WHILE, "while" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_BOOL_NOT, "!" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "x" });
            expectedTokens.push_back({ spa::TOKEN_COND_EQ, " == " });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "12" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_BOOL_OR, "||" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "y" });
            expectedTokens.push_back({ spa::TOKEN_COND_NEQ, " != " });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "12" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_BOOL_AND, " && " });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACKET, "(" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "a" });
            expectedTokens.push_back({ spa::TOKEN_COND_GT, " > " });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "3" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACKET, ")" });
            expectedTokens.push_back({ spa::TOKEN_OPEN_BRACE, "{" });
            expectedTokens.push_back({ spa::TOKEN_NAME, "x" });
            expectedTokens.push_back({ spa::TOKEN_EQUAL, "=" });
            expectedTokens.push_back({ spa::TOKEN_INTEGER, "3" });
            expectedTokens.push_back({ spa::TOKEN_SEMICOLON, ";" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            expectedTokens.push_back({ spa::TOKEN_CLOSE_BRACE, "}" });
            spa::SpValidator validator(tokens);
            Assert::IsTrue(validator.validateGrammar());
            Assert::IsTrue(validator.getUpdatedStream().match(expectedTokens));
        }

        //  procedure z { while ( z + 1 ) {}}
        TEST_METHOD(TestSpValidatorInvalidCondExprOne) {
            spa::Stream<spa::Token> tokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "while"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_INTEGER, "12"});
            tokens.pushBack({spa::TOKEN_COND_EQ, " == "});
            tokens.pushBack({spa::TOKEN_INTEGER, "3"});
            tokens.pushBack({spa::TOKEN_MODULO, "%"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_INTEGER, "20"});
            tokens.pushBack({spa::TOKEN_MINUS, "-"});
            tokens.pushBack({spa::TOKEN_INTEGER, "12"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});

            spa::SpValidator validator(tokens);
            Assert::IsFalse(validator.validateGrammar());
        }

        //  procedure z { while ( ( z < 3)) {}}
        TEST_METHOD(TestSpValidatorInvalidCondExprTwo) {
            spa::Stream<spa::Token> tokens;
            std::vector<spa::Token> expectedTokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "while"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_COND_LT, " < "});
            tokens.pushBack({spa::TOKEN_INTEGER, "3"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            spa::SpValidator validator(tokens);
            Assert::IsFalse(validator.validateGrammar());
        }


        //  procedure z { while ( ( x == 12 ) || !( y == 12 ){}}
        TEST_METHOD(TestSpValidatorInvalidCondExpThree) {
            spa::Stream<spa::Token> tokens;
            std::vector<spa::Token> expectedTokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "while"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_NAME, "x"});
            tokens.pushBack({spa::TOKEN_COND_EQ, " == "});
            tokens.pushBack({spa::TOKEN_INTEGER, "12"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_BOOL_OR, "||"});
            tokens.pushBack({spa::TOKEN_BOOL_NOT, "!"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_NAME, "y"});
            tokens.pushBack({spa::TOKEN_COND_EQ, " == "});
            tokens.pushBack({spa::TOKEN_INTEGER, "12"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            spa::SpValidator validator(tokens);
            Assert::IsFalse(validator.validateGrammar());
        }

        //  procedure z { while ( ! (x >= 12 ) && ( 12 == 12){}}
        TEST_METHOD(TestSpValidatorInvalidCondExpFour) {
            spa::Stream<spa::Token> tokens;
            std::vector<spa::Token> expectedTokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "while"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_BOOL_NOT, "!"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_NAME, "x"});
            tokens.pushBack({spa::TOKEN_COND_GTE, " >= "});
            tokens.pushBack({spa::TOKEN_INTEGER, "12"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_BOOL_AND, "&&"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_INTEGER, "12"});
            tokens.pushBack({spa::TOKEN_COND_EQ, " == "});
            tokens.pushBack({spa::TOKEN_INTEGER, "12"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            spa::SpValidator validator(tokens);
            Assert::IsFalse(validator.validateGrammar());
        }

        //  procedure z { while ( !(12 == 12 ) && ( 12 == 12){}}
        TEST_METHOD(TestSpValidatorInvalidCondExpFive) {
            spa::Stream<spa::Token> tokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "while"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_BOOL_NOT, " !"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_INTEGER, "12"});
            tokens.pushBack({spa::TOKEN_COND_EQ, " == "});
            tokens.pushBack({spa::TOKEN_INTEGER, "12"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_BOOL_AND, "&&"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_INTEGER, "12"});
            tokens.pushBack({spa::TOKEN_COND_EQ, " == "});
            tokens.pushBack({spa::TOKEN_INTEGER, "12"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACKET, ")"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            spa::SpValidator validator(tokens);
            Assert::IsFalse(validator.validateGrammar());
        }

        //  procedure z { while ( x == 12  ||  y == 12{}}
        TEST_METHOD(TestSpValidatorInvalidCondExpSix) {
            spa::Stream<spa::Token> tokens;
            tokens.pushBack({spa::TOKEN_NAME, "procedure"});
            tokens.pushBack({spa::TOKEN_NAME, "z"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_NAME, "while"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACKET, "("});
            tokens.pushBack({spa::TOKEN_NAME, "x"});
            tokens.pushBack({spa::TOKEN_COND_EQ, " == "});
            tokens.pushBack({spa::TOKEN_INTEGER, "12"});
            tokens.pushBack({spa::TOKEN_BOOL_OR, "||"});
            tokens.pushBack({spa::TOKEN_NAME, "y"});
            tokens.pushBack({spa::TOKEN_COND_EQ, " == "});
            tokens.pushBack({spa::TOKEN_INTEGER, "12"});
            tokens.pushBack({spa::TOKEN_OPEN_BRACE, "{"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            tokens.pushBack({spa::TOKEN_CLOSE_BRACE, "}"});
            spa::SpValidator validator(tokens);
            Assert::IsFalse(validator.validateGrammar());
        }

        //  procedure a {x = 12; call b;} procedure b {x = 2;}
        TEST_METHOD(TestSpValidatorValidCallOne) {
            spa::Stream<spa::Token> tokens;
            tokens.pushBack({ spa::TOKEN_NAME, "procedure" });
            tokens.pushBack({ spa::TOKEN_NAME, "a" });
            tokens.pushBack({ spa::TOKEN_OPEN_BRACE, "{" });
            tokens.pushBack({ spa::TOKEN_NAME, "x" });
            tokens.pushBack({ spa::TOKEN_EQUAL, "=" });
            tokens.pushBack({ spa::TOKEN_INTEGER, "12" });
            tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
            tokens.pushBack({ spa::TOKEN_NAME, "call" });
            tokens.pushBack({ spa::TOKEN_NAME, "b" });
            tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
            tokens.pushBack({ spa::TOKEN_CLOSE_BRACE, "}" });
            tokens.pushBack({ spa::TOKEN_NAME, "procedure" });
            tokens.pushBack({ spa::TOKEN_NAME, "b" });
            tokens.pushBack({ spa::TOKEN_OPEN_BRACE, "{" });
            tokens.pushBack({ spa::TOKEN_NAME, "x" });
            tokens.pushBack({ spa::TOKEN_EQUAL, "=" });
            tokens.pushBack({ spa::TOKEN_INTEGER, "2" });
            tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
            tokens.pushBack({ spa::TOKEN_CLOSE_BRACE, "}" });
            spa::SpValidator validator(tokens);
            Assert::IsTrue(validator.validateGrammar());
        }

        //  procedure a {x = 12; call b; call c;} procedure b {x = 2;} procedure c {x = 3;}
        TEST_METHOD(TestSpValidatorValidCallTwo) {
            spa::Stream<spa::Token> tokens;
            tokens.pushBack({ spa::TOKEN_NAME, "procedure" });
            tokens.pushBack({ spa::TOKEN_NAME, "a" });
            tokens.pushBack({ spa::TOKEN_OPEN_BRACE, "{" });
            tokens.pushBack({ spa::TOKEN_NAME, "x" });
            tokens.pushBack({ spa::TOKEN_EQUAL, "=" });
            tokens.pushBack({ spa::TOKEN_INTEGER, "12" });
            tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
            tokens.pushBack({ spa::TOKEN_NAME, "call" });
            tokens.pushBack({ spa::TOKEN_NAME, "b" });
            tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
            tokens.pushBack({ spa::TOKEN_NAME, "call" });
            tokens.pushBack({ spa::TOKEN_NAME, "c" });
            tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
            tokens.pushBack({ spa::TOKEN_CLOSE_BRACE, "}" });
            tokens.pushBack({ spa::TOKEN_NAME, "procedure" });
            tokens.pushBack({ spa::TOKEN_NAME, "b" });
            tokens.pushBack({ spa::TOKEN_OPEN_BRACE, "{" });
            tokens.pushBack({ spa::TOKEN_NAME, "x" });
            tokens.pushBack({ spa::TOKEN_EQUAL, "=" });
            tokens.pushBack({ spa::TOKEN_INTEGER, "2" });
            tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
            tokens.pushBack({ spa::TOKEN_CLOSE_BRACE, "}" });
            tokens.pushBack({ spa::TOKEN_NAME, "procedure" });
            tokens.pushBack({ spa::TOKEN_NAME, "c" });
            tokens.pushBack({ spa::TOKEN_OPEN_BRACE, "{" });
            tokens.pushBack({ spa::TOKEN_NAME, "x" });
            tokens.pushBack({ spa::TOKEN_EQUAL, "=" });
            tokens.pushBack({ spa::TOKEN_INTEGER, "3" });
            tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
            tokens.pushBack({ spa::TOKEN_CLOSE_BRACE, "}" });
            spa::SpValidator validator(tokens);
            Assert::IsTrue(validator.validateGrammar());
        }

        //  procedure a {x = 12; call b;} procedure b {x = 2; call c;}
        TEST_METHOD(TestSpValidatorInvalidCallOne) {
            spa::Stream<spa::Token> tokens;
            tokens.pushBack({ spa::TOKEN_NAME, "procedure" });
            tokens.pushBack({ spa::TOKEN_NAME, "a" });
            tokens.pushBack({ spa::TOKEN_OPEN_BRACE, "{" });
            tokens.pushBack({ spa::TOKEN_NAME, "x" });
            tokens.pushBack({ spa::TOKEN_EQUAL, "=" });
            tokens.pushBack({ spa::TOKEN_INTEGER, "12" });
            tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
            tokens.pushBack({ spa::TOKEN_NAME, "call" });
            tokens.pushBack({ spa::TOKEN_NAME, "b" });
            tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
            tokens.pushBack({ spa::TOKEN_CLOSE_BRACE, "}" });
            tokens.pushBack({ spa::TOKEN_NAME, "procedure" });
            tokens.pushBack({ spa::TOKEN_NAME, "b" });
            tokens.pushBack({ spa::TOKEN_OPEN_BRACE, "{" });
            tokens.pushBack({ spa::TOKEN_NAME, "x" });
            tokens.pushBack({ spa::TOKEN_EQUAL, "=" });
            tokens.pushBack({ spa::TOKEN_INTEGER, "2" });
            tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
            tokens.pushBack({ spa::TOKEN_NAME, "call" });
            tokens.pushBack({ spa::TOKEN_NAME, "c" });
            tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
            tokens.pushBack({ spa::TOKEN_CLOSE_BRACE, "}" });
            spa::SpValidator validator(tokens);
            Assert::IsFalse(validator.validateGrammar());
        }

        //  procedure a {x = 12; call b; call c;} procedure b {x = 2;} procedure c {x = 3; call d;}
        TEST_METHOD(TestSpValidatorInvalidCallTwo) {
            spa::Stream<spa::Token> tokens;
            tokens.pushBack({ spa::TOKEN_NAME, "procedure" });
            tokens.pushBack({ spa::TOKEN_NAME, "a" });
            tokens.pushBack({ spa::TOKEN_OPEN_BRACE, "{" });
            tokens.pushBack({ spa::TOKEN_NAME, "x" });
            tokens.pushBack({ spa::TOKEN_EQUAL, "=" });
            tokens.pushBack({ spa::TOKEN_INTEGER, "12" });
            tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
            tokens.pushBack({ spa::TOKEN_NAME, "call" });
            tokens.pushBack({ spa::TOKEN_NAME, "b" });
            tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
            tokens.pushBack({ spa::TOKEN_NAME, "call" });
            tokens.pushBack({ spa::TOKEN_NAME, "c" });
            tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
            tokens.pushBack({ spa::TOKEN_CLOSE_BRACE, "}" });
            tokens.pushBack({ spa::TOKEN_NAME, "procedure" });
            tokens.pushBack({ spa::TOKEN_NAME, "b" });
            tokens.pushBack({ spa::TOKEN_OPEN_BRACE, "{" });
            tokens.pushBack({ spa::TOKEN_NAME, "x" });
            tokens.pushBack({ spa::TOKEN_EQUAL, "=" });
            tokens.pushBack({ spa::TOKEN_INTEGER, "2" });
            tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
            tokens.pushBack({ spa::TOKEN_CLOSE_BRACE, "}" });
            tokens.pushBack({ spa::TOKEN_NAME, "procedure" });
            tokens.pushBack({ spa::TOKEN_NAME, "c" });
            tokens.pushBack({ spa::TOKEN_OPEN_BRACE, "{" });
            tokens.pushBack({ spa::TOKEN_NAME, "x" });
            tokens.pushBack({ spa::TOKEN_EQUAL, "=" });
            tokens.pushBack({ spa::TOKEN_INTEGER, "3" });
            tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
            tokens.pushBack({ spa::TOKEN_NAME, "call" });
            tokens.pushBack({ spa::TOKEN_NAME, "d" });
            tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
            tokens.pushBack({ spa::TOKEN_CLOSE_BRACE, "}" });
            spa::SpValidator validator(tokens);
            Assert::IsFalse(validator.validateGrammar());
        }
    };
}  //  namespace UnitTesting
