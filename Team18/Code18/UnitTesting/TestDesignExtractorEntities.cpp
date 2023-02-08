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
TEST_CLASS(TestDesignExtractor) {
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
  TEST_METHOD(TestExtractEntitiesWithNoNesting) {
    ///*
    // *   procedure a {
    // * 1. b = 1;
    // * 2. c = b;
    // * 3. c = d - b;
    // *  }
    // */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenB, tokenAssign, tokenConstant, tokenSemiColon,
      tokenC, tokenAssign, tokenB, tokenSemiColon,
      tokenC, tokenAssign, tokenD, tokenMinusOp, tokenB, tokenSemiColon,
      tokenCloseBrace
    };
    spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
    for (auto& token : tokenList) {
      tokenStream.pushBack(token);
    }
    spa::PKBManager* pkbManager = new spa::PKB();
    auto parser = spa::SpParser(tokenStream);
    std::vector<spa::ProcedureStatement> procedureList = parser.parse();
    Assert::IsTrue(procedureList.size() == 1);

    spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
    designExtractor.extractRelationship();

    spa::QueryResult procedureRes = pkbManager->getEntity(spa::PROCEDURE);
    spa::QueryResult variablesRes = pkbManager->getEntity(spa::VARIABLE);
    spa::QueryResult constantRes = pkbManager->getEntity(spa::CONSTANT);

    std::optional<std::vector<std::string>> testProcedure = procedureRes.getNames();
    std::optional<std::vector<std::string>> testVariable = variablesRes.getNames();
    std::optional<std::vector<std::string>> testConstant = constantRes.getNames();

    std::optional<std::vector<std::string>> expectedProcedure = {{varA}};
    std::optional<std::vector<std::string>> expectedVariable = {{varB, varC, varD}};
    std::optional<std::vector<std::string>> expectedConstant = {{constant}};

    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedProcedure, testProcedure));
    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedVariable,
      testVariable));
    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedConstant,
      testConstant));
  }

  TEST_METHOD(TestExtractEntitiesWithWhileNesting) {
    ///*
    // *  procedure a {
    // * 1. while (d >= 1) {
    // * 2.   b = 1;
    // * 3.   c = b;
    // *    }
    // *  }
    // */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenWhile, tokenOpenBracket, tokenD, tokenGreaterEqual, tokenConstant, tokenCloseBracket,
      tokenOpenBrace, tokenB, tokenAssign, tokenConstant, tokenSemiColon,
      tokenC, tokenAssign, tokenB, tokenSemiColon,
      tokenCloseBrace, tokenCloseBrace
    };
    spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
    for (auto& token : tokenList) {
      tokenStream.pushBack(token);
    }
    spa::PKBManager* pkbManager = new spa::PKB();
    auto parser = spa::SpParser(tokenStream);
    std::vector<spa::ProcedureStatement> procedureList = parser.parse();
    Assert::IsTrue(procedureList.size() == 1);

    spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
    designExtractor.extractRelationship();

    spa::QueryResult procedureRes = pkbManager->getEntity(spa::PROCEDURE);
    spa::QueryResult variablesRes = pkbManager->getEntity(spa::VARIABLE);
    spa::QueryResult constantRes = pkbManager->getEntity(spa::CONSTANT);

    std::optional<std::vector<std::string>> testProcedure = procedureRes.getNames();
    std::optional<std::vector<std::string>> testVariable = variablesRes.getNames();
    std::optional<std::vector<std::string>> testConstant = constantRes.getNames();

    std::optional<std::vector<std::string>> expectedProcedure = {{varA}};
    std::optional<std::vector<std::string>> expectedVariable = {{varB, varC, varD}};
    std::optional<std::vector<std::string>> expectedConstant = {{constant}};

    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedProcedure,
      testProcedure));
    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedVariable,
      testVariable));
    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedConstant,
      testConstant));
  }

  TEST_METHOD(TestExtractEntitiesWithIfNesting) {
    ///*
    // *  procedure a {
    // * 1. if (b >= 1) then {
    // * 2.   b = 1;
    // * 3.   c = b;
    // *    }
    // *    else {
    // * 4.   d = b;
    // *    }
    // *  }
    // */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenIf, tokenOpenBracket, tokenD, tokenGreaterEqual, tokenConstant, tokenCloseBracket,
      tokenThen, tokenOpenBrace, tokenB, tokenAssign, tokenConstant, tokenSemiColon,
      tokenC, tokenAssign, tokenB, tokenSemiColon,
      tokenCloseBrace, tokenElse, tokenOpenBrace, tokenD, tokenAssign, tokenB, tokenSemiColon,
      tokenCloseBrace,
      tokenCloseBrace
    };
    spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
    for (auto& token : tokenList) {
      tokenStream.pushBack(token);
    }
    spa::PKBManager* pkbManager = new spa::PKB();
    auto parser = spa::SpParser(tokenStream);
    std::vector<spa::ProcedureStatement> procedureList = parser.parse();
    Assert::IsTrue(procedureList.size() == 1);

    spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
    designExtractor.extractRelationship();

    spa::QueryResult procedureRes = pkbManager->getEntity(spa::PROCEDURE);
    spa::QueryResult variablesRes = pkbManager->getEntity(spa::VARIABLE);
    spa::QueryResult constantRes = pkbManager->getEntity(spa::CONSTANT);

    std::optional<std::vector<std::string>> testProcedure = procedureRes.getNames();
    std::optional<std::vector<std::string>> testVariable = variablesRes.getNames();
    std::optional<std::vector<std::string>> testConstant = constantRes.getNames();

    std::optional<std::vector<std::string>> expectedProcedure = {{varA}};
    std::optional<std::vector<std::string>> expectedVariable = {{varB, varC, varD}};
    std::optional<std::vector<std::string>> expectedConstant = {{constant}};

    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedProcedure,
      testProcedure));
    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedVariable,
      testVariable));
    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedConstant,
      testConstant));
  }

  // TEST_METHOD(TestExtractFollowsWithNoNesting) {
  ///*
  // *   procedure a {
  // * 1. b = 1;
  // * 2. c = b;
  // * 3. while (b >= 1) {
  // * 4.   d = b - c;
  // * 5.   c = d - b;
  // *    }
  // *  }
  // */
  // tokenList = {
  //  tokenProcedure, tokenA, tokenOpenBrace,
  //  tokenB, tokenAssign, tokenConstant, tokenSemiColon,
  //  tokenC, tokenAssign, tokenB, tokenSemiColon,
  //  tokenWhile, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenConstant, tokenCloseBracket,
  //  tokenOpenBrace, tokenD, tokenAssign, tokenB, tokenMinusOp, tokenC, tokenSemiColon,
  //  tokenC, tokenAssign, tokenD, tokenMinusOp, tokenB, tokenSemiColon,
  //  tokenCloseBrace, tokenCloseBrace
  // };
  // Stream<Token> tokenStream = Stream<Token>();
  // for (auto token: tokenList) {
  //  tokenStream.pushBack(token);
  // }

  // PKBManager* pkbManager = new PKB();
  // auto parser = SpParser(tokenStream);
  // vector<ProcedureStatement> procedureList = parser.parse();
  // Assert::IsTrue(procedureList.size() == 1);
  // optional<DesignEntityType> designEntity;
  // PqlArgument pqlArgOne = PqlArgument(LINE_NO, "2", designEntity);
  // PqlArgument pqlArgtwo = PqlArgument(LINE_NO, "3", designEntity);
  // QueryResult results = pkbManager->getRelationship(FOLLOWS, PKBQueryArg(pqlArgOne), PKBQueryArg(pqlArgtwo));
  // }
};
}  // namespace UnitTesting
