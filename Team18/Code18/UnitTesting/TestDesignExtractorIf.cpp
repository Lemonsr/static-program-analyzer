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
            tokenE, tokenAssign, tokenF,tokenSemiColon, tokenCloseBrace, tokenCloseBrace
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

        std::vector<std::pair<int, std::string>> expectedIf = { {1, "c"}, {1, "d"} };
        spa::PKBQueryArg firstArg = spa::PKBQueryArg(
            spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
        spa::QueryResult testIf = pkbManager->getContainerPattern(spa::IF, firstArg);

        Assert::IsTrue(expectedIf == testIf.getLineNumberVariablePairs());
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
            tokenE, tokenAssign, tokenF,tokenSemiColon, tokenCloseBrace, tokenCloseBrace
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

        std::vector<std::pair<int, std::string>> negIf = { {1, "a"}, {1, "d"} };

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(
            spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
        spa::QueryResult testIf = pkbManager->getContainerPattern(spa::IF, firstArg);

        Assert::IsFalse(negIf == testIf.getLineNumberVariablePairs());
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
            tokenE, tokenAssign, tokenF,tokenSemiColon, tokenCloseBrace,
            tokenIf, tokenOpenBracket, tokenD, tokenGreaterEqual, tokenConstant, tokenCloseBracket,
            tokenThen, tokenOpenBrace,
            tokenF, tokenAssign, tokenE, tokenSemiColon, tokenCloseBrace,
            tokenElse, tokenOpenBrace,
            tokenA, tokenAssign, tokenB, tokenSemiColon,
            tokenCloseBrace,tokenCloseBrace
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

        std::vector<std::pair<int, std::string>> expectedIf = { {1, "c"}, {1, "d"}, {4, "d"}};
        spa::PKBQueryArg firstArg = spa::PKBQueryArg(
            spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
        spa::QueryResult testIf = pkbManager->getContainerPattern(spa::IF, firstArg);

        Assert::IsTrue(expectedIf == testIf.getLineNumberVariablePairs());
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
            tokenE, tokenAssign, tokenF,tokenSemiColon, tokenCloseBrace,
            tokenIf, tokenOpenBracket, tokenD, tokenGreaterEqual, tokenConstant, tokenCloseBracket,
            tokenThen, tokenOpenBrace,
            tokenF, tokenAssign, tokenE, tokenSemiColon, tokenCloseBrace,
            tokenElse, tokenOpenBrace,
            tokenA, tokenAssign, tokenB, tokenSemiColon,
            tokenCloseBrace,tokenCloseBrace
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

        std::vector<std::pair<int, std::string>> expectedIf = { {1, "a"}, {1, "d"}, {3, "e"}, {3, "a"}, {3, "b"} };
        spa::PKBQueryArg firstArg = spa::PKBQueryArg(
            spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
        spa::QueryResult testIf = pkbManager->getContainerPattern(spa::IF, firstArg);

        Assert::IsFalse(expectedIf == testIf.getLineNumberVariablePairs());
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
            tokenThen, tokenOpenBrace, tokenF, tokenAssign, tokenE, tokenSemiColon, tokenCloseBrace,
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
        std::vector<spa::ProcedureStatement> procedureList = parser.parse();
        Assert::IsTrue(procedureList.size() == 1);

        spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
        designExtractor.extractRelationship();

        std::vector<std::pair<int, std::string>> expectedIf = { {1, "c"}, {1, "d"}, {3, "a"}, {3, "e"}};
        spa::PKBQueryArg firstArg = spa::PKBQueryArg(
            spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
        spa::QueryResult testIf = pkbManager->getContainerPattern(spa::IF, firstArg);

        Assert::IsTrue(expectedIf == testIf.getLineNumberVariablePairs());
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
            tokenThen, tokenOpenBrace, tokenF, tokenAssign, tokenE, tokenSemiColon, tokenCloseBrace,
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
        std::vector<spa::ProcedureStatement> procedureList = parser.parse();
        Assert::IsTrue(procedureList.size() == 1);

        spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
        designExtractor.extractRelationship();

        std::vector<std::pair<int, std::string>> negIf = { {1, "b"}, {3, "b"} };
        spa::PKBQueryArg firstArg = spa::PKBQueryArg(
            spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
        spa::QueryResult testIf = pkbManager->getContainerPattern(spa::IF, firstArg);

        Assert::IsFalse(negIf == testIf.getLineNumberVariablePairs());
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
        std::vector<spa::ProcedureStatement> procedureList = parser.parse();
        Assert::IsTrue(procedureList.size() == 1);

        spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
        designExtractor.extractRelationship();

        std::vector<std::pair<int, std::string>> expectedIf = { {1, "c"}, {1, "d"}, {3, "a"}, {3, "e"}, {5, "e"}, {5, "g"}};
        spa::PKBQueryArg firstArg = spa::PKBQueryArg(
            spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
        spa::QueryResult testIf = pkbManager->getContainerPattern(spa::IF, firstArg);

        Assert::IsTrue(expectedIf == testIf.getLineNumberVariablePairs());
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
        std::vector<spa::ProcedureStatement> procedureList = parser.parse();
        Assert::IsTrue(procedureList.size() == 1);

        spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
        designExtractor.extractRelationship();

        std::vector<std::pair<int, std::string>> expectedWhile = { {2, "b"}, {2, "e"}, {3, "b"}, {3, "c"}, {3, "d"} };
        spa::PKBQueryArg firstArg = spa::PKBQueryArg(
            spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
        spa::QueryResult testWhile = pkbManager->getContainerPattern(spa::IF, firstArg);

        Assert::IsFalse(expectedWhile == testWhile.getLineNumberVariablePairs());
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
            tokenIf, tokenOpenBracket, tokenG, tokenGreaterEqual, tokenE, tokenCloseBracket, tokenThen, tokenOpenBrace,
            tokenB, tokenAssign, tokenB, tokenSemiColon,
            tokenIf, tokenOpenBracket, tokenA, tokenGreaterEqual, tokenB,tokenCloseBracket, tokenThen, tokenOpenBrace,
            tokenB, tokenAssign, tokenC, tokenSemiColon, tokenCloseBrace,
            tokenElse, tokenOpenBrace,
            tokenA, tokenAssign, tokenF, tokenSemiColon, tokenCloseBrace, tokenCloseBrace,
            tokenElse, tokenOpenBrace,
            tokenB, tokenAssign, tokenC, tokenSemiColon, tokenCloseBrace,
            tokenCloseBrace, tokenElse, tokenOpenBrace,
            tokenA, tokenAssign, tokenF, tokenSemiColon, tokenCloseBrace,
            tokenCloseBrace,tokenElse, tokenOpenBrace,
            tokenE, tokenAssign, tokenF, tokenSemiColon, tokenCloseBrace,
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

        std::vector<std::pair<int, std::string>> expectedIf = { {1, "c"}, {1, "d"}, {3, "a"}, {3, "e"}, {5, "e"}, {5, "g"}, {7, "a"}, {7, "b"} };
        spa::PKBQueryArg firstArg = spa::PKBQueryArg(
            spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
        spa::QueryResult testIf = pkbManager->getContainerPattern(spa::IF, firstArg);

        Assert::IsTrue(expectedIf == testIf.getLineNumberVariablePairs());
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
            tokenIf, tokenOpenBracket, tokenG, tokenGreaterEqual, tokenE, tokenCloseBracket, tokenThen, tokenOpenBrace,
            tokenB, tokenAssign, tokenB, tokenSemiColon,
            tokenIf, tokenOpenBracket, tokenA, tokenGreaterEqual, tokenB,tokenCloseBracket, tokenThen, tokenOpenBrace,
            tokenB, tokenAssign, tokenC, tokenSemiColon, tokenCloseBrace,
            tokenElse, tokenOpenBrace,
            tokenA, tokenAssign, tokenF, tokenSemiColon, tokenCloseBrace, tokenCloseBrace,
            tokenElse, tokenOpenBrace,
            tokenB, tokenAssign, tokenC, tokenSemiColon, tokenCloseBrace,
            tokenCloseBrace, tokenElse, tokenOpenBrace,
            tokenA, tokenAssign, tokenF, tokenSemiColon, tokenCloseBrace,
            tokenCloseBrace,tokenElse, tokenOpenBrace,
            tokenE, tokenAssign, tokenF, tokenSemiColon, tokenCloseBrace,
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

        std::vector<std::pair<int, std::string>> negIf = { {1, "c"}, {1, "d"}, {3, "e"}, {3, "a"}, {5, "g"}, {5, "e"}, {7, "a"}, {7, "b"} };
        spa::PKBQueryArg firstArg = spa::PKBQueryArg(
            spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
        spa::QueryResult testIf = pkbManager->getContainerPattern(spa::IF, firstArg);

        Assert::IsFalse(negIf == testIf.getLineNumberVariablePairs());
    }
    };
}
