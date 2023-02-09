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
TEST_CLASS(TestDesignExtractorFollows) {
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
  TEST_METHOD(TestExtractSingleFollowsWithNoNesting) {
    /*
     *   procedure a {
     * 1. b = 1;
     * 2. c = b;
     *  }
     */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenB, tokenAssign, tokenConstant, tokenSemiColon,
      tokenC, tokenAssign, tokenB, tokenSemiColon,
      tokenCloseBrace
    };
    spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
    for (auto token : tokenList) {
      tokenStream.pushBack(token);
    }

    spa::PKBManager* pkbManager = new spa::PKB();

    auto parser = spa::SpParser(tokenStream);
    std::vector<spa::ProcedureStatement> procedureList = parser.parse();
    Assert::IsTrue(procedureList.size() == 1);

    spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
    designExtractor.extractRelationship();

    spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, "1", {});
    spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LINE_NO, "2", {});
    spa::QueryResult firstRes = pkbManager->getRelationship(spa::FOLLOWS,
      spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));

    Assert::IsTrue(firstRes.getQueryResultType() == spa::BOOL);

    bool testFirstFollows = firstRes.getIsTrue();

    bool expectedFirstFollows = true;

    Assert::IsTrue(expectedFirstFollows == testFirstFollows);
  }

  TEST_METHOD(TestExtractMultipleFollowsWithNoNesting) {
    /*
     *   procedure a {
     * 1. b = 1;
     * 2. c = b;
     * 3. d = b - c;
     *  }
     */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenB, tokenAssign, tokenConstant, tokenSemiColon,
      tokenC, tokenAssign, tokenB, tokenSemiColon,
      tokenD, tokenAssign, tokenB, tokenMinusOp, tokenC, tokenSemiColon,
      tokenCloseBrace
    };
    spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
    for (auto token : tokenList) {
      tokenStream.pushBack(token);
    }

    spa::PKBManager* pkbManager = new spa::PKB();

    auto parser = spa::SpParser(tokenStream);
    std::vector<spa::ProcedureStatement> procedureList = parser.parse();
    Assert::IsTrue(procedureList.size() == 1);

    spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
    designExtractor.extractRelationship();

    spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, "1", {});
    spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LINE_NO, "2", {});
    spa::QueryResult firstRes = pkbManager->getRelationship(spa::FOLLOWS,
      spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));

    pqlArgOne = spa::PqlArgument(spa::LINE_NO, "2", {});
    pqlArgTwo = spa::PqlArgument(spa::LINE_NO, "3", {});
    spa::QueryResult secondRes = pkbManager->getRelationship(spa::FOLLOWS,
      spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));

    pqlArgOne = spa::PqlArgument(spa::LINE_NO, "1", {});
    pqlArgTwo = spa::PqlArgument(spa::LINE_NO, "3", {});
    spa::QueryResult thirdRes = pkbManager->getRelationship(spa::FOLLOWS,
      spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));

    Assert::IsTrue(firstRes.getQueryResultType() == spa::BOOL);
    Assert::IsTrue(secondRes.getQueryResultType() == spa::BOOL);
    Assert::IsTrue(secondRes.getQueryResultType() == spa::BOOL);

    bool testFirstFollows = firstRes.getIsTrue();
    bool testSecondFollows = secondRes.getIsTrue();
    bool testThirdFollows = thirdRes.getIsTrue();

    bool expectedFirstFollows = true;
    bool expectedSecondFollows = true;
    bool expectedThirdFollows = false;

    Assert::IsTrue(expectedFirstFollows == testFirstFollows);
    Assert::IsTrue(expectedSecondFollows == testSecondFollows);
    Assert::IsTrue(expectedThirdFollows == testThirdFollows);
  }

  TEST_METHOD(TestExtractFollowsWithBeforeWhileNesting) {
    /*
     *   procedure a {
     * 1. b = 1;
     * 2. while (b >= 1) {
     * 3.   d = b;
     * 4.   d = b;
     *    }
     *  }
     */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenB, tokenAssign, tokenConstant, tokenSemiColon,
      tokenWhile, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenOpenBrace,
      tokenD, tokenAssign, tokenB, tokenSemiColon,
      tokenD, tokenAssign, tokenB, tokenSemiColon,
      tokenCloseBrace, tokenCloseBrace
    };
    spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
    for (auto token : tokenList) {
      tokenStream.pushBack(token);
    }

    spa::PKBManager* pkbManager = new spa::PKB();

    auto parser = spa::SpParser(tokenStream);
    std::vector<spa::ProcedureStatement> procedureList = parser.parse();
    Assert::IsTrue(procedureList.size() == 1);

    spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
    designExtractor.extractRelationship();

    spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, "1", {});
    spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LINE_NO, "2", {});
    spa::QueryResult firstRes = pkbManager->getRelationship(spa::FOLLOWS,
      spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));

    pqlArgOne = spa::PqlArgument(spa::LINE_NO, "3", {});
    pqlArgTwo = spa::PqlArgument(spa::LINE_NO, "4", {});
    spa::QueryResult secondRes = pkbManager->getRelationship(spa::FOLLOWS,
      spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));

    pqlArgOne = spa::PqlArgument(spa::LINE_NO, "1", {});
    pqlArgTwo = spa::PqlArgument(spa::LINE_NO, "3", {});
    spa::QueryResult thirdRes = pkbManager->getRelationship(spa::FOLLOWS,
      spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));

    Assert::IsTrue(firstRes.getQueryResultType() == spa::BOOL);
    Assert::IsTrue(secondRes.getQueryResultType() == spa::BOOL);
    Assert::IsTrue(thirdRes.getQueryResultType() == spa::BOOL);

    bool testFirstFollows = firstRes.getIsTrue();
    bool testSecondFollows = secondRes.getIsTrue();
    bool testThirdFollows = thirdRes.getIsTrue();

    bool expectedFirstFollows = true;
    bool expectedSecondFollows = true;
    bool expectedThirdFollows = false;

    Assert::IsTrue(expectedFirstFollows == testFirstFollows);
    Assert::IsTrue(expectedSecondFollows == testSecondFollows);
    Assert::IsTrue(expectedThirdFollows == testThirdFollows);
  }

  TEST_METHOD(TestExtractFollowsWithAfterWhileNesting) {
    /*
     *   procedure a {
     * 1. while (b >= 1) {
     * 2.   d = b;
     * 3.   d = b;
     *    }
     * 4. b = 1;
     *  }
     */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenWhile, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenOpenBrace,
      tokenD, tokenAssign, tokenB, tokenSemiColon,
      tokenD, tokenAssign, tokenB, tokenSemiColon,
      tokenCloseBrace,
      tokenB, tokenAssign, tokenConstant, tokenSemiColon,
      tokenCloseBrace
    };
    spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
    for (auto token : tokenList) {
      tokenStream.pushBack(token);
    }

    spa::PKBManager* pkbManager = new spa::PKB();

    auto parser = spa::SpParser(tokenStream);
    std::vector<spa::ProcedureStatement> procedureList = parser.parse();
    Assert::IsTrue(procedureList.size() == 1);

    spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
    designExtractor.extractRelationship();

    spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, "1", {});
    spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LINE_NO, "4", {});
    spa::QueryResult firstRes = pkbManager->getRelationship(spa::FOLLOWS,
      spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));

    pqlArgOne = spa::PqlArgument(spa::LINE_NO, "2", {});
    pqlArgTwo = spa::PqlArgument(spa::LINE_NO, "3", {});
    spa::QueryResult secondRes = pkbManager->getRelationship(spa::FOLLOWS,
      spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));

    pqlArgOne = spa::PqlArgument(spa::LINE_NO, "1", {});
    pqlArgTwo = spa::PqlArgument(spa::LINE_NO, "3", {});
    spa::QueryResult thirdRes = pkbManager->getRelationship(spa::FOLLOWS,
      spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));

    pqlArgOne = spa::PqlArgument(spa::LINE_NO, "3", {});
    pqlArgTwo = spa::PqlArgument(spa::LINE_NO, "4", {});
    spa::QueryResult FourthRes = pkbManager->getRelationship(spa::FOLLOWS,
      spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));

    Assert::IsTrue(firstRes.getQueryResultType() == spa::BOOL);
    Assert::IsTrue(secondRes.getQueryResultType() == spa::BOOL);
    Assert::IsTrue(thirdRes.getQueryResultType() == spa::BOOL);
    Assert::IsTrue(FourthRes.getQueryResultType() == spa::BOOL);

    bool testFirstFollows = firstRes.getIsTrue();
    bool testSecondFollows = secondRes.getIsTrue();
    bool testThirdFollows = thirdRes.getIsTrue();
    bool testFourthFollows = FourthRes.getIsTrue();

    bool expectedFirstFollows = true;
    bool expectedSecondFollows = true;
    bool expectedThirdFollows = false;
    bool expectedFourthFollows = false;

    Assert::IsTrue(expectedFirstFollows == testFirstFollows);
    Assert::IsTrue(expectedSecondFollows == testSecondFollows);
    Assert::IsTrue(expectedThirdFollows == testThirdFollows);
    Assert::IsTrue(expectedFourthFollows == testFourthFollows);
  }

  TEST_METHOD(TestExtractFollowsWithBeforeIfNesting) {
    /*
     *   procedure a {
     * 1. b = 1;
     * 2. if (b >= 1) then {
     * 3.   d = b;
     * 4.   d = b;
     *    } else {
     * 5.   d = b;
     * 6.   d = b;
     *    }
     *  }
     */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenB, tokenAssign, tokenConstant, tokenSemiColon,
      tokenIf, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenThen, tokenOpenBrace,
      tokenD, tokenAssign, tokenB, tokenSemiColon,
      tokenD, tokenAssign, tokenB, tokenSemiColon,
      tokenCloseBrace, tokenElse, tokenOpenBrace,
      tokenD, tokenAssign, tokenB, tokenSemiColon,
      tokenD, tokenAssign, tokenB, tokenSemiColon,
      tokenCloseBrace, tokenCloseBrace
    };
    spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
    for (auto token : tokenList) {
      tokenStream.pushBack(token);
    }

    spa::PKBManager* pkbManager = new spa::PKB();

    auto parser = spa::SpParser(tokenStream);
    std::vector<spa::ProcedureStatement> procedureList = parser.parse();
    Assert::IsTrue(procedureList.size() == 1);

    spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
    designExtractor.extractRelationship();

    spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, "1", {});
    spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LINE_NO, "2", {});
    spa::QueryResult firstRes = pkbManager->getRelationship(spa::FOLLOWS,
      spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));

    pqlArgOne = spa::PqlArgument(spa::LINE_NO, "3", {});
    pqlArgTwo = spa::PqlArgument(spa::LINE_NO, "4", {});
    spa::QueryResult secondRes = pkbManager->getRelationship(spa::FOLLOWS,
      spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));

    pqlArgOne = spa::PqlArgument(spa::LINE_NO, "5", {});
    pqlArgTwo = spa::PqlArgument(spa::LINE_NO, "6", {});
    spa::QueryResult thirdRes = pkbManager->getRelationship(spa::FOLLOWS,
      spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));

    pqlArgOne = spa::PqlArgument(spa::LINE_NO, "1", {});
    pqlArgTwo = spa::PqlArgument(spa::LINE_NO, "3", {});
    spa::QueryResult fourthRes = pkbManager->getRelationship(spa::FOLLOWS,
      spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));

    pqlArgOne = spa::PqlArgument(spa::LINE_NO, "1", {});
    pqlArgTwo = spa::PqlArgument(spa::LINE_NO, "5", {});
    spa::QueryResult fifthRes = pkbManager->getRelationship(spa::FOLLOWS,
      spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));

    pqlArgOne = spa::PqlArgument(spa::LINE_NO, "3", {});
    pqlArgTwo = spa::PqlArgument(spa::LINE_NO, "5", {});
    spa::QueryResult sixthRes = pkbManager->getRelationship(spa::FOLLOWS,
      spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));

    Assert::IsTrue(firstRes.getQueryResultType() == spa::BOOL);
    Assert::IsTrue(secondRes.getQueryResultType() == spa::BOOL);
    Assert::IsTrue(thirdRes.getQueryResultType() == spa::BOOL);
    Assert::IsTrue(fourthRes.getQueryResultType() == spa::BOOL);
    Assert::IsTrue(fifthRes.getQueryResultType() == spa::BOOL);
    Assert::IsTrue(sixthRes.getQueryResultType() == spa::BOOL);

    bool testFirstFollows = firstRes.getIsTrue();
    bool testSecondFollows = secondRes.getIsTrue();
    bool testThirdFollows = thirdRes.getIsTrue();
    bool testFourthFollows = fourthRes.getIsTrue();
    bool testFifthFollows = fifthRes.getIsTrue();
    bool testSixthFollows = sixthRes.getIsTrue();

    bool expectedFirstFollows = true;
    bool expectedSecondFollows = true;
    bool expectedThirdFollows = true;
    bool expectedFourthFollows = false;
    bool expectedFifthFollows = false;
    bool expectedSixthFollows = false;

    Assert::IsTrue(expectedFirstFollows == testFirstFollows);
    Assert::IsTrue(expectedSecondFollows == testSecondFollows);
    Assert::IsTrue(expectedThirdFollows == testThirdFollows);
    Assert::IsTrue(expectedFourthFollows == testFourthFollows);
    Assert::IsTrue(expectedFifthFollows == testFifthFollows);
    Assert::IsTrue(expectedSixthFollows == testSixthFollows);
  }

  TEST_METHOD(TestExtractFollowsWithAfterIfNesting) {
    /*
     *   procedure a {
     * 1. if (b >= 1) then {
     * 2.   d = b;
     * 3.   d = b;
     *    } else {
     * 4.   d = b;
     * 5.   d = b;
     *    }
     * 6. b = 1;
     *  }
     */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenIf, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenThen, tokenOpenBrace,
      tokenD, tokenAssign, tokenB, tokenSemiColon,
      tokenD, tokenAssign, tokenB, tokenSemiColon,
      tokenCloseBrace, tokenElse, tokenOpenBrace,
      tokenD, tokenAssign, tokenB, tokenSemiColon,
      tokenD, tokenAssign, tokenB, tokenSemiColon,
      tokenCloseBrace,
      tokenB, tokenAssign, tokenConstant, tokenSemiColon,
      tokenCloseBrace
    };
    spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
    for (auto token : tokenList) {
      tokenStream.pushBack(token);
    }

    spa::PKBManager* pkbManager = new spa::PKB();

    auto parser = spa::SpParser(tokenStream);
    std::vector<spa::ProcedureStatement> procedureList = parser.parse();
    Assert::IsTrue(procedureList.size() == 1);

    spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
    designExtractor.extractRelationship();

    spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, "1", {});
    spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LINE_NO, "6", {});
    spa::QueryResult firstRes = pkbManager->getRelationship(spa::FOLLOWS,
      spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));

    pqlArgOne = spa::PqlArgument(spa::LINE_NO, "2", {});
    pqlArgTwo = spa::PqlArgument(spa::LINE_NO, "3", {});
    spa::QueryResult secondRes = pkbManager->getRelationship(spa::FOLLOWS,
      spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));

    pqlArgOne = spa::PqlArgument(spa::LINE_NO, "4", {});
    pqlArgTwo = spa::PqlArgument(spa::LINE_NO, "5", {});
    spa::QueryResult thirdRes = pkbManager->getRelationship(spa::FOLLOWS,
      spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));

    pqlArgOne = spa::PqlArgument(spa::LINE_NO, "1", {});
    pqlArgTwo = spa::PqlArgument(spa::LINE_NO, "2", {});
    spa::QueryResult fourthRes = pkbManager->getRelationship(spa::FOLLOWS,
      spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));

    pqlArgOne = spa::PqlArgument(spa::LINE_NO, "1", {});
    pqlArgTwo = spa::PqlArgument(spa::LINE_NO, "4", {});
    spa::QueryResult fifthRes = pkbManager->getRelationship(spa::FOLLOWS,
      spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));

    pqlArgOne = spa::PqlArgument(spa::LINE_NO, "2", {});
    pqlArgTwo = spa::PqlArgument(spa::LINE_NO, "6", {});
    spa::QueryResult sixthRes = pkbManager->getRelationship(spa::FOLLOWS,
      spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));

    pqlArgOne = spa::PqlArgument(spa::LINE_NO, "5", {});
    pqlArgTwo = spa::PqlArgument(spa::LINE_NO, "6", {});
    spa::QueryResult seventhRes = pkbManager->getRelationship(spa::FOLLOWS,
      spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));

    Assert::IsTrue(firstRes.getQueryResultType() == spa::BOOL);
    Assert::IsTrue(secondRes.getQueryResultType() == spa::BOOL);
    Assert::IsTrue(thirdRes.getQueryResultType() == spa::BOOL);
    Assert::IsTrue(fourthRes.getQueryResultType() == spa::BOOL);
    Assert::IsTrue(fifthRes.getQueryResultType() == spa::BOOL);
    Assert::IsTrue(sixthRes.getQueryResultType() == spa::BOOL);
    Assert::IsTrue(seventhRes.getQueryResultType() == spa::BOOL);

    bool testFirstFollows = firstRes.getIsTrue();
    bool testSecondFollows = secondRes.getIsTrue();
    bool testThirdFollows = thirdRes.getIsTrue();
    bool testFourthFollows = fourthRes.getIsTrue();
    bool testFifthFollows = fifthRes.getIsTrue();
    bool testSixthFollows = sixthRes.getIsTrue();
    bool testSeventhFollows = seventhRes.getIsTrue();

    bool expectedFirstFollows = true;
    bool expectedSecondFollows = true;
    bool expectedThirdFollows = true;
    bool expectedFourthFollows = false;
    bool expectedFifthFollows = false;
    bool expectedSixthFollows = false;
    bool expectedSeventhFollows = false;

    Assert::IsTrue(expectedFirstFollows == testFirstFollows);
    Assert::IsTrue(expectedSecondFollows == testSecondFollows);
    Assert::IsTrue(expectedThirdFollows == testThirdFollows);
    Assert::IsTrue(expectedFourthFollows == testFourthFollows);
    Assert::IsTrue(expectedFifthFollows == testFifthFollows);
    Assert::IsTrue(expectedSixthFollows == testSixthFollows);
    Assert::IsTrue(expectedSeventhFollows == testSeventhFollows);
  }
};
}  // namespace UnitTesting
