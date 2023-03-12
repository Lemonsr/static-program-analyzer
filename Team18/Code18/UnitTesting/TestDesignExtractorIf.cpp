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
    TEST_CLASS(TestDesignExtractorIf) {
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
            for (int i = 0; i < expected.size(); i++) {
                bool found = false;
                for (int j = 0; j < actual.size(); j++) {
                    if (expected[i].first == actual[j].first && expected[i].second == actual[j].second) {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    return false;
                }
            }
            return true;
        }

    public:
        TEST_METHOD(TestExtractSingleIfThenElse) {
            /*
             *  procedure a {
             * 1. if ( c >= d) then {
             * 2.    a = b;
             *    }
             *    else {
             * 3.    e = f; 
             *    }
             *  }
             */
            tokenList = {
                tokenProcedure, tokenA, tokenOpenBrace,
                tokenIf, tokenOpenBracket, tokenC, tokenGreaterEqual, tokenD, tokenCloseBracket,
                tokenThen, tokenOpenBrace,
                tokenA, tokenAssign, tokenB, tokenSemiColon, tokenCloseBrace,
                tokenElse, tokenOpenBrace,
                tokenE, tokenAssign, tokenF, tokenSemiColon, tokenCloseBrace, tokenCloseBrace
            };
            spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
            for (auto token : tokenList) {
                tokenStream.pushBack(token);
            }
            std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
            auto parser = spa::SpParser(tokenStream);
            std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();
            Assert::IsTrue(procedureList.size() == 1);

            spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
            designExtractor.extractRelationship();

            std::vector<std::pair<int, std::string>> expectedIf = {{1, "c"}, {1, "d"}};
            spa::PKBQueryArg firstArg = spa::PKBQueryArg(
                spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
            spa::QueryResult testIf = pkbManager->getContainerPattern(spa::IF, firstArg);

            Assert::IsTrue(checkVectorEquals(expectedIf, testIf.getLineNumberNamePairs()));
        }

        TEST_METHOD(TestExtractSingleIfThenElseNegative) {
            /*
             *  procedure a {
             * 1. if ( c >= d) then {
             * 2.    a = b;
             *    }
             *    else {
             * 3.    e = f;
             *    }
             *  }
             */
            tokenList = {
                tokenProcedure, tokenA, tokenOpenBrace,
                tokenIf, tokenOpenBracket, tokenC, tokenGreaterEqual, tokenD, tokenCloseBracket,
                tokenThen, tokenOpenBrace,
                tokenA, tokenAssign, tokenB, tokenSemiColon, tokenCloseBrace,
                tokenElse, tokenOpenBrace,
                tokenE, tokenAssign, tokenF, tokenSemiColon, tokenCloseBrace, tokenCloseBrace
            };
            spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
            for (auto token : tokenList) {
                tokenStream.pushBack(token);
            }
            std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
            auto parser = spa::SpParser(tokenStream);
            std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();
            Assert::IsTrue(procedureList.size() == 1);

            spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
            designExtractor.extractRelationship();

            std::vector<std::pair<int, std::string>> negIf = {{1, "a"}, {1, "d"}};

            spa::PKBQueryArg firstArg = spa::PKBQueryArg(
                spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
            spa::QueryResult testIf = pkbManager->getContainerPattern(spa::IF, firstArg);

            Assert::IsFalse(checkVectorEquals(negIf, testIf.getLineNumberNamePairs()));
        }

        TEST_METHOD(TestExtractDoubleSingleIfThenElse) {
            /*
             *  procedure a {
             * 1. if ( c >= d) then {
             * 2.    a = b;
             *    }
             *    else {
             * 3.    e = f;
             *    }
             * 4. if ( d >= 6 ) then {
             * 5.    f = e;
             *    }
             *    else {
             * 6.    a = b;
             *    }
             *  }
             */
            tokenList = {
                tokenProcedure, tokenA, tokenOpenBrace,
                tokenIf, tokenOpenBracket, tokenC, tokenGreaterEqual, tokenD, tokenCloseBracket,
                tokenThen, tokenOpenBrace,
                tokenA, tokenAssign, tokenB, tokenSemiColon, tokenCloseBrace,
                tokenElse, tokenOpenBrace,
                tokenE, tokenAssign, tokenF, tokenSemiColon, tokenCloseBrace,
                tokenIf, tokenOpenBracket, tokenD, tokenGreaterEqual, tokenConstant,
                tokenCloseBracket,
                tokenThen, tokenOpenBrace,
                tokenF, tokenAssign, tokenE, tokenSemiColon, tokenCloseBrace,
                tokenElse, tokenOpenBrace,
                tokenA, tokenAssign, tokenB, tokenSemiColon,
                tokenCloseBrace, tokenCloseBrace
            };
            spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
            for (auto token : tokenList) {
                tokenStream.pushBack(token);
            }
            std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
            auto parser = spa::SpParser(tokenStream);
            std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();
            Assert::IsTrue(procedureList.size() == 1);

            spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
            designExtractor.extractRelationship();

            std::vector<std::pair<int, std::string>> expectedIf = {{1, "c"}, {1, "d"}, {4, "d"}};
            spa::PKBQueryArg firstArg = spa::PKBQueryArg(
                spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
            spa::QueryResult testIf = pkbManager->getContainerPattern(spa::IF, firstArg);

            Assert::IsTrue(checkVectorEquals(expectedIf, testIf.getLineNumberNamePairs()));
        }

        TEST_METHOD(TestExtractDoubleSingleIfThenElseNegative) {
            /*
             *  procedure a {
             * 1. if ( c >= d) then {
             * 2.    a = b;
             *    }
             *    else {
             * 3.    e = f;
             *    }
             * 4. if ( d >= 6 ) then {
             * 5.    f = e;
             *    }
             *    else {
             * 6.    a = b;
             *    }
             *  }
             */
            tokenList = {
                tokenProcedure, tokenA, tokenOpenBrace,
                tokenIf, tokenOpenBracket, tokenC, tokenGreaterEqual, tokenD, tokenCloseBracket,
                tokenThen, tokenOpenBrace,
                tokenA, tokenAssign, tokenB, tokenSemiColon, tokenCloseBrace,
                tokenElse, tokenOpenBrace,
                tokenE, tokenAssign, tokenF, tokenSemiColon, tokenCloseBrace,
                tokenIf, tokenOpenBracket, tokenD, tokenGreaterEqual, tokenConstant,
                tokenCloseBracket,
                tokenThen, tokenOpenBrace,
                tokenF, tokenAssign, tokenE, tokenSemiColon, tokenCloseBrace,
                tokenElse, tokenOpenBrace,
                tokenA, tokenAssign, tokenB, tokenSemiColon,
                tokenCloseBrace, tokenCloseBrace
            };
            spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
            for (auto token : tokenList) {
                tokenStream.pushBack(token);
            }
            std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
            auto parser = spa::SpParser(tokenStream);
            std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();
            Assert::IsTrue(procedureList.size() == 1);

            spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
            designExtractor.extractRelationship();

            std::vector<std::pair<int, std::string>> negIf = {
                {1, "a"}, {1, "d"}, {3, "e"}, {3, "a"}, {3, "b"}
            };
            spa::PKBQueryArg firstArg = spa::PKBQueryArg(
                spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
            spa::QueryResult testIf = pkbManager->getContainerPattern(spa::IF, firstArg);

            Assert::IsFalse(checkVectorEquals(negIf, testIf.getLineNumberNamePairs()));
        }

        TEST_METHOD(TestExtractNestedOneIfThenElse) {
            /*
             *  procedure a {
             * 1. if ( c >= d) then {
             * 2.    a = b;
             * 3.    if ( e >= a) then {
             * 4.       f = e;
             *       }
             *       else {
             * 5.       a = f;
             *       }
             *     }
             *  else {
             * 6.  e = f;
             *    }
             *  }
             */
            tokenList = {
                tokenProcedure, tokenA, tokenOpenBrace,
                tokenIf, tokenOpenBracket, tokenC, tokenGreaterEqual, tokenD, tokenCloseBracket,
                tokenThen, tokenOpenBrace,
                tokenA, tokenAssign, tokenB, tokenSemiColon,
                tokenIf, tokenOpenBracket, tokenE, tokenGreaterEqual, tokenA, tokenCloseBracket,
                tokenThen, tokenOpenBrace, tokenF, tokenAssign, tokenE, tokenSemiColon,
                tokenCloseBrace,
                tokenElse, tokenOpenBrace,
                tokenA, tokenAssign, tokenF, tokenSemiColon, tokenCloseBrace,
                tokenCloseBrace, tokenElse, tokenOpenBrace,
                tokenE, tokenAssign, tokenF, tokenSemiColon, tokenCloseBrace,
                tokenCloseBrace
            };
            spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
            for (auto token : tokenList) {
                tokenStream.pushBack(token);
            }
            std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
            auto parser = spa::SpParser(tokenStream);
            std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();
            Assert::IsTrue(procedureList.size() == 1);

            spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
            designExtractor.extractRelationship();

            std::vector<std::pair<int, std::string>> expectedIf = {
                {1, "c"}, {1, "d"}, {3, "a"}, {3, "e"}
            };
            spa::PKBQueryArg firstArg = spa::PKBQueryArg(
                spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
            spa::QueryResult testIf = pkbManager->getContainerPattern(spa::IF, firstArg);

            Assert::IsTrue(checkVectorEquals(expectedIf, testIf.getLineNumberNamePairs()));
        }

        TEST_METHOD(TestExtractNestedOneIfElseThenNegative) {
            /*
             *  procedure a {
             * 1. if ( c >= d) then {
             * 2.    a = b;
             * 3.    if ( e >= a) then {
             * 4.       f = e;
             *       }
             *       else {
             * 5.       a = f;
             *       }
             *     }
             *  else {
             * 6.  e = f;
             *    }
             *  }
             */
            tokenList = {
                tokenProcedure, tokenA, tokenOpenBrace,
                tokenIf, tokenOpenBracket, tokenC, tokenGreaterEqual, tokenD, tokenCloseBracket,
                tokenThen, tokenOpenBrace,
                tokenA, tokenAssign, tokenB, tokenSemiColon,
                tokenIf, tokenOpenBracket, tokenE, tokenGreaterEqual, tokenA, tokenCloseBracket,
                tokenThen, tokenOpenBrace, tokenF, tokenAssign, tokenE, tokenSemiColon,
                tokenCloseBrace,
                tokenElse, tokenOpenBrace,
                tokenA, tokenAssign, tokenF, tokenSemiColon, tokenCloseBrace,
                tokenCloseBrace, tokenElse, tokenOpenBrace,
                tokenE, tokenAssign, tokenF, tokenSemiColon, tokenCloseBrace,
                tokenCloseBrace
            };
            spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
            for (auto token : tokenList) {
                tokenStream.pushBack(token);
            }
            std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
            auto parser = spa::SpParser(tokenStream);
            std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();
            Assert::IsTrue(procedureList.size() == 1);

            spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
            designExtractor.extractRelationship();

            std::vector<std::pair<int, std::string>> negIf = {{1, "b"}, {3, "b"}};
            spa::PKBQueryArg firstArg = spa::PKBQueryArg(
                spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
            spa::QueryResult testIf = pkbManager->getContainerPattern(spa::IF, firstArg);

            Assert::IsFalse(checkVectorEquals(negIf, testIf.getLineNumberNamePairs()));
        }

        TEST_METHOD(TestExtractNestedTwoIfElseThen) {
            /*
             *  procedure a {
             * 1. if ( c >= d) then {
             * 2.    a = b;
             * 3.    if ( e >= a) then {
             * 4.       f = e;
             * 5.       if ( g >= e) then {
             * 6.          b = b;
             *          }
             *          else {
             * 7.          b = c;
             *          }
             *       }
             *       else {
             * 8.       a = f;
             *       }
             *  }
             *  else {
             * 9.  e = f;
             *    }
             *  }
             */
            tokenList = {
                tokenProcedure, tokenA, tokenOpenBrace,
                tokenIf, tokenOpenBracket, tokenC, tokenGreaterEqual, tokenD, tokenCloseBracket,
                tokenThen, tokenOpenBrace,
                tokenA, tokenAssign, tokenB, tokenSemiColon,
                tokenIf, tokenOpenBracket, tokenE, tokenGreaterEqual, tokenA, tokenCloseBracket,
                tokenThen, tokenOpenBrace, tokenF, tokenAssign, tokenE, tokenSemiColon,
                tokenIf, tokenOpenBracket, tokenG, tokenGreaterEqual, tokenE, tokenCloseBracket,
                tokenThen, tokenOpenBrace,
                tokenB, tokenAssign, tokenB, tokenSemiColon, tokenCloseBrace,
                tokenElse, tokenOpenBrace,
                tokenB, tokenAssign, tokenC, tokenSemiColon, tokenCloseBrace,
                tokenCloseBrace, tokenElse, tokenOpenBrace,
                tokenA, tokenAssign, tokenF, tokenSemiColon, tokenCloseBrace,
                tokenCloseBrace, tokenElse, tokenOpenBrace,
                tokenE, tokenAssign, tokenF, tokenSemiColon, tokenCloseBrace,
                tokenCloseBrace
            };
            spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
            for (auto token : tokenList) {
                tokenStream.pushBack(token);
            }
            std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
            auto parser = spa::SpParser(tokenStream);
            std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();
            Assert::IsTrue(procedureList.size() == 1);

            spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
            designExtractor.extractRelationship();

            std::vector<std::pair<int, std::string>> expectedIf = {
                {1, "c"}, {1, "d"}, {3, "a"}, {3, "e"}, {5, "e"}, {5, "g"}
            };
            spa::PKBQueryArg firstArg = spa::PKBQueryArg(
                spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
            spa::QueryResult testIf = pkbManager->getContainerPattern(spa::IF, firstArg);

            Assert::IsTrue(checkVectorEquals(expectedIf, testIf.getLineNumberNamePairs()));
        }

        TEST_METHOD(TestExtractNestedTwoIfElseThenNegative) {
            /*
             *  procedure a {
             * 1. if ( c >= d) then {
             * 2.    a = b;
             * 3.    if ( e >= a) then {
             * 4.       f = e;
             * 5.       if ( g >= e) then {
             * 6.          b = b;
             *          }
             *          else {
             * 7.          b = c;
             *          }
             *       }
             *       else {
             * 8.       a = f;
             *       }
             *  }
             *  else {
             * 9.  e = f;
             *    }
             *  }
             */
            tokenList = {
                tokenProcedure, tokenA, tokenOpenBrace,
                tokenIf, tokenOpenBracket, tokenC, tokenGreaterEqual, tokenD, tokenCloseBracket,
                tokenThen, tokenOpenBrace,
                tokenA, tokenAssign, tokenB, tokenSemiColon,
                tokenIf, tokenOpenBracket, tokenE, tokenGreaterEqual, tokenA, tokenCloseBracket,
                tokenThen, tokenOpenBrace, tokenF, tokenAssign, tokenE, tokenSemiColon,
                tokenIf, tokenOpenBracket, tokenG, tokenGreaterEqual, tokenE, tokenCloseBracket,
                tokenThen, tokenOpenBrace,
                tokenB, tokenAssign, tokenB, tokenSemiColon, tokenCloseBrace,
                tokenElse, tokenOpenBrace,
                tokenB, tokenAssign, tokenC, tokenSemiColon, tokenCloseBrace,
                tokenCloseBrace, tokenElse, tokenOpenBrace,
                tokenA, tokenAssign, tokenF, tokenSemiColon, tokenCloseBrace,
                tokenCloseBrace, tokenElse, tokenOpenBrace,
                tokenE, tokenAssign, tokenF, tokenSemiColon, tokenCloseBrace,
                tokenCloseBrace
            };
            spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
            for (auto token : tokenList) {
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
            spa::QueryResult testWhile = pkbManager->getContainerPattern(spa::IF, firstArg);

            Assert::IsFalse(expectedWhile == testWhile.getLineNumberNamePairs());
        }

        TEST_METHOD(TestExtractNestedThreeIfElseThen) {
            /*
             *  procedure a {
             * 1. if ( c >= d) then {
             * 2.    a = b;
             * 3.    if ( e >= a) then {
             * 4.       f = e;
             * 5.       if ( g >= e) then {
             * 6.          b = b;
             * 7.          if( a >= b) then {
             * 8.             b = c;
             *             }
             *             else {
             * 9.             a = f;
             *             }
             *          }
             *          else {
             * 10.         b = c;
             *          }
             *       }
             *       else {
             * 11.       a = f;
             *       }
             *  }
             *  else {
             * 12.  e = f;
             *    }
             *  }
             */

            tokenList = {
                tokenProcedure, tokenA, tokenOpenBrace,
                tokenIf, tokenOpenBracket, tokenC, tokenGreaterEqual, tokenD, tokenCloseBracket,
                tokenThen, tokenOpenBrace,
                tokenA, tokenAssign, tokenB, tokenSemiColon,
                tokenIf, tokenOpenBracket, tokenE, tokenGreaterEqual, tokenA, tokenCloseBracket,
                tokenThen, tokenOpenBrace, tokenF, tokenAssign, tokenE, tokenSemiColon,
                tokenIf, tokenOpenBracket, tokenG, tokenGreaterEqual, tokenE, tokenCloseBracket,
                tokenThen, tokenOpenBrace,
                tokenB, tokenAssign, tokenB, tokenSemiColon,
                tokenIf, tokenOpenBracket, tokenA, tokenGreaterEqual, tokenB, tokenCloseBracket,
                tokenThen, tokenOpenBrace,
                tokenB, tokenAssign, tokenC, tokenSemiColon, tokenCloseBrace,
                tokenElse, tokenOpenBrace,
                tokenA, tokenAssign, tokenF, tokenSemiColon, tokenCloseBrace, tokenCloseBrace,
                tokenElse, tokenOpenBrace,
                tokenB, tokenAssign, tokenC, tokenSemiColon, tokenCloseBrace,
                tokenCloseBrace, tokenElse, tokenOpenBrace,
                tokenA, tokenAssign, tokenF, tokenSemiColon, tokenCloseBrace,
                tokenCloseBrace, tokenElse, tokenOpenBrace,
                tokenE, tokenAssign, tokenF, tokenSemiColon, tokenCloseBrace,
                tokenCloseBrace
            };
            spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
            for (auto token : tokenList) {
                tokenStream.pushBack(token);
            }
            std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
            auto parser = spa::SpParser(tokenStream);
            std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();
            Assert::IsTrue(procedureList.size() == 1);

            spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
            designExtractor.extractRelationship();

            std::vector<std::pair<int, std::string>> expectedIf = {
                {1, "c"}, {1, "d"}, {3, "a"}, {3, "e"}, {5, "e"}, {5, "g"}, {7, "a"}, {7, "b"}
            };
            spa::PKBQueryArg firstArg = spa::PKBQueryArg(
                spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
            spa::QueryResult testIf = pkbManager->getContainerPattern(spa::IF, firstArg);

            Assert::IsTrue(checkVectorEquals(expectedIf, testIf.getLineNumberNamePairs()));
        }

        TEST_METHOD(TestExtractNestedThreeIfElseThenNegative) {
            /*
             *  procedure a {
             * 1. if ( c >= d) then {
             * 2.    a = b;
             * 3.    if ( e >= a) then {
             * 4.       f = e;
             * 5.       if ( g >= e) then {
             * 6.          b = b;
             * 7.          if( a >= b) then {
             * 8.             b = c;
             *             }
             *             else {
             * 9.             a = f;
             *             }
             *          }
             *          else {
             * 10.         b = c;
             *          }
             *       }
             *       else {
             * 11.       a = f;
             *       }
             *  }
             *  else {
             * 12.  e = f;
             *    }
             *  }
             */

            tokenList = {
                tokenProcedure, tokenA, tokenOpenBrace,
                tokenIf, tokenOpenBracket, tokenC, tokenGreaterEqual, tokenD, tokenCloseBracket,
                tokenThen, tokenOpenBrace,
                tokenA, tokenAssign, tokenB, tokenSemiColon,
                tokenIf, tokenOpenBracket, tokenE, tokenGreaterEqual, tokenA, tokenCloseBracket,
                tokenThen, tokenOpenBrace, tokenF, tokenAssign, tokenE, tokenSemiColon,
                tokenIf, tokenOpenBracket, tokenG, tokenGreaterEqual, tokenE, tokenCloseBracket,
                tokenThen, tokenOpenBrace,
                tokenB, tokenAssign, tokenB, tokenSemiColon,
                tokenIf, tokenOpenBracket, tokenA, tokenGreaterEqual, tokenB, tokenCloseBracket,
                tokenThen, tokenOpenBrace,
                tokenB, tokenAssign, tokenC, tokenSemiColon, tokenCloseBrace,
                tokenElse, tokenOpenBrace,
                tokenA, tokenAssign, tokenF, tokenSemiColon, tokenCloseBrace, tokenCloseBrace,
                tokenElse, tokenOpenBrace,
                tokenB, tokenAssign, tokenC, tokenSemiColon, tokenCloseBrace,
                tokenCloseBrace, tokenElse, tokenOpenBrace,
                tokenA, tokenAssign, tokenF, tokenSemiColon, tokenCloseBrace,
                tokenCloseBrace, tokenElse, tokenOpenBrace,
                tokenE, tokenAssign, tokenF, tokenSemiColon, tokenCloseBrace,
                tokenCloseBrace
            };
            spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
            for (auto token : tokenList) {
                tokenStream.pushBack(token);
            }
            std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
            auto parser = spa::SpParser(tokenStream);
            std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();
            Assert::IsTrue(procedureList.size() == 1);

            spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
            designExtractor.extractRelationship();

            std::vector<std::pair<int, std::string>> negIf = {
                {1, "c"}, {1, "d"}, {3, "a"}, {3, "e"}, {5, "e"}, {5, "z"}, {7, "a"}, {7, "b"}
            };
            spa::PKBQueryArg firstArg = spa::PKBQueryArg(
                spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
            spa::QueryResult testIf = pkbManager->getContainerPattern(spa::IF, firstArg);

            Assert::IsFalse(checkVectorEquals(negIf, testIf.getLineNumberNamePairs()));
        }

        TEST_METHOD(TestExtractIfWhile) {
            /*
             *   procedure a{
             *1.   if (b >= 1) then {
             *2.       c = 5;
             *3.       while (d >= 3) {
             *4.           e = 6;
             *         }
             *     }
             *5.   else {
             *6.       c = 5;
             *     }
             *   }
             */
            tokenList = {
                tokenProcedure, tokenA, tokenOpenBrace,
                tokenIf, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenConstant, tokenCloseBracket,
                tokenThen, tokenOpenBrace,
                tokenC, tokenAssign, tokenConstant, tokenSemiColon,
                tokenWhile, tokenOpenBracket, tokenD, tokenGreaterEqual, tokenConstant, tokenCloseBracket,
                tokenOpenBrace, tokenE, tokenAssign, tokenConstant, tokenSemiColon, tokenCloseBrace,
                tokenCloseBrace, tokenElse, tokenOpenBrace,
                tokenC, tokenAssign, tokenConstant, tokenSemiColon, tokenCloseBrace,
                tokenCloseBrace
            };
            spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
            for (auto token : tokenList) {
                tokenStream.pushBack(token);
            }
            std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
            auto parser = spa::SpParser(tokenStream);
            std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();
            Assert::IsTrue(procedureList.size() == 1);

            spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
            designExtractor.extractRelationship();

            std::vector<std::pair<int, std::string>> expectedIf = { {1, "b"}};

            spa::PKBQueryArg firstArgIf = spa::PKBQueryArg(
                spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
            spa::QueryResult testIf = pkbManager->getContainerPattern(spa::IF, firstArgIf);

            Assert::IsTrue(checkVectorEquals(expectedIf, testIf.getLineNumberNamePairs()));

            std::vector<std::pair<int, std::string>> expectedWhile = { {3, "d"}};
            spa::PKBQueryArg firstArgWhile = spa::PKBQueryArg(
                spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
            spa::QueryResult testWhile = pkbManager->getContainerPattern(spa::WHILE, firstArgWhile);

            Assert::IsTrue(expectedWhile == testWhile.getLineNumberNamePairs());
        }
        TEST_METHOD(TestExtractIfWhileIf) {
            /*
             *   procedure a {
             *1.    if (b >= 1) then {
             *2.        c = 5;
             *3.        while (d >= 3) {
             *4.            e = 6;
             *5.            if (b >= 1) then {
             *6.                c = 5;
             *            }
             *            else {
             *7.                c = 5;
             *            }
             *        }
             *      }
             *8.    else {
             *9.        c = 5;
             *     }
             *   }
             */

            tokenList = {
                tokenProcedure, tokenA, tokenOpenBrace,
                tokenIf, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenConstant, tokenCloseBracket,
                tokenThen, tokenOpenBrace,
                tokenC, tokenAssign, tokenConstant, tokenSemiColon,
                tokenWhile, tokenOpenBracket, tokenD, tokenGreaterEqual, tokenConstant, tokenCloseBracket,
                tokenOpenBrace, tokenE, tokenAssign, tokenConstant, tokenSemiColon, tokenIf, tokenOpenBracket,
                tokenB, tokenGreaterEqual, tokenConstant, tokenCloseBracket, tokenThen, tokenOpenBrace,
                tokenC, tokenAssign, tokenConstant, tokenSemiColon, tokenCloseBrace, tokenElse, tokenOpenBrace,
                tokenC, tokenAssign, tokenConstant, tokenSemiColon, tokenCloseBrace, tokenCloseBrace,
                tokenCloseBrace, tokenElse, tokenOpenBrace,
                tokenC, tokenAssign, tokenConstant, tokenSemiColon, tokenCloseBrace,
                tokenCloseBrace
            };
            spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
            for (auto token : tokenList) {
                tokenStream.pushBack(token);
            }
            std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
            auto parser = spa::SpParser(tokenStream);
            std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();
            Assert::IsTrue(procedureList.size() == 1);

            spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
            designExtractor.extractRelationship();

            std::vector<std::pair<int, std::string>> expectedIf = { {1, "b"}, {5, "b"} };

            spa::PKBQueryArg firstArgIf = spa::PKBQueryArg(
                spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
            spa::QueryResult testIf = pkbManager->getContainerPattern(spa::IF, firstArgIf);

            Assert::IsTrue(checkVectorEquals(expectedIf, testIf.getLineNumberNamePairs()));

            std::vector<std::pair<int, std::string>> expectedWhile = { {3, "d"} };
            spa::PKBQueryArg firstArgWhile = spa::PKBQueryArg(
                spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
            spa::QueryResult testWhile = pkbManager->getContainerPattern(spa::WHILE, firstArgWhile);

            Assert::IsTrue(expectedWhile == testWhile.getLineNumberNamePairs());
        }

        TEST_METHOD(TestExtractIfElseWhileIf) {
            /*
            *   procedure a {
            *1.    if (b >= 1) then {
            *2.        c = 5;
            *      }
            *      else {
            *3.        c = 5;
            *4.        while (d >= 3) {
            *5.            e = 6;
            *6.            if (b  >= 1) then {
            *7.                c = 5;
            *              }
            *              else {
            *8.                c = 5;
            *              }
            *          }
            *      }
            *   }
             */

            tokenList = {
                tokenProcedure, tokenA, tokenOpenBrace,
                tokenIf, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenConstant, tokenCloseBracket,
                tokenThen, tokenOpenBrace,
                tokenC, tokenAssign, tokenConstant, tokenSemiColon,
                tokenCloseBrace, tokenElse, tokenOpenBrace,
                tokenC, tokenAssign, tokenConstant, tokenSemiColon,
                tokenWhile, tokenOpenBracket, tokenD, tokenGreaterEqual, tokenConstant, tokenCloseBracket,
                tokenOpenBrace, tokenE, tokenAssign, tokenConstant, tokenSemiColon, tokenIf, tokenOpenBracket,
                tokenB, tokenGreaterEqual, tokenConstant, tokenCloseBracket, tokenThen, tokenOpenBrace,
                tokenC, tokenAssign, tokenConstant, tokenSemiColon, tokenCloseBrace, tokenElse, tokenOpenBrace,
                tokenC, tokenAssign, tokenConstant, tokenSemiColon, tokenCloseBrace, tokenCloseBrace,
                tokenCloseBrace, tokenCloseBrace
            };
            spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
            for (auto token : tokenList) {
                tokenStream.pushBack(token);
            }
            std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
            auto parser = spa::SpParser(tokenStream);
            std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();
            Assert::IsTrue(procedureList.size() == 1);

            spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
            designExtractor.extractRelationship();

            std::vector<std::pair<int, std::string>> expectedIf = { {1, "b"}, {6, "b"} };

            spa::PKBQueryArg firstArgIf = spa::PKBQueryArg(
                spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
            spa::QueryResult testIf = pkbManager->getContainerPattern(spa::IF, firstArgIf);

            Assert::IsTrue(checkVectorEquals(expectedIf, testIf.getLineNumberNamePairs()));

            std::vector<std::pair<int, std::string>> expectedWhile = { {4, "d"} };
            spa::PKBQueryArg firstArgWhile = spa::PKBQueryArg(
                spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
            spa::QueryResult testWhile = pkbManager->getContainerPattern(spa::WHILE, firstArgWhile);

            Assert::IsTrue(expectedWhile == testWhile.getLineNumberNamePairs());
        }

        TEST_METHOD(TestExtractIfWhileWhile) {
            /*
            *   procedure a {
            *1.    if (c >= e) then {
            *2.        while (d >= a) {
            *3.            while (b >= c) {
            *4.                a = 10;
            *              }
            *          }
            *      }
            *5.    else {
            *6.        d = 5;
            *      }
            *  }
             */

            tokenList = {
                tokenProcedure, tokenA, tokenOpenBrace,
                tokenIf, tokenOpenBracket, tokenC, tokenGreaterEqual, tokenE, tokenCloseBracket,
                tokenThen, tokenOpenBrace,
                tokenWhile, tokenOpenBracket, tokenD, tokenGreaterEqual, tokenA, tokenCloseBracket,
                tokenOpenBrace, tokenWhile, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenC,
                tokenCloseBracket, tokenOpenBrace, tokenA, tokenAssign, tokenConstant, tokenSemiColon,
                tokenCloseBrace, tokenCloseBrace, tokenCloseBrace, tokenElse, tokenOpenBrace,
                tokenD, tokenAssign, tokenConstant, tokenSemiColon, tokenCloseBrace, tokenCloseBrace
            };
            spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
            for (auto token : tokenList) {
                tokenStream.pushBack(token);
            }
            std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
            auto parser = spa::SpParser(tokenStream);
            std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();
            Assert::IsTrue(procedureList.size() == 1);

            spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
            designExtractor.extractRelationship();

            std::vector<std::pair<int, std::string>> expectedIf = { {1, "c"}, {1, "e"} };

            spa::PKBQueryArg firstArgIf = spa::PKBQueryArg(
                spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
            spa::QueryResult testIf = pkbManager->getContainerPattern(spa::IF, firstArgIf);

            Assert::IsTrue(checkVectorEquals(expectedIf, testIf.getLineNumberNamePairs()));

            std::vector<std::pair<int, std::string>> expectedWhile = { {2, "d"}, {2, "a"}, {3, "b"}, {3, "c"} };
            spa::PKBQueryArg firstArgWhile = spa::PKBQueryArg(
                spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
            spa::QueryResult testWhile = pkbManager->getContainerPattern(spa::WHILE, firstArgWhile);

            Assert::IsTrue(expectedWhile == testWhile.getLineNumberNamePairs());
        }

        TEST_METHOD(TestExtractIfWhileIfElseWhile) {
            /*
            *  procedure a {
            *1.    if (a >= b) then {
            *2.        while (c >= 10) {
            *3.            if (a >= c) then {
            *4.                d = 10;
            *              }
            *              else {
            *5.                while (e >= 10) {
            *6.                     b = 10;
            *                  }
            *              }
            *          }
            *      }
            *      else {
            *7.      e = 20;
            *      }
            *  }
            */

            tokenList = {
                tokenProcedure, tokenA, tokenOpenBrace, tokenIf,
                tokenOpenBracket, tokenA, tokenGreaterEqual, tokenB, tokenCloseBracket,
                tokenThen, tokenOpenBrace, tokenWhile, tokenOpenBracket, tokenC, tokenGreaterEqual,
                tokenConstant, tokenCloseBracket, tokenOpenBrace, tokenIf, tokenOpenBracket, tokenA,
                tokenGreaterEqual, tokenC, tokenCloseBracket, tokenThen, tokenOpenBrace, tokenD,
                tokenAssign, tokenConstant, tokenSemiColon, tokenCloseBrace, tokenElse, tokenOpenBrace,
                tokenWhile, tokenOpenBracket, tokenE, tokenGreaterEqual, tokenConstant,
                tokenCloseBracket, tokenOpenBrace, tokenB, tokenAssign, tokenConstant, tokenSemiColon,
                tokenCloseBrace, tokenCloseBrace, tokenCloseBrace, tokenCloseBrace,
                tokenElse, tokenOpenBrace, tokenE, tokenAssign, tokenConstant, tokenSemiColon,
                tokenCloseBrace, tokenCloseBrace
            };
            spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
            for (auto token : tokenList) {
                tokenStream.pushBack(token);
            }
            std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
            auto parser = spa::SpParser(tokenStream);
            std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();
            Assert::IsTrue(procedureList.size() == 1);

            spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
            designExtractor.extractRelationship();

            std::vector<std::pair<int, std::string>> expectedIf = { {1, "a"}, {1, "b"}, {3, "a"}, {3, "c"} };

            spa::PKBQueryArg firstArgIf = spa::PKBQueryArg(
                spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
            spa::QueryResult testIf = pkbManager->getContainerPattern(spa::IF, firstArgIf);

            Assert::IsTrue(checkVectorEquals(expectedIf, testIf.getLineNumberNamePairs()));

            std::vector<std::pair<int, std::string>> expectedWhile = { {2, "c"}, {5, "e"}};
            spa::PKBQueryArg firstArgWhile = spa::PKBQueryArg(
                spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
            spa::QueryResult testWhile = pkbManager->getContainerPattern(spa::WHILE, firstArgWhile);

            Assert::IsTrue(expectedWhile == testWhile.getLineNumberNamePairs());
        }
    };
}  // namespace UnitTesting
