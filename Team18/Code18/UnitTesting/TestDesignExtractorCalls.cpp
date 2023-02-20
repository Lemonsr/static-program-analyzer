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
    TEST_CLASS(TestDesignExtractorCalls) {
        std::string varA = "a";
        std::string varB = "b";
        std::string varC = "c";
        std::string varD = "d";
        std::string varE = "e";
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
    TEST_METHOD(TestSingleCall) {
        /*
         *   procedure a {
         * 1. b = 1;
         * 2. c = b;
         * 3. call d;
         *  }
         *
         *   procedure d {
         * 4. a = 5;
         *   }
         */
        tokenList = {
          tokenProcedure, tokenA, tokenOpenBrace,
          tokenB, tokenAssign, tokenConstant, tokenSemiColon,
          tokenC, tokenAssign, tokenB, tokenSemiColon,
          tokenCall, tokenD, tokenSemiColon,
          tokenCloseBrace,
          tokenProcedure, tokenD, tokenOpenBrace,
          tokenA, tokenAssign, tokenConstant, tokenSemiColon,
          tokenCloseBrace
        };
        spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
        for (auto token : tokenList) {
            tokenStream.pushBack(token);
        }

        std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();

        auto parser = spa::SpParser(tokenStream);
        std::vector<spa::ProcedureStatement> procedureList = parser.parse();
        Assert::IsTrue(procedureList.size() == 2);

        spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
        designExtractor.extractRelationship();

        spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::PROCEDURE_NAME, "a", {});
        spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::PROCEDURE_NAME, "d", {});
        spa::QueryResult firstRes = pkbManager->getRelationship(spa::CALLS,
            spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));

        Assert::IsTrue(firstRes.getQueryResultType() == spa::BOOL);

        bool testFirstCalls = firstRes.getIsTrue();

        bool expectedFirstCalls = true;

        Assert::IsTrue(expectedFirstCalls == testFirstCalls);
    }

    //TEST_METHOD(TestDoubleCall) {
    //    /*
    //     *   procedure a {
    //     * 1. b = 1;
    //     * 2. c = b;
    //     * 3. call d;
    //     * 4. call e;
    //     *  }
    //     *
    //     *   procedure d {
    //     * 5. a = 5;
    //     *   }
    //     */
    //    tokenList = {
    //      tokenProcedure, tokenA, tokenOpenBrace,
    //      tokenB, tokenAssign, tokenConstant, tokenSemiColon,
    //      tokenC, tokenAssign, tokenB, tokenSemiColon,
    //      tokenCall, tokenD, tokenSemiColon,
    //      tokenCloseBrace,
    //      tokenProcedure, tokenD, tokenOpenBrace,
    //      tokenA, tokenAssign, tokenConstant, tokenSemiColon,
    //      tokenCloseBrace
    //    };
    //    spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
    //    for (auto token : tokenList) {
    //        tokenStream.pushBack(token);
    //    }

    //    spa::PKBManager* pkbManager = new spa::PKB();

    //    auto parser = spa::SpParser(tokenStream);
    //    std::vector<spa::ProcedureStatement> procedureList = parser.parse();
    //    Assert::IsTrue(procedureList.size() == 2);

    //    spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
    //    designExtractor.extractRelationship();


    //    std::vector<std::pair<std::string, std::string>> expected = { {"a", "d"} };

    //    spa::RelationshipStorage relationshipStorage;
    //    spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
    //    spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::PROCEDURE_NAME, "a", {}));
    //    spa::QueryResult queryResult = relationshipStorage.getCallsUnderscoreName(firstArg, secondArg);

    //    Assert::IsTrue(queryResult.getProcNameProcNamePairs() == expected);
    //}
        };
    }