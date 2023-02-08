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
  TEST_METHOD(TestExtractModifiesSingleAssignment) {
    ///*
    // *  procedure a {
    // * 1. c = d;
    // *  }
    // */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenC, tokenAssign, tokenD, tokenSemiColon,
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

    spa::PqlArgument lineNum(spa::ArgumentType::LINE_NO, "1", {});
    spa::PqlArgument variable(spa::ArgumentType::VARIABLE_NAME, varC, {});
    spa::QueryResult modifiesRes = pkbManager->getRelationship(spa::MODIFIES,
      spa::PKBQueryArg(lineNum),
      spa::PKBQueryArg(variable));

    Assert::IsTrue(modifiesRes.getQueryResultType() == spa::BOOL);

    bool testModifies = modifiesRes.getIsTrue();

    bool expectedModifies = true;

    Assert::IsTrue(expectedModifies == testModifies);
  }

  TEST_METHOD(TestExtractModifiesIfNestedAssignment) {
    ///*
    // *  procedure a {
    // * 1. if (e >= 1) then {
    // * 2.   c = d;
    // *    } else {
    // * 3.   b = d;
    // *    }
    // *  }
    // */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenIf, tokenOpenBracket, tokenE, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenThen, tokenOpenBrace,
      tokenC, tokenAssign, tokenD, tokenSemiColon,
      tokenCloseBrace,
      tokenElse, tokenOpenBrace,
      tokenB, tokenAssign, tokenD, tokenSemiColon,
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

    spa::PqlArgument lineNum(spa::ArgumentType::LINE_NO, "1", {});
    spa::PqlArgument variable(spa::ArgumentType::VARIABLE_NAME, varC, {});
    spa::QueryResult firstModifiesRes = pkbManager->getRelationship(spa::MODIFIES,
      spa::PKBQueryArg(lineNum),
      spa::PKBQueryArg(variable));

    variable = spa::PqlArgument(spa::ArgumentType::VARIABLE_NAME, varB, {});
    spa::QueryResult secondModifiesRes = pkbManager->getRelationship(spa::MODIFIES,
      spa::PKBQueryArg(lineNum),
      spa::PKBQueryArg(variable));

    lineNum = spa::PqlArgument(spa::ArgumentType::LINE_NO, "2", {});
    variable = spa::PqlArgument(spa::ArgumentType::VARIABLE_NAME, varC, {});
    spa::QueryResult thenModifiesRes = pkbManager->getRelationship(spa::MODIFIES,
      spa::PKBQueryArg(lineNum),
      spa::PKBQueryArg(variable));

    lineNum = spa::PqlArgument(spa::ArgumentType::LINE_NO, "3", {});
    variable = spa::PqlArgument(spa::ArgumentType::VARIABLE_NAME, varB, {});
    spa::QueryResult elseModifiesRes = pkbManager->getRelationship(spa::MODIFIES,
      spa::PKBQueryArg(lineNum),
      spa::PKBQueryArg(variable));

    bool testFirstModifies = firstModifiesRes.getIsTrue();
    bool testSecondModifies = secondModifiesRes.getIsTrue();
    bool testThenModifies = thenModifiesRes.getIsTrue();
    bool testElseModifies = elseModifiesRes.getIsTrue();

    bool expectedFirstModifies = true;
    bool expectedSecondModifies = true;
    bool expectedThenModifies = true;
    bool expectedElseModifies = true;

    Assert::IsTrue(firstModifiesRes.getQueryResultType() == spa::BOOL);
    Assert::IsTrue(secondModifiesRes.getQueryResultType() == spa::BOOL);
    Assert::IsTrue(thenModifiesRes.getQueryResultType() == spa::BOOL);
    Assert::IsTrue(elseModifiesRes.getQueryResultType() == spa::BOOL);

    Assert::IsTrue(expectedFirstModifies == testFirstModifies);
    Assert::IsTrue(expectedSecondModifies == testSecondModifies);
    Assert::IsTrue(expectedThenModifies == testThenModifies);
    Assert::IsTrue(expectedElseModifies == testElseModifies);
  }

  TEST_METHOD(TestExtractModifiesWhileNestedAssignment) {
    ///*
    // *  procedure a {
    // * 1. while (e >= 1) {
    // * 2.   c = d;
    // * 3.   b = d;
    // *    }
    // *  }
    // */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenWhile, tokenOpenBracket, tokenE, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenOpenBrace,
      tokenC, tokenAssign, tokenD, tokenSemiColon,
      tokenB, tokenAssign, tokenD, tokenSemiColon,
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

    spa::PqlArgument lineNum(spa::ArgumentType::LINE_NO, "1", {});
    spa::PqlArgument variable(spa::ArgumentType::VARIABLE_NAME, varC, {});
    spa::QueryResult firstModifiesRes = pkbManager->getRelationship(spa::MODIFIES,
      spa::PKBQueryArg(lineNum),
      spa::PKBQueryArg(variable));

    variable = spa::PqlArgument(spa::ArgumentType::VARIABLE_NAME, varB, {});
    spa::QueryResult secondModifiesRes = pkbManager->getRelationship(spa::MODIFIES,
      spa::PKBQueryArg(lineNum),
      spa::PKBQueryArg(variable));

    lineNum = spa::PqlArgument(spa::ArgumentType::LINE_NO, "2", {});
    variable = spa::PqlArgument(spa::ArgumentType::VARIABLE_NAME, varC, {});
    spa::QueryResult fourthModifiesRes = pkbManager->getRelationship(spa::MODIFIES,
      spa::PKBQueryArg(lineNum),
      spa::PKBQueryArg(variable));

    lineNum = spa::PqlArgument(spa::ArgumentType::LINE_NO, "3", {});
    variable = spa::PqlArgument(spa::ArgumentType::VARIABLE_NAME, varB, {});
    spa::QueryResult fifthModifiesRes = pkbManager->getRelationship(spa::MODIFIES,
      spa::PKBQueryArg(lineNum),
      spa::PKBQueryArg(variable));

    bool testFirstModifies = firstModifiesRes.getIsTrue();
    bool testSecondModifies = secondModifiesRes.getIsTrue();
    bool testFourthModifies = fourthModifiesRes.getIsTrue();
    bool testFifthModifies = fifthModifiesRes.getIsTrue();

    bool expectedFirstModifies = true;
    bool expectedSecondModifies = true;
    bool expectedFourthModifies = true;
    bool expectedFifthModifies = true;

    Assert::IsTrue(firstModifiesRes.getQueryResultType() == spa::BOOL);
    Assert::IsTrue(secondModifiesRes.getQueryResultType() == spa::BOOL);
    Assert::IsTrue(fourthModifiesRes.getQueryResultType() == spa::BOOL);
    Assert::IsTrue(fifthModifiesRes.getQueryResultType() == spa::BOOL);

    Assert::IsTrue(expectedFirstModifies == testFirstModifies);
    Assert::IsTrue(expectedSecondModifies == testSecondModifies);
    Assert::IsTrue(expectedFourthModifies == testFourthModifies);
    Assert::IsTrue(expectedFifthModifies == testFifthModifies);
  }
};
}  // namespace UnitTesting
