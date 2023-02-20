#include <iostream>
#include <vector>

#include "stdafx.h"

#include "DesignExtractor.h"
#include "PKB.h"
#include "SpParser.h"
#include "Token.h"
#include "UtilsFunction.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace UnitTesting {
    TEST_CLASS(TestDesignExtractorUses) {
        std::string varA = "a";
        std::string varB = "b";
        std::string varC = "c";
        std::string varD = "d";
        std::string varE = "e";
        std::string varF = "f";
        std::string varG = "g";
        std::string constant = "1";

        std::string openBracket = "(";
        std::string closeBracket = ")";
        std::string openBrace = "{";
        std::string closeBrace = "}";
        std::string semiColon = ";";

        std::string minusOp = "-";
        std::string multiplyOp = "*";
        std::string greaterEqOp = ">=";
        std::string equal = "=";

        std::string readString = "read";
        std::string callString = "call";
        std::string printString = "print";
        std::string procedure = "procedure";
        std::string ifString = "if";
        std::string thenString = "then";
        std::string elseString = "else";
        std::string whileString = "while";

        spa::Token tokenA = spa::Token(spa::TOKEN_NAME, varA);
        spa::Token tokenB = spa::Token(spa::TOKEN_NAME, varB);
        spa::Token tokenC = spa::Token(spa::TOKEN_NAME, varC);
        spa::Token tokenD = spa::Token(spa::TOKEN_NAME, varD);
        spa::Token tokenE = spa::Token(spa::TOKEN_NAME, varE);
        spa::Token tokenF = spa::Token(spa::TOKEN_NAME, varF);
        spa::Token tokenG = spa::Token(spa::TOKEN_NAME, varG);
        spa::Token tokenConstant = spa::Token(spa::TOKEN_INTEGER, constant);

        spa::Token tokenOpenBracket = spa::Token(spa::TOKEN_OPEN_BRACKET, openBracket);
        spa::Token tokenCloseBracket = spa::Token(spa::TOKEN_CLOSE_BRACKET, closeBracket);
        spa::Token tokenOpenBrace = spa::Token(spa::TOKEN_OPEN_BRACE, openBrace);
        spa::Token tokenCloseBrace = spa::Token(spa::TOKEN_CLOSE_BRACE, closeBrace);
        spa::Token tokenSemiColon = spa::Token(spa::TOKEN_SEMICOLON, semiColon);

        spa::Token tokenMinusOp = spa::Token(spa::TOKEN_MINUS, minusOp);
        spa::Token tokenMultiply = spa::Token(spa::TOKEN_MULTIPLY, multiplyOp);
        spa::Token tokenAssign = spa::Token(spa::TOKEN_EQUAL, equal);
        spa::Token tokenGreaterEqual = spa::Token(spa::TOKEN_COND_GTE, greaterEqOp);

        spa::Token tokenRead = spa::Token(spa::TOKEN_READ, readString);
        spa::Token tokenCall = spa::Token(spa::TOKEN_CALL, callString);
        spa::Token tokenPrint = spa::Token(spa::TOKEN_PRINT, printString);
        spa::Token tokenProcedure = spa::Token(spa::TOKEN_PROCEDURE, procedure);
        spa::Token tokenIf = spa::Token(spa::TOKEN_IF, ifString);
        spa::Token tokenThen = spa::Token(spa::TOKEN_THEN, thenString);
        spa::Token tokenElse = spa::Token(spa::TOKEN_ELSE, elseString);
        spa::Token tokenWhile = spa::Token(spa::TOKEN_WHILE, whileString);


        std::vector<spa::Token> tokenList{};

    public:
        TEST_METHOD(TestExtractSingleWhile) {
            /*
             *  procedure a {
             * 1. while ( c >= d) {
             * 2.  a = b;
             *    }
             *  }
             */
            tokenList = {
                tokenProcedure, tokenA, tokenOpenBrace,
                tokenWhile, tokenOpenBracket, tokenC, tokenGreaterEqual, tokenD, tokenCloseBracket,
                tokenOpenBrace,
                tokenA, tokenAssign, tokenB, tokenSemiColon, tokenCloseBrace, tokenCloseBrace
            };
            spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
            for (auto token : tokenList) {
                tokenStream.pushBack(token);
            }
            std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
            auto parser = spa::SpParser(tokenStream);
            std::vector<spa::ProcedureStatement> procedureList = parser.parse();
            Assert::IsTrue(procedureList.size() == 1);

            spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
            designExtractor.extractRelationship();

            std::vector<std::pair<int, std::string>> expectedWhile = {{1, "c"}, {1, "d"}};
            spa::PKBQueryArg firstArg = spa::PKBQueryArg(
                spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
            spa::QueryResult testWhile = pkbManager->getContainerPattern(spa::WHILE, firstArg);

            Assert::IsTrue(expectedWhile == testWhile.getLineNumberVariablePairs());
        }

        TEST_METHOD(TestExtractSingleWhileNegative) {
            /*
             *  procedure a {
             * 1. while ( c >= d) {
             * 2.  a = b;
             *    }
             *  }
             */
            tokenList = {
                tokenProcedure, tokenA, tokenOpenBrace,
                tokenWhile, tokenOpenBracket, tokenC, tokenGreaterEqual, tokenD, tokenCloseBracket,
                tokenOpenBrace,
                tokenA, tokenAssign, tokenB, tokenSemiColon, tokenCloseBrace, tokenCloseBrace
            };
            spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
            for (auto token : tokenList) {
                tokenStream.pushBack(token);
            }
            std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
            auto parser = spa::SpParser(tokenStream);
            std::vector<spa::ProcedureStatement> procedureList = parser.parse();
            Assert::IsTrue(procedureList.size() == 1);

            spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
            designExtractor.extractRelationship();

            std::vector<std::pair<int, std::string>> negWhile = {{1, "a"}, {1, "d"}};

            spa::PKBQueryArg firstArg = spa::PKBQueryArg(
                spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
            spa::QueryResult testWhile = pkbManager->getContainerPattern(spa::WHILE, firstArg);

            Assert::IsFalse(negWhile == testWhile.getLineNumberVariablePairs());
        }

        TEST_METHOD(TestExtractDoubleSingleWhile) {
            /*
             *  procedure a {
             * 1. while ( c >= d) {
             * 2.  a = b;
             *    }
             * 3. while ( e >= ( a * b)) {
             * 4. c = d;
             *    }
             *  }
             */
            tokenList = {
                tokenProcedure, tokenA, tokenOpenBrace,
                tokenWhile, tokenOpenBracket, tokenC, tokenGreaterEqual, tokenD, tokenCloseBracket,
                tokenOpenBrace,
                tokenA, tokenAssign, tokenB, tokenSemiColon, tokenCloseBrace,
                tokenWhile, tokenOpenBracket, tokenE, tokenGreaterEqual, tokenOpenBracket,
                tokenA, tokenMultiply, tokenB, tokenCloseBracket, tokenCloseBracket,
                tokenOpenBrace, tokenC, tokenAssign, tokenD, tokenSemiColon, tokenCloseBrace,
                tokenCloseBrace
            };
            spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
            for (auto token : tokenList) {
                tokenStream.pushBack(token);
            }
            std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
            auto parser = spa::SpParser(tokenStream);
            std::vector<spa::ProcedureStatement> procedureList = parser.parse();
            Assert::IsTrue(procedureList.size() == 1);

            spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
            designExtractor.extractRelationship();

            std::vector<std::pair<int, std::string>> expectedWhile = {
                {1, "c"}, {1, "d"}, {3, "a"}, {3, "b"}, {3, "e"}
            };
            spa::PKBQueryArg firstArg = spa::PKBQueryArg(
                spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
            spa::QueryResult testWhile = pkbManager->getContainerPattern(spa::WHILE, firstArg);

            Assert::IsTrue(expectedWhile == testWhile.getLineNumberVariablePairs());
        }

        TEST_METHOD(TestExtractDoubleSingleWhileNegative) {
            /*
             *  procedure a {
             * 1. while ( c >= d) {
             * 2.  a = b;
             *    }
             * 3. while ( e >= ( a * b)) {
             * 4. c = d;
             *    }
             *  }
             */
            tokenList = {
                tokenProcedure, tokenA, tokenOpenBrace,
                tokenWhile, tokenOpenBracket, tokenC, tokenGreaterEqual, tokenD, tokenCloseBracket,
                tokenOpenBrace,
                tokenA, tokenAssign, tokenB, tokenSemiColon, tokenCloseBrace,
                tokenWhile, tokenOpenBracket, tokenE, tokenGreaterEqual, tokenOpenBracket,
                tokenA, tokenMultiply, tokenB, tokenCloseBracket, tokenCloseBracket,
                tokenOpenBrace, tokenC, tokenAssign, tokenD, tokenSemiColon, tokenCloseBrace,
                tokenCloseBrace
            };
            spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
            for (auto token : tokenList) {
                tokenStream.pushBack(token);
            }
            std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
            auto parser = spa::SpParser(tokenStream);
            std::vector<spa::ProcedureStatement> procedureList = parser.parse();
            Assert::IsTrue(procedureList.size() == 1);

            spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
            designExtractor.extractRelationship();

            std::vector<std::pair<int, std::string>> expectedWhile = {
                {1, "a"}, {1, "d"}, {3, "e"}, {3, "a"}, {3, "b"}
            };
            spa::PKBQueryArg firstArg = spa::PKBQueryArg(
                spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
            spa::QueryResult testWhile = pkbManager->getContainerPattern(spa::WHILE, firstArg);

            Assert::IsFalse(expectedWhile == testWhile.getLineNumberVariablePairs());
        }

        TEST_METHOD(TestExtractNestedOneWhile) {
            /*
             *   procedure a {
             * 1. while (b >= 1) {
             * 2.   d = b;
             * 3.   while (b >= (e - 1)) {
             * 4.     d = b;
             * 5.     d = b;
             *      }
             *    }
             * 6. b = 1;
             *  }
             */
            tokenList = {
                tokenProcedure, tokenA, tokenOpenBrace,
                tokenWhile, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenConstant,
                tokenCloseBracket, tokenOpenBrace,
                tokenD, tokenAssign, tokenB, tokenSemiColon,
                tokenWhile, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenOpenBracket, tokenE,
                tokenMinusOp,
                tokenConstant, tokenCloseBracket, tokenCloseBracket, tokenOpenBrace,
                tokenD, tokenAssign, tokenB, tokenSemiColon,
                tokenD, tokenAssign, tokenB, tokenSemiColon,
                tokenCloseBrace,
                tokenCloseBrace,
                tokenB, tokenAssign, tokenConstant, tokenSemiColon,
                tokenCloseBrace
            };
            spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
            for (auto token : tokenList) {
                tokenStream.pushBack(token);
            }
            std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
            auto parser = spa::SpParser(tokenStream);
            std::vector<spa::ProcedureStatement> procedureList = parser.parse();
            Assert::IsTrue(procedureList.size() == 1);

            spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
            designExtractor.extractRelationship();

            std::vector<std::pair<int, std::string>> expectedWhile = {{1, "b"}, {3, "b"}, {3, "e"}};
            spa::PKBQueryArg firstArg = spa::PKBQueryArg(
                spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
            spa::QueryResult testWhile = pkbManager->getContainerPattern(spa::WHILE, firstArg);

            Assert::IsTrue(expectedWhile == testWhile.getLineNumberVariablePairs());
        }

        TEST_METHOD(TestExtractNestedOneWhileNegative) {
            /*
             *   procedure a {
             * 1. while (b >= 1) {
             * 2.   d = b;
             * 3.   while (b >= (e - 1)) {
             * 4.     d = b;
             * 5.     d = b;
             *      }
             *    }
             * 6. b = 1;
             *  }
             */
            tokenList = {
                tokenProcedure, tokenA, tokenOpenBrace,
                tokenWhile, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenConstant,
                tokenCloseBracket, tokenOpenBrace,
                tokenD, tokenAssign, tokenB, tokenSemiColon,
                tokenWhile, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenOpenBracket, tokenE,
                tokenMinusOp,
                tokenConstant, tokenCloseBracket, tokenCloseBracket, tokenOpenBrace,
                tokenD, tokenAssign, tokenB, tokenSemiColon,
                tokenD, tokenAssign, tokenB, tokenSemiColon,
                tokenCloseBrace,
                tokenCloseBrace,
                tokenB, tokenAssign, tokenConstant, tokenSemiColon,
                tokenCloseBrace
            };
            spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
            for (auto token : tokenList) {
                tokenStream.pushBack(token);
            }
            std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
            auto parser = spa::SpParser(tokenStream);
            std::vector<spa::ProcedureStatement> procedureList = parser.parse();
            Assert::IsTrue(procedureList.size() == 1);

            spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
            designExtractor.extractRelationship();

            std::vector<std::pair<int, std::string>> expectedWhile = {{1, "b"}, {3, "b"}};
            spa::PKBQueryArg firstArg = spa::PKBQueryArg(
                spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
            spa::QueryResult testWhile = pkbManager->getContainerPattern(spa::WHILE, firstArg);

            Assert::IsFalse(expectedWhile == testWhile.getLineNumberVariablePairs());
        }

        TEST_METHOD(TestExtractNestedTwoWhile) {
            /*
             *   procedure a {
             * 1. while (b >= 1) {
             * 2.   while (b >= (e - 1)) {
             * 3.     while (b >= (c - d)) {
             * 4.        d = a;
             *        }
             *      }
             *    }
             * 5. b = 1;
             *  }
             */
            tokenList = {
                tokenProcedure, tokenA, tokenOpenBrace,
                tokenWhile, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenConstant,
                tokenCloseBracket, tokenOpenBrace,
                tokenWhile, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenOpenBracket,
                tokenE, tokenMinusOp, tokenConstant, tokenCloseBracket, tokenCloseBracket,
                tokenOpenBrace,
                tokenWhile, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenOpenBracket, tokenC,
                tokenMinusOp,
                tokenD, tokenCloseBracket, tokenCloseBracket, tokenOpenBrace,
                tokenD, tokenAssign, tokenA, tokenSemiColon,
                tokenCloseBrace,
                tokenCloseBrace,
                tokenCloseBrace,
                tokenB, tokenAssign, tokenConstant, tokenSemiColon,
                tokenCloseBrace
            };
            spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
            for (auto token : tokenList) {
                tokenStream.pushBack(token);
            }
            std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
            auto parser = spa::SpParser(tokenStream);
            std::vector<spa::ProcedureStatement> procedureList = parser.parse();
            Assert::IsTrue(procedureList.size() == 1);

            spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
            designExtractor.extractRelationship();

            std::vector<std::pair<int, std::string>> expectedWhile = {
                {1, "b"}, {2, "b"}, {2, "e"}, {3, "b"}, {3, "c"}, {3, "d"}
            };
            spa::PKBQueryArg firstArg = spa::PKBQueryArg(
                spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
            spa::QueryResult testWhile = pkbManager->getContainerPattern(spa::WHILE, firstArg);

            Assert::IsTrue(expectedWhile == testWhile.getLineNumberVariablePairs());
        }

        TEST_METHOD(TestExtractNestedTwoWhileNegative) {
            /*
             *   procedure a {
             * 1. while (b >= 1) {
             * 2.   while (b >= (e - 1)) {
             * 3.     while (b >= (c - d)) {
             * 4.        d = a;
             * 5.        b = 10;
             *        }
             *      }
             *    }
             * 6. b = 1;
             *  }
             */
            tokenList = {
                tokenProcedure, tokenA, tokenOpenBrace,
                tokenWhile, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenConstant,
                tokenCloseBracket, tokenOpenBrace,
                tokenWhile, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenOpenBracket,
                tokenE, tokenMinusOp, tokenConstant, tokenCloseBracket, tokenCloseBracket,
                tokenOpenBrace,
                tokenWhile, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenOpenBracket, tokenC,
                tokenMinusOp,
                tokenD, tokenCloseBracket, tokenCloseBracket, tokenOpenBrace,
                tokenD, tokenAssign, tokenA, tokenSemiColon,
                tokenB, tokenAssign, tokenConstant, tokenSemiColon,
                tokenCloseBrace,
                tokenCloseBrace,
                tokenCloseBrace,
                tokenB, tokenAssign, tokenConstant, tokenSemiColon,
                tokenCloseBrace
            };
            spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
            for (auto token : tokenList) {
                tokenStream.pushBack(token);
            }
            std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
            auto parser = spa::SpParser(tokenStream);
            std::vector<spa::ProcedureStatement> procedureList = parser.parse();
            Assert::IsTrue(procedureList.size() == 1);

            spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
            designExtractor.extractRelationship();

            std::vector<std::pair<int, std::string>> expectedWhile = {
                {2, "b"}, {2, "e"}, {3, "b"}, {3, "c"}, {3, "d"}
            };
            spa::PKBQueryArg firstArg = spa::PKBQueryArg(
                spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
            spa::QueryResult testWhile = pkbManager->getContainerPattern(spa::WHILE, firstArg);

            Assert::IsFalse(expectedWhile == testWhile.getLineNumberVariablePairs());
        }

        TEST_METHOD(TestExtractNestedThreeWhile) {
            /*
             *   procedure a {
             * 1. while (b >= 1) {
             * 2.   while (b >= (e - 1)) {
             * 3.     while (b >= (c - d)) {
             * 4.       while (b >= d) {
             * 5.       d = a;
             *          }
             *        }
             *      }
             *    }
             *    b = 1;
             *  }
             */
            tokenList = {
                tokenProcedure, tokenA, tokenOpenBrace,
                tokenWhile, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenConstant,
                tokenCloseBracket, tokenOpenBrace,
                tokenWhile, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenOpenBracket,
                tokenE, tokenMinusOp, tokenConstant, tokenCloseBracket, tokenCloseBracket,
                tokenOpenBrace,
                tokenWhile, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenOpenBracket, tokenC,
                tokenMinusOp,
                tokenD, tokenCloseBracket, tokenCloseBracket, tokenOpenBrace,
                tokenWhile, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenD, tokenCloseBracket,
                tokenOpenBrace,
                tokenD, tokenAssign, tokenA, tokenSemiColon,
                tokenCloseBrace,
                tokenCloseBrace,
                tokenCloseBrace,
                tokenCloseBrace,
                tokenB, tokenAssign, tokenConstant, tokenSemiColon,
                tokenCloseBrace
            };
            spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
            for (auto token : tokenList) {
                tokenStream.pushBack(token);
            }
            std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
            auto parser = spa::SpParser(tokenStream);
            std::vector<spa::ProcedureStatement> procedureList = parser.parse();
            Assert::IsTrue(procedureList.size() == 1);

            spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
            designExtractor.extractRelationship();

            std::vector<std::pair<int, std::string>> expectedWhile = {
                {1, "b"}, {2, "b"}, {2, "e"}, {3, "b"}, {3, "c"}, {3, "d"}, {4, "b"}, {4, "d"}
            };
            spa::PKBQueryArg firstArg = spa::PKBQueryArg(
                spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
            spa::QueryResult testWhile = pkbManager->getContainerPattern(spa::WHILE, firstArg);

            Assert::IsTrue(expectedWhile == testWhile.getLineNumberVariablePairs());
        }

        TEST_METHOD(TestExtractNestedThreeWhileNegative) {
            /*
             *   procedure a {
             * 1. while (b >= 1) {
             * 2.   while (b >= (e - 1)) {
             * 3.     while (b >= (c - d)) {
             * 4.       while (b >= d) {
             * 4.        d = a;
             * 5.        b = 10;
             *          }
             *        }
             *      }
             *    }
             * 6. b = 1;
             *  }
             */
            tokenList = {
                tokenProcedure, tokenA, tokenOpenBrace,
                tokenWhile, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenConstant,
                tokenCloseBracket, tokenOpenBrace,
                tokenWhile, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenOpenBracket,
                tokenE, tokenMinusOp, tokenConstant, tokenCloseBracket, tokenCloseBracket,
                tokenOpenBrace,
                tokenWhile, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenOpenBracket, tokenC,
                tokenMinusOp,
                tokenD, tokenCloseBracket, tokenCloseBracket, tokenOpenBrace,
                tokenD, tokenAssign, tokenA, tokenSemiColon,
                tokenB, tokenAssign, tokenConstant, tokenSemiColon,
                tokenCloseBrace,
                tokenCloseBrace,
                tokenCloseBrace,
                tokenB, tokenAssign, tokenConstant, tokenSemiColon,
                tokenCloseBrace
            };
            spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
            for (auto token : tokenList) {
                tokenStream.pushBack(token);
            }
            std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
            auto parser = spa::SpParser(tokenStream);
            std::vector<spa::ProcedureStatement> procedureList = parser.parse();
            Assert::IsTrue(procedureList.size() == 1);

            spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
            designExtractor.extractRelationship();

            std::vector<std::pair<int, std::string>> expectedWhile = {
                {2, "b"}, {2, "e"}, {3, "b"}, {3, "c"}, {3, "d"}
            };
            spa::PKBQueryArg firstArg = spa::PKBQueryArg(
                spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
            spa::QueryResult testWhile = pkbManager->getContainerPattern(spa::WHILE, firstArg);

            Assert::IsFalse(expectedWhile == testWhile.getLineNumberVariablePairs());
        }
    };
}  // namespace UnitTesting
