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
    TEST_CLASS(TestDesignExtractorWhile) {
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

        bool checkVectorEquals(std::vector<std::pair<int, std::string>> expected,
                               std::vector<std::pair<int, std::string>> actual) {
            if (expected.size() != actual.size()) {
                return false;
            }
            for (unsigned i = 0; i < expected.size(); i++) {
                if (expected[i].first != actual[i].first || expected[i].second != actual[i].second) {
                    return false;
                }
            }
            return true;
        }

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
            for (spa::Token token : tokenList) {
                tokenStream.pushBack(token);
            }
            std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
            auto parser = spa::SpParser(tokenStream);
            std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();
            Assert::IsTrue(procedureList.size() == 1);

            spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
            designExtractor.extractRelationship();

            std::vector<std::pair<int, std::string>> expectedWhile = {{1, "c"}, {1, "d"}};
            spa::PKBQueryArg firstArg = spa::PKBQueryArg(
                spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
            spa::QueryResult testWhile = pkbManager->getContainerPattern(spa::WHILE, firstArg);

            Assert::IsTrue(checkVectorEquals(expectedWhile, testWhile.getLineNumberNamePairs()));
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
            for (spa::Token token : tokenList) {
                tokenStream.pushBack(token);
            }
            std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
            auto parser = spa::SpParser(tokenStream);
            std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();
            Assert::IsTrue(procedureList.size() == 1);

            spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
            designExtractor.extractRelationship();

            std::vector<std::pair<int, std::string>> negWhile = {{1, "a"}, {1, "d"}};

            spa::PKBQueryArg firstArg = spa::PKBQueryArg(
                spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
            spa::QueryResult testWhile = pkbManager->getContainerPattern(spa::WHILE, firstArg);

            Assert::IsFalse(checkVectorEquals(negWhile, testWhile.getLineNumberNamePairs()));
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
            for (spa::Token token : tokenList) {
                tokenStream.pushBack(token);
            }
            std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
            auto parser = spa::SpParser(tokenStream);
            std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();
            Assert::IsTrue(procedureList.size() == 1);

            spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
            designExtractor.extractRelationship();

            std::vector<std::pair<int, std::string>> expectedWhile = {
                {1, "c"}, {1, "d"}, {3, "e"}, {3, "a"}, {3, "b"}
            };
            spa::PKBQueryArg firstArg = spa::PKBQueryArg(
                spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
            spa::QueryResult testWhile = pkbManager->getContainerPattern(spa::WHILE, firstArg);

            Assert::IsTrue(checkVectorEquals(expectedWhile, testWhile.getLineNumberNamePairs()));
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
            for (spa::Token token : tokenList) {
                tokenStream.pushBack(token);
            }
            std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
            auto parser = spa::SpParser(tokenStream);
            std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();
            Assert::IsTrue(procedureList.size() == 1);

            spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
            designExtractor.extractRelationship();

            std::vector<std::pair<int, std::string>> expectedWhile = {
                {1, "a"}, {1, "d"}, {3, "e"}, {3, "a"}, {3, "b"}
            };
            spa::PKBQueryArg firstArg = spa::PKBQueryArg(
                spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
            spa::QueryResult testWhile = pkbManager->getContainerPattern(spa::WHILE, firstArg);

            Assert::IsFalse(expectedWhile == testWhile.getLineNumberNamePairs());
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
            for (spa::Token token : tokenList) {
                tokenStream.pushBack(token);
            }
            std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
            auto parser = spa::SpParser(tokenStream);
            std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();
            Assert::IsTrue(procedureList.size() == 1);

            spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
            designExtractor.extractRelationship();

            std::vector<std::pair<int, std::string>> expectedWhile = {{1, "b"}, {3, "b"}, {3, "e"}};
            spa::PKBQueryArg firstArg = spa::PKBQueryArg(
                spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
            spa::QueryResult testWhile = pkbManager->getContainerPattern(spa::WHILE, firstArg);

            Assert::IsTrue(checkVectorEquals(expectedWhile, testWhile.getLineNumberNamePairs()));
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
            for (spa::Token token : tokenList) {
                tokenStream.pushBack(token);
            }
            std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
            auto parser = spa::SpParser(tokenStream);
            std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();
            Assert::IsTrue(procedureList.size() == 1);

            spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
            designExtractor.extractRelationship();

            std::vector<std::pair<int, std::string>> expectedWhile = {{1, "b"}, {3, "b"}};
            spa::PKBQueryArg firstArg = spa::PKBQueryArg(
                spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
            spa::QueryResult testWhile = pkbManager->getContainerPattern(spa::WHILE, firstArg);

            Assert::IsFalse(expectedWhile == testWhile.getLineNumberNamePairs());
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
            for (spa::Token token : tokenList) {
                tokenStream.pushBack(token);
            }
            std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
            auto parser = spa::SpParser(tokenStream);
            std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();
            Assert::IsTrue(procedureList.size() == 1);

            spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
            designExtractor.extractRelationship();

            std::vector<std::pair<int, std::string>> expectedWhile = {
                {1, "b"}, {2, "b"}, {2, "e"}, {3, "b"}, {3, "c"}, {3, "d"}
            };
            spa::PKBQueryArg firstArg = spa::PKBQueryArg(
                spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
            spa::QueryResult testWhile = pkbManager->getContainerPattern(spa::WHILE, firstArg);

            Assert::IsTrue(checkVectorEquals(expectedWhile, testWhile.getLineNumberNamePairs()));
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
            for (spa::Token token : tokenList) {
                tokenStream.pushBack(token);
            }
            std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
            auto parser = spa::SpParser(tokenStream);
            std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();
            Assert::IsTrue(procedureList.size() == 1);

            spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
            designExtractor.extractRelationship();

            std::vector<std::pair<int, std::string>> expectedWhile = {
                {2, "b"}, {2, "e"}, {3, "b"}, {3, "c"}, {3, "d"}
            };
            spa::PKBQueryArg firstArg = spa::PKBQueryArg(
                spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
            spa::QueryResult testWhile = pkbManager->getContainerPattern(spa::WHILE, firstArg);

            Assert::IsFalse(expectedWhile == testWhile.getLineNumberNamePairs());
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
            for (spa::Token token : tokenList) {
                tokenStream.pushBack(token);
            }
            std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
            auto parser = spa::SpParser(tokenStream);
            std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();
            Assert::IsTrue(procedureList.size() == 1);

            spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
            designExtractor.extractRelationship();

            std::vector<std::pair<int, std::string>> expectedWhile = {
                {1, "b"}, {2, "b"}, {2, "e"}, {3, "b"}, {3, "c"}, {3, "d"}, {4, "b"}, {4, "d"}
            };
            spa::PKBQueryArg firstArg = spa::PKBQueryArg(
                spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
            spa::QueryResult testWhile = pkbManager->getContainerPattern(spa::WHILE, firstArg);

            Assert::IsTrue(checkVectorEquals(expectedWhile, testWhile.getLineNumberNamePairs()));
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
            for (spa::Token token : tokenList) {
                tokenStream.pushBack(token);
            }
            std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
            auto parser = spa::SpParser(tokenStream);
            std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();
            Assert::IsTrue(procedureList.size() == 1);

            spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
            designExtractor.extractRelationship();

            std::vector<std::pair<int, std::string>> expectedWhile = {
                {2, "b"}, {2, "e"}, {3, "b"}, {3, "c"}, {3, "d"}
            };
            spa::PKBQueryArg firstArg = spa::PKBQueryArg(
                spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
            spa::QueryResult testWhile = pkbManager->getContainerPattern(spa::WHILE, firstArg);

            Assert::IsFalse(expectedWhile == testWhile.getLineNumberNamePairs());
        }

        TEST_METHOD(TestExtractWhileIf) {
            /*
            *  procedure a {
            *1.    while (a >= 10) {
            *2.        if (b >= e) then{
            *3.            c = 10;
            *          }
            *          else {
            *4.            d = a;
            *          }
            *      }
            *  }            
            */
            tokenList = {
                tokenProcedure, tokenA, tokenOpenBrace,
                tokenWhile, tokenOpenBracket, tokenA, tokenGreaterEqual, tokenConstant, tokenCloseBracket,
                tokenOpenBrace, tokenIf, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenE, tokenCloseBracket,
                tokenThen, tokenOpenBrace, tokenC, tokenAssign, tokenConstant, tokenSemiColon, tokenCloseBrace,
                tokenElse, tokenOpenBrace, tokenD, tokenAssign, tokenA, tokenSemiColon, tokenCloseBrace,
                tokenCloseBrace, tokenCloseBrace
            };
            spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
            for (spa::Token token : tokenList) {
                tokenStream.pushBack(token);
            }
            std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
            auto parser = spa::SpParser(tokenStream);
            std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();
            Assert::IsTrue(procedureList.size() == 1);

            spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
            designExtractor.extractRelationship();

            std::vector<std::pair<int, std::string>> expectedWhile = { {1, "a"}};
            spa::PKBQueryArg firstArgWhile = spa::PKBQueryArg(
                spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
            spa::QueryResult testWhile = pkbManager->getContainerPattern(spa::WHILE, firstArgWhile);

            Assert::IsTrue(expectedWhile == testWhile.getLineNumberNamePairs());

            std::vector<std::pair<int, std::string>> expectedIf = { {2, "b"}, {2, "e"}};

            spa::PKBQueryArg firstArgIf = spa::PKBQueryArg(
                spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
            spa::QueryResult testIf = pkbManager->getContainerPattern(spa::IF, firstArgIf);

            Assert::IsTrue(checkVectorEquals(expectedIf, testIf.getLineNumberNamePairs()));
        }

        TEST_METHOD(TestExtractWhileIfIf) {
            /*
            *  procedure a {
            *1.    while (b>= a) {
            *2.        c = 5;
            *3.        if (c >= 10) then {
            *4.            a = 5;
            *5.            if (c >= 10) then {
            *6.                a = 5;
            *              }
            *              else {
            *7.                a = 5;
            *              }
            *          }
            *          else {
            *8.            a = 5;
            *          }
            *      }
            *  }
            */
            tokenList = {
                tokenProcedure, tokenA, tokenOpenBrace,
                tokenWhile, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenA, tokenCloseBracket,
                tokenOpenBrace, tokenC, tokenAssign, tokenConstant, tokenSemiColon, tokenIf, tokenOpenBracket,
                tokenC, tokenGreaterEqual, tokenConstant, tokenCloseBracket, tokenThen, tokenOpenBrace,
                tokenA, tokenAssign, tokenConstant, tokenSemiColon, tokenIf, tokenOpenBracket, tokenC,
                tokenGreaterEqual, tokenConstant, tokenCloseBracket, tokenThen, tokenOpenBrace, tokenA,
                tokenAssign, tokenConstant, tokenSemiColon, tokenCloseBrace, tokenElse, tokenOpenBrace,
                tokenA, tokenAssign,
                tokenConstant, tokenSemiColon, tokenCloseBrace, tokenCloseBrace, tokenElse, tokenOpenBrace, tokenA,
                tokenAssign, tokenConstant, tokenSemiColon, tokenCloseBrace, tokenCloseBrace, tokenCloseBrace
            };
            spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
            for (spa::Token token : tokenList) {
                tokenStream.pushBack(token);
            }
            std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
            auto parser = spa::SpParser(tokenStream);
            std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();
            Assert::IsTrue(procedureList.size() == 1);

            spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
            designExtractor.extractRelationship();

            std::vector<std::pair<int, std::string>> expectedWhile = { {1, "b"}, {1, "a"}};
            spa::PKBQueryArg firstArgWhile = spa::PKBQueryArg(
                spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
            spa::QueryResult testWhile = pkbManager->getContainerPattern(spa::WHILE, firstArgWhile);

            Assert::IsTrue(expectedWhile == testWhile.getLineNumberNamePairs());

            std::vector<std::pair<int, std::string>> expectedIf = { {3, "c"}, {5, "c"} };

            spa::PKBQueryArg firstArgIf = spa::PKBQueryArg(
                spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
            spa::QueryResult testIf = pkbManager->getContainerPattern(spa::IF, firstArgIf);

            Assert::IsTrue(checkVectorEquals(expectedIf, testIf.getLineNumberNamePairs()));
        }

        TEST_METHOD(TestExtractWhileIfIfElse) {
            /*
            *  procedure a {
            *1.    while (b>= a) {
            *2.        c = 5;
            *3.        if (c >= 10) then {
            *4.            a = 5;
            *          }
            *          else {
            *5.            a = 5;
            *6.            if (c >= 10) then {
            *7.                a = 5;
            *              }
            *              else {
            *8.                a = 5;
            *              }
            *          }
            *      }
            *  }
            */
            tokenList = {
                tokenProcedure, tokenA, tokenOpenBrace,
                tokenWhile, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenA, tokenCloseBracket,
                tokenOpenBrace, tokenC, tokenAssign, tokenConstant, tokenSemiColon, tokenIf, tokenOpenBracket,
                tokenC, tokenGreaterEqual, tokenConstant, tokenCloseBracket, tokenThen, tokenOpenBrace,
                tokenA, tokenAssign, tokenConstant, tokenSemiColon, tokenCloseBrace, tokenElse, tokenOpenBrace,
                tokenA, tokenAssign, tokenConstant, tokenSemiColon, tokenIf, tokenOpenBracket, tokenC,
                tokenGreaterEqual, tokenConstant, tokenCloseBracket, tokenThen, tokenOpenBrace, tokenA,
                tokenAssign, tokenConstant, tokenSemiColon, tokenCloseBrace, tokenElse, tokenOpenBrace, tokenA,
                tokenAssign, tokenConstant, tokenSemiColon, tokenCloseBrace, tokenCloseBrace, tokenCloseBrace,
                tokenCloseBrace
            };
            spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
            for (spa::Token token : tokenList) {
                tokenStream.pushBack(token);
            }
            std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
            auto parser = spa::SpParser(tokenStream);
            std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();
            Assert::IsTrue(procedureList.size() == 1);

            spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
            designExtractor.extractRelationship();

            std::vector<std::pair<int, std::string>> expectedWhile = { {1, "b"}, {1, "a"} };
            spa::PKBQueryArg firstArgWhile = spa::PKBQueryArg(
                spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
            spa::QueryResult testWhile = pkbManager->getContainerPattern(spa::WHILE, firstArgWhile);

            Assert::IsTrue(expectedWhile == testWhile.getLineNumberNamePairs());

            std::vector<std::pair<int, std::string>> expectedIf = { {3, "c"}, {6, "c"} };

            spa::PKBQueryArg firstArgIf = spa::PKBQueryArg(
                spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
            spa::QueryResult testIf = pkbManager->getContainerPattern(spa::IF, firstArgIf);

            Assert::IsTrue(checkVectorEquals(expectedIf, testIf.getLineNumberNamePairs()));
        }

        TEST_METHOD(TestExtractWhileIfWhileIf) {
            /*
            *   procedure a {
            *1.     while (b>= a) {
            *2.        c = 5;
            *3.        if (c >= 10) then {
            *4.            a = 5;
            *5.            while (b>= a) {
            *6.                c = 5;
            *7.                if (c >= 10) then {
            *8.                    a = 5;
            *                  }
            *                  else {
            *9.                    a = 5;
            *                  }
            *              }
            *          }
            *          else {
            *10.            a = 5;
            *          }
            *      }
            *  }
            */
            tokenList = {
                tokenProcedure, tokenA, tokenOpenBrace,
                tokenWhile, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenA, tokenCloseBracket,
                tokenOpenBrace, tokenC, tokenAssign, tokenConstant, tokenSemiColon, tokenIf, tokenOpenBracket,
                tokenC, tokenGreaterEqual, tokenConstant, tokenCloseBracket, tokenThen, tokenOpenBrace,
                tokenA, tokenAssign, tokenConstant, tokenSemiColon, tokenWhile, tokenOpenBracket, tokenB,
                tokenGreaterEqual, tokenA, tokenCloseBracket, tokenOpenBrace, tokenC, tokenAssign, tokenConstant,
                tokenSemiColon, tokenIf, tokenOpenBracket, tokenC, tokenGreaterEqual, tokenConstant, tokenCloseBracket,
                tokenThen, tokenOpenBrace, tokenA, tokenAssign, tokenConstant, tokenSemiColon, tokenCloseBrace,
                tokenElse, tokenOpenBrace, tokenA, tokenAssign, tokenConstant, tokenSemiColon, tokenCloseBrace,
                tokenCloseBrace, tokenCloseBrace, tokenElse, tokenOpenBrace, tokenA, tokenAssign, tokenConstant,
                tokenSemiColon, tokenCloseBrace, tokenCloseBrace, tokenCloseBrace
            };
            spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
            for (spa::Token token : tokenList) {
                tokenStream.pushBack(token);
            }
            std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
            auto parser = spa::SpParser(tokenStream);
            std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();
            Assert::IsTrue(procedureList.size() == 1);

            spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
            designExtractor.extractRelationship();

            std::vector<std::pair<int, std::string>> expectedWhile = { {1, "b"}, {1, "a"}, {5, "b"}, {5, "a"} };
            spa::PKBQueryArg firstArgWhile = spa::PKBQueryArg(
                spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
            spa::QueryResult testWhile = pkbManager->getContainerPattern(spa::WHILE, firstArgWhile);

            Assert::IsTrue(expectedWhile == testWhile.getLineNumberNamePairs());

            std::vector<std::pair<int, std::string>> expectedIf = { {3, "c"}, {7, "c"} };

            spa::PKBQueryArg firstArgIf = spa::PKBQueryArg(
                spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
            spa::QueryResult testIf = pkbManager->getContainerPattern(spa::IF, firstArgIf);

            Assert::IsTrue(checkVectorEquals(expectedIf, testIf.getLineNumberNamePairs()));
        }

        TEST_METHOD(TestExtractWhileIfElseWhileIf) {
            /*
            *  procedure a {
            *1.    while (b>= a) {
            *2.        c = 5;
            *3.        if (c >= 10) then {
            *4.            a = 5;
            *          }
            *          else {
            *5.            a = 5;
            *6.            while (b>= a) {
            *7.                c = 5;
            *8.                if (c >= 10) then {
            *9.                    a = 5;
            *                  }
            *                  else {
            *10.                    a = 5;
            *                  }
            *              }
            *          }
            *      }
            *  }
            */
            tokenList = {
                tokenProcedure, tokenA, tokenOpenBrace,
                tokenWhile, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenA, tokenCloseBracket,
                tokenOpenBrace, tokenC, tokenAssign, tokenConstant, tokenSemiColon, tokenIf, tokenOpenBracket,
                tokenC, tokenGreaterEqual, tokenConstant, tokenCloseBracket, tokenThen,
                tokenOpenBrace, tokenA, tokenAssign, tokenConstant, tokenSemiColon, tokenCloseBrace, tokenElse,
                tokenOpenBrace, tokenA, tokenAssign, tokenConstant, tokenSemiColon, tokenWhile, tokenOpenBracket,
                tokenB, tokenGreaterEqual, tokenA, tokenCloseBracket, tokenOpenBrace, tokenC, tokenAssign,
                tokenConstant, tokenSemiColon, tokenIf, tokenOpenBracket, tokenC, tokenGreaterEqual, tokenConstant,
                tokenCloseBracket, tokenThen, tokenOpenBrace, tokenA, tokenAssign, tokenConstant, tokenSemiColon,
                tokenCloseBrace, tokenElse, tokenOpenBrace, tokenA, tokenAssign, tokenConstant, tokenSemiColon,
                tokenCloseBrace, tokenCloseBrace, tokenCloseBrace, tokenCloseBrace, tokenCloseBrace
            };
            spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
            for (spa::Token token : tokenList) {
                tokenStream.pushBack(token);
            }
            std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
            auto parser = spa::SpParser(tokenStream);
            std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();
            Assert::IsTrue(procedureList.size() == 1);

            spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
            designExtractor.extractRelationship();

            std::vector<std::pair<int, std::string>> expectedWhile = { {1, "b"}, {1, "a"}, {6, "b"}, {6, "a"} };
            spa::PKBQueryArg firstArgWhile = spa::PKBQueryArg(
                spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
            spa::QueryResult testWhile = pkbManager->getContainerPattern(spa::WHILE, firstArgWhile);

            Assert::IsTrue(expectedWhile == testWhile.getLineNumberNamePairs());

            std::vector<std::pair<int, std::string>> expectedIf = { {3, "c"}, {8, "c"} };

            spa::PKBQueryArg firstArgIf = spa::PKBQueryArg(
                spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
            spa::QueryResult testIf = pkbManager->getContainerPattern(spa::IF, firstArgIf);

            Assert::IsTrue(checkVectorEquals(expectedIf, testIf.getLineNumberNamePairs()));
        }
    };
}  // namespace UnitTesting
