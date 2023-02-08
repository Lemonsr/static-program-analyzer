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
  TEST_METHOD(TestExtractUsesSingleAssignment) {
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
    spa::PqlArgument variable(spa::ArgumentType::VARIABLE_NAME, varD, {});
    spa::QueryResult usesRes = pkbManager->getRelationship(spa::USES, spa::PKBQueryArg(lineNum),
      spa::PKBQueryArg(variable));

    Assert::IsTrue(usesRes.getQueryResultType() == spa::BOOL);

    bool testUses = usesRes.getIsTrue();

    bool expectedUses = true;

    Assert::IsTrue(expectedUses == testUses);
  }

  TEST_METHOD(TestExtractUsesMultipleVarAssignment) {
    ///*
    // *  procedure a {
    // * 1. c = d - b * e;
    // *  }
    // */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenC, tokenAssign, tokenD, tokenMinusOp, tokenB, tokenMultiply, tokenE,
      tokenSemiColon, tokenCloseBrace
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
    spa::PqlArgument variable(spa::ArgumentType::VARIABLE_NAME, varD, {});
    spa::QueryResult firstUsesRes = pkbManager->getRelationship(spa::USES,
      spa::PKBQueryArg(lineNum),
      spa::PKBQueryArg(variable));

    variable = spa::PqlArgument(spa::ArgumentType::VARIABLE_NAME, varB, {});
    spa::QueryResult secondUsesRes = pkbManager->getRelationship(spa::USES,
      spa::PKBQueryArg(lineNum),
      spa::PKBQueryArg(variable));

    variable = spa::PqlArgument(spa::ArgumentType::VARIABLE_NAME, varE, {});
    spa::QueryResult thirdUsesRes = pkbManager->getRelationship(spa::USES,
      spa::PKBQueryArg(lineNum),
      spa::PKBQueryArg(variable));

    bool testFirstUses = firstUsesRes.getIsTrue();
    bool testSecondUses = secondUsesRes.getIsTrue();
    bool testThirdUses = thirdUsesRes.getIsTrue();

    bool expectedFirstUses = true;
    bool expectedSecondUses = true;
    bool expectedThirdUses = true;

    Assert::IsTrue(firstUsesRes.getQueryResultType() == spa::BOOL);
    Assert::IsTrue(secondUsesRes.getQueryResultType() == spa::BOOL);
    Assert::IsTrue(thirdUsesRes.getQueryResultType() == spa::BOOL);

    Assert::IsTrue(expectedFirstUses == testFirstUses);
    Assert::IsTrue(expectedSecondUses == testSecondUses);
    Assert::IsTrue(expectedThirdUses == testThirdUses);
  }

  TEST_METHOD(TestExtractUsesIfNestedAssignment) {
    ///*
    // *  procedure a {
    // * 1. if (e >= 1) then {
    // * 2.   c = d;
    // *    } else {
    // * 3.   c = b;
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
      tokenC, tokenAssign, tokenB, tokenSemiColon,
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
    spa::PqlArgument variable(spa::ArgumentType::VARIABLE_NAME, varD, {});
    spa::QueryResult firstUsesRes = pkbManager->getRelationship(spa::USES,
      spa::PKBQueryArg(lineNum),
      spa::PKBQueryArg(variable));

    variable = spa::PqlArgument(spa::ArgumentType::VARIABLE_NAME, varB, {});
    spa::QueryResult secondUsesRes = pkbManager->getRelationship(spa::USES,
      spa::PKBQueryArg(lineNum),
      spa::PKBQueryArg(variable));

    variable = spa::PqlArgument(spa::ArgumentType::VARIABLE_NAME, varE, {});
    spa::QueryResult thirdUsesRes = pkbManager->getRelationship(spa::USES,
      spa::PKBQueryArg(lineNum),
      spa::PKBQueryArg(variable));

    lineNum = spa::PqlArgument(spa::ArgumentType::LINE_NO, "2", {});
    variable = spa::PqlArgument(spa::ArgumentType::VARIABLE_NAME, varD, {});
    spa::QueryResult thenUsesRes = pkbManager->getRelationship(spa::USES,
      spa::PKBQueryArg(lineNum),
      spa::PKBQueryArg(variable));

    lineNum = spa::PqlArgument(spa::ArgumentType::LINE_NO, "3", {});
    variable = spa::PqlArgument(spa::ArgumentType::VARIABLE_NAME, varB, {});
    spa::QueryResult elseUsesRes = pkbManager->getRelationship(spa::USES,
      spa::PKBQueryArg(lineNum),
      spa::PKBQueryArg(variable));

    bool testFirstUses = firstUsesRes.getIsTrue();
    bool testSecondUses = secondUsesRes.getIsTrue();
    bool testThirdUses = thirdUsesRes.getIsTrue();
    bool testThenUses = thenUsesRes.getIsTrue();
    bool testElseUses = elseUsesRes.getIsTrue();

    bool expectedFirstUses = true;
    bool expectedSecondUses = true;
    bool expectedThirdUses = true;
    bool expectedThenUses = true;
    bool expectedElseUses = true;

    Assert::IsTrue(firstUsesRes.getQueryResultType() == spa::BOOL);
    Assert::IsTrue(secondUsesRes.getQueryResultType() == spa::BOOL);
    Assert::IsTrue(thirdUsesRes.getQueryResultType() == spa::BOOL);
    Assert::IsTrue(thenUsesRes.getQueryResultType() == spa::BOOL);
    Assert::IsTrue(elseUsesRes.getQueryResultType() == spa::BOOL);

    Assert::IsTrue(expectedFirstUses == testFirstUses);
    Assert::IsTrue(expectedSecondUses == testSecondUses);
    Assert::IsTrue(expectedThirdUses == testThirdUses);
    Assert::IsTrue(expectedThenUses == testThenUses);
    Assert::IsTrue(expectedElseUses == testElseUses);
  }

  TEST_METHOD(TestExtractUsesWhileNestedAssignment) {
    ///*
    // *  procedure a {
    // * 1. while (e >= 1) {
    // * 2.   c = d;
    // * 3.   c = b;
    // *    }
    // *  }
    // */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenWhile, tokenOpenBracket, tokenE, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenOpenBrace,
      tokenC, tokenAssign, tokenD, tokenSemiColon,
      tokenC, tokenAssign, tokenB, tokenSemiColon,
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
    spa::PqlArgument variable(spa::ArgumentType::VARIABLE_NAME, varD, {});
    spa::QueryResult firstUsesRes = pkbManager->getRelationship(spa::USES,
      spa::PKBQueryArg(lineNum),
      spa::PKBQueryArg(variable));

    variable = spa::PqlArgument(spa::ArgumentType::VARIABLE_NAME, varB, {});
    spa::QueryResult secondUsesRes = pkbManager->getRelationship(spa::USES,
      spa::PKBQueryArg(lineNum),
      spa::PKBQueryArg(variable));

    variable = spa::PqlArgument(spa::ArgumentType::VARIABLE_NAME, varE, {});
    spa::QueryResult thirdUsesRes = pkbManager->getRelationship(spa::USES,
      spa::PKBQueryArg(lineNum),
      spa::PKBQueryArg(variable));

    lineNum = spa::PqlArgument(spa::ArgumentType::LINE_NO, "2", {});
    variable = spa::PqlArgument(spa::ArgumentType::VARIABLE_NAME, varD, {});
    spa::QueryResult fourthUsesRes = pkbManager->getRelationship(spa::USES,
      spa::PKBQueryArg(lineNum),
      spa::PKBQueryArg(variable));

    lineNum = spa::PqlArgument(spa::ArgumentType::LINE_NO, "3", {});
    variable = spa::PqlArgument(spa::ArgumentType::VARIABLE_NAME, varB, {});
    spa::QueryResult fifthUsesRes = pkbManager->getRelationship(spa::USES,
      spa::PKBQueryArg(lineNum),
      spa::PKBQueryArg(variable));

    bool testFirstUses = firstUsesRes.getIsTrue();
    bool testSecondUses = secondUsesRes.getIsTrue();
    bool testThirdUses = thirdUsesRes.getIsTrue();
    bool testFourthUses = fourthUsesRes.getIsTrue();
    bool testFifthUses = fifthUsesRes.getIsTrue();

    bool expectedFirstUses = true;
    bool expectedSecondUses = true;
    bool expectedThirdUses = true;
    bool expectedFourthUses = true;
    bool expectedFifthUses = true;

    Assert::IsTrue(firstUsesRes.getQueryResultType() == spa::BOOL);
    Assert::IsTrue(secondUsesRes.getQueryResultType() == spa::BOOL);
    Assert::IsTrue(thirdUsesRes.getQueryResultType() == spa::BOOL);
    Assert::IsTrue(fourthUsesRes.getQueryResultType() == spa::BOOL);
    Assert::IsTrue(fifthUsesRes.getQueryResultType() == spa::BOOL);

    Assert::IsTrue(expectedFirstUses == testFirstUses);
    Assert::IsTrue(expectedSecondUses == testSecondUses);
    Assert::IsTrue(expectedThirdUses == testThirdUses);
    Assert::IsTrue(expectedFourthUses == testFourthUses);
    Assert::IsTrue(expectedFifthUses == testFifthUses);
  }

  TEST_METHOD(TestExtractUsesWhileNestedWhileAssignment) {
    ///*
    // *  procedure a {
    // * 1. while (e >= 1) {
    // * 2.   while (d >= 1) {
    // * 3.     c = b;
    // *      }
    // *    }
    // *  }
    // */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenWhile, tokenOpenBracket, tokenE, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenOpenBrace,
      tokenWhile, tokenOpenBracket, tokenD, tokenGreaterEqual, tokenConstant, tokenCloseBracket,
      tokenOpenBrace,
      tokenC, tokenAssign, tokenB, tokenSemiColon,
      tokenCloseBrace, tokenCloseBrace, tokenCloseBrace
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
    spa::PqlArgument variable(spa::ArgumentType::VARIABLE_NAME, varD, {});
    spa::QueryResult firstUsesRes = pkbManager->getRelationship(spa::USES,
      spa::PKBQueryArg(lineNum),
      spa::PKBQueryArg(variable));

    variable = spa::PqlArgument(spa::ArgumentType::VARIABLE_NAME, varB, {});
    spa::QueryResult secondUsesRes = pkbManager->getRelationship(spa::USES,
      spa::PKBQueryArg(lineNum),
      spa::PKBQueryArg(variable));

    variable = spa::PqlArgument(spa::ArgumentType::VARIABLE_NAME, varE, {});
    spa::QueryResult thirdUsesRes = pkbManager->getRelationship(spa::USES,
      spa::PKBQueryArg(lineNum),
      spa::PKBQueryArg(variable));

    bool testFirstUses = firstUsesRes.getIsTrue();
    bool testSecondUses = secondUsesRes.getIsTrue();
    bool testThirdUses = thirdUsesRes.getIsTrue();

    bool expectedFirstUses = true;
    bool expectedSecondUses = true;
    bool expectedThirdUses = true;
    bool expectedFourthUses = true;
    bool expectedFifthUses = true;

    Assert::IsTrue(firstUsesRes.getQueryResultType() == spa::BOOL);
    Assert::IsTrue(secondUsesRes.getQueryResultType() == spa::BOOL);
    Assert::IsTrue(thirdUsesRes.getQueryResultType() == spa::BOOL);

    Assert::IsTrue(expectedFirstUses == testFirstUses);
    Assert::IsTrue(expectedSecondUses == testSecondUses);
    Assert::IsTrue(expectedThirdUses == testThirdUses);
  }

  TEST_METHOD(TestExtractUsesIfNestedThenAssignment) {
    ///*
    // *  procedure a {
    // * 1. if (d >= 1) then {
    // * 2.   if (e >= 1) then {
    // * 3.     c = b;
    // *      } else {
    // * 4.     c = f;
    // *      }
    // *    } else { }
    // *  }
    // */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenIf, tokenOpenBracket, tokenD, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenThen,tokenOpenBrace,
      tokenIf, tokenOpenBracket, tokenE, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenThen, tokenOpenBrace,
      tokenC, tokenAssign, tokenB, tokenSemiColon,
      tokenCloseBrace, tokenElse, tokenOpenBrace,
      tokenC, tokenAssign, tokenF, tokenSemiColon,
      tokenCloseBrace, tokenCloseBrace, tokenElse, tokenOpenBrace, tokenCloseBrace,
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
    spa::PqlArgument variable(spa::ArgumentType::VARIABLE_NAME, varD, {});
    spa::QueryResult firstUsesRes = pkbManager->getRelationship(spa::USES,
      spa::PKBQueryArg(lineNum),
      spa::PKBQueryArg(variable));

    variable = spa::PqlArgument(spa::ArgumentType::VARIABLE_NAME, varB, {});
    spa::QueryResult secondUsesRes = pkbManager->getRelationship(spa::USES,
      spa::PKBQueryArg(lineNum),
      spa::PKBQueryArg(variable));

    variable = spa::PqlArgument(spa::ArgumentType::VARIABLE_NAME, varE, {});
    spa::QueryResult thirdUsesRes = pkbManager->getRelationship(spa::USES,
      spa::PKBQueryArg(lineNum),
      spa::PKBQueryArg(variable));

    variable = spa::PqlArgument(spa::ArgumentType::VARIABLE_NAME, varF, {});
    spa::QueryResult fourthUsesRes = pkbManager->getRelationship(spa::USES,
      spa::PKBQueryArg(lineNum),
      spa::PKBQueryArg(variable));

    bool testFirstUses = firstUsesRes.getIsTrue();
    bool testSecondUses = secondUsesRes.getIsTrue();
    bool testThirdUses = thirdUsesRes.getIsTrue();
    bool testFourthUses = fourthUsesRes.getIsTrue();

    bool expectedFirstUses = true;
    bool expectedSecondUses = true;
    bool expectedThirdUses = true;
    bool expectedFourthUses = true;

    Assert::IsTrue(firstUsesRes.getQueryResultType() == spa::BOOL);
    Assert::IsTrue(secondUsesRes.getQueryResultType() == spa::BOOL);
    Assert::IsTrue(thirdUsesRes.getQueryResultType() == spa::BOOL);
    Assert::IsTrue(fourthUsesRes.getQueryResultType() == spa::BOOL);

    Assert::IsTrue(expectedFirstUses == testFirstUses);
    Assert::IsTrue(expectedSecondUses == testSecondUses);
    Assert::IsTrue(expectedThirdUses == testThirdUses);
    Assert::IsTrue(expectedFourthUses == testFourthUses);
  }

  TEST_METHOD(TestExtractUsesIfNestedElseAssignment) {
    ///*
    // *  procedure a {
    // * 1. if (d >= 1) then {
    // *    } else {
    // * 2.   if (e >= 1) then {
    // * 3.     c = b;
    // *      } else {
    // * 4.     c = f;
    // *      }
    // *    }
    // *  }
    // */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenIf, tokenOpenBracket, tokenD, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenThen,tokenOpenBrace,
      tokenCloseBrace, tokenElse, tokenOpenBrace,
      tokenIf, tokenOpenBracket, tokenE, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenThen, tokenOpenBrace,
      tokenC, tokenAssign, tokenB, tokenSemiColon,
      tokenCloseBrace, tokenElse, tokenOpenBrace,
      tokenC, tokenAssign, tokenF, tokenSemiColon,
      tokenCloseBrace, tokenCloseBrace, tokenCloseBrace
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
    spa::PqlArgument variable(spa::ArgumentType::VARIABLE_NAME, varD, {});
    spa::QueryResult firstUsesRes = pkbManager->getRelationship(spa::USES,
      spa::PKBQueryArg(lineNum),
      spa::PKBQueryArg(variable));

    variable = spa::PqlArgument(spa::ArgumentType::VARIABLE_NAME, varB, {});
    spa::QueryResult secondUsesRes = pkbManager->getRelationship(spa::USES,
      spa::PKBQueryArg(lineNum),
      spa::PKBQueryArg(variable));

    variable = spa::PqlArgument(spa::ArgumentType::VARIABLE_NAME, varE, {});
    spa::QueryResult thirdUsesRes = pkbManager->getRelationship(spa::USES,
      spa::PKBQueryArg(lineNum),
      spa::PKBQueryArg(variable));

    variable = spa::PqlArgument(spa::ArgumentType::VARIABLE_NAME, varF, {});
    spa::QueryResult fourthUsesRes = pkbManager->getRelationship(spa::USES,
      spa::PKBQueryArg(lineNum),
      spa::PKBQueryArg(variable));

    bool testFirstUses = firstUsesRes.getIsTrue();
    bool testSecondUses = secondUsesRes.getIsTrue();
    bool testThirdUses = thirdUsesRes.getIsTrue();
    bool testFourthUses = fourthUsesRes.getIsTrue();

    bool expectedFirstUses = true;
    bool expectedSecondUses = true;
    bool expectedThirdUses = true;
    bool expectedFourthUses = true;

    Assert::IsTrue(firstUsesRes.getQueryResultType() == spa::BOOL);
    Assert::IsTrue(secondUsesRes.getQueryResultType() == spa::BOOL);
    Assert::IsTrue(thirdUsesRes.getQueryResultType() == spa::BOOL);
    Assert::IsTrue(fourthUsesRes.getQueryResultType() == spa::BOOL);

    Assert::IsTrue(expectedFirstUses == testFirstUses);
    Assert::IsTrue(expectedSecondUses == testSecondUses);
    Assert::IsTrue(expectedThirdUses == testThirdUses);
    Assert::IsTrue(expectedFourthUses == testFourthUses);
  }

  TEST_METHOD(TestExtractUsesPrintStatement) {
    ///*
    // *  procedure a {
    // * 1. print d;
    // *  }
    // */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenPrint, tokenD, tokenSemiColon,
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
    spa::PqlArgument variable(spa::ArgumentType::VARIABLE_NAME, varD, {});
    spa::QueryResult usesRes = pkbManager->getRelationship(spa::USES, spa::PKBQueryArg(lineNum),
                                                           spa::PKBQueryArg(variable));

    Assert::IsTrue(usesRes.getQueryResultType() == spa::BOOL);

    bool testUses = usesRes.getIsTrue();

    bool expectedUses = true;

    Assert::IsTrue(expectedUses == testUses);
  }

  TEST_METHOD(TestExtractUsesIfNestedPrintStatement) {
    ///*
    // *  procedure a {
    // * 1. if (e >= 1) then {
    // * 2.   print d;
    // *    } else {
    // * 3.   print b;
    // *    }
    // *  }
    // */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenIf, tokenOpenBracket, tokenE, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenThen, tokenOpenBrace,
      tokenPrint, tokenD, tokenSemiColon,
      tokenCloseBrace,
      tokenElse, tokenOpenBrace,
      tokenPrint, tokenB, tokenSemiColon,
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
    spa::PqlArgument variable(spa::ArgumentType::VARIABLE_NAME, varD, {});
    spa::QueryResult firstUsesRes = pkbManager->getRelationship(spa::USES,
                                                                spa::PKBQueryArg(lineNum),
                                                                spa::PKBQueryArg(variable));

    variable = spa::PqlArgument(spa::ArgumentType::VARIABLE_NAME, varB, {});
    spa::QueryResult secondUsesRes = pkbManager->getRelationship(spa::USES,
                                                                 spa::PKBQueryArg(lineNum),
                                                                 spa::PKBQueryArg(variable));

    variable = spa::PqlArgument(spa::ArgumentType::VARIABLE_NAME, varE, {});
    spa::QueryResult thirdUsesRes = pkbManager->getRelationship(spa::USES,
                                                                spa::PKBQueryArg(lineNum),
                                                                spa::PKBQueryArg(variable));

    lineNum = spa::PqlArgument(spa::ArgumentType::LINE_NO, "2", {});
    variable = spa::PqlArgument(spa::ArgumentType::VARIABLE_NAME, varD, {});
    spa::QueryResult thenUsesRes = pkbManager->getRelationship(spa::USES,
                                                               spa::PKBQueryArg(lineNum),
                                                               spa::PKBQueryArg(variable));

    lineNum = spa::PqlArgument(spa::ArgumentType::LINE_NO, "3", {});
    variable = spa::PqlArgument(spa::ArgumentType::VARIABLE_NAME, varB, {});
    spa::QueryResult elseUsesRes = pkbManager->getRelationship(spa::USES,
                                                               spa::PKBQueryArg(lineNum),
                                                               spa::PKBQueryArg(variable));

    bool testFirstUses = firstUsesRes.getIsTrue();
    bool testSecondUses = secondUsesRes.getIsTrue();
    bool testThirdUses = thirdUsesRes.getIsTrue();
    bool testThenUses = thenUsesRes.getIsTrue();
    bool testElseUses = elseUsesRes.getIsTrue();

    bool expectedFirstUses = true;
    bool expectedSecondUses = true;
    bool expectedThirdUses = true;
    bool expectedThenUses = true;
    bool expectedElseUses = true;

    Assert::IsTrue(firstUsesRes.getQueryResultType() == spa::BOOL);
    Assert::IsTrue(secondUsesRes.getQueryResultType() == spa::BOOL);
    Assert::IsTrue(thirdUsesRes.getQueryResultType() == spa::BOOL);
    Assert::IsTrue(thenUsesRes.getQueryResultType() == spa::BOOL);
    Assert::IsTrue(elseUsesRes.getQueryResultType() == spa::BOOL);

    Assert::IsTrue(expectedFirstUses == testFirstUses);
    Assert::IsTrue(expectedSecondUses == testSecondUses);
    Assert::IsTrue(expectedThirdUses == testThirdUses);
    Assert::IsTrue(expectedThenUses == testThenUses);
    Assert::IsTrue(expectedElseUses == testElseUses);
  }

  TEST_METHOD(TestExtractUsesWhileNestedPrintStatement) {
    ///*
    // *  procedure a {
    // * 1. while (e >= 1) {
    // * 2.   print d;
    // * 3.   print b;
    // *    }
    // *  }
    // */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenWhile, tokenOpenBracket, tokenE, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenOpenBrace,
      tokenPrint, tokenD, tokenSemiColon,
      tokenPrint, tokenB, tokenSemiColon,
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
    spa::PqlArgument variable(spa::ArgumentType::VARIABLE_NAME, varD, {});
    spa::QueryResult firstUsesRes = pkbManager->getRelationship(spa::USES,
      spa::PKBQueryArg(lineNum),
      spa::PKBQueryArg(variable));

    variable = spa::PqlArgument(spa::ArgumentType::VARIABLE_NAME, varB, {});
    spa::QueryResult secondUsesRes = pkbManager->getRelationship(spa::USES,
      spa::PKBQueryArg(lineNum),
      spa::PKBQueryArg(variable));

    variable = spa::PqlArgument(spa::ArgumentType::VARIABLE_NAME, varE, {});
    spa::QueryResult thirdUsesRes = pkbManager->getRelationship(spa::USES,
      spa::PKBQueryArg(lineNum),
      spa::PKBQueryArg(variable));

    lineNum = spa::PqlArgument(spa::ArgumentType::LINE_NO, "2", {});
    variable = spa::PqlArgument(spa::ArgumentType::VARIABLE_NAME, varD, {});
    spa::QueryResult fourthUsesRes = pkbManager->getRelationship(spa::USES,
      spa::PKBQueryArg(lineNum),
      spa::PKBQueryArg(variable));

    lineNum = spa::PqlArgument(spa::ArgumentType::LINE_NO, "3", {});
    variable = spa::PqlArgument(spa::ArgumentType::VARIABLE_NAME, varB, {});
    spa::QueryResult fifthUsesRes = pkbManager->getRelationship(spa::USES,
      spa::PKBQueryArg(lineNum),
      spa::PKBQueryArg(variable));

    bool testFirstUses = firstUsesRes.getIsTrue();
    bool testSecondUses = secondUsesRes.getIsTrue();
    bool testThirdUses = thirdUsesRes.getIsTrue();
    bool testFourthUses = fourthUsesRes.getIsTrue();
    bool testFifthUses = fifthUsesRes.getIsTrue();

    bool expectedFirstUses = true;
    bool expectedSecondUses = true;
    bool expectedThirdUses = true;
    bool expectedFourthUses = true;
    bool expectedFifthUses = true;

    Assert::IsTrue(firstUsesRes.getQueryResultType() == spa::BOOL);
    Assert::IsTrue(secondUsesRes.getQueryResultType() == spa::BOOL);
    Assert::IsTrue(thirdUsesRes.getQueryResultType() == spa::BOOL);
    Assert::IsTrue(fourthUsesRes.getQueryResultType() == spa::BOOL);
    Assert::IsTrue(fifthUsesRes.getQueryResultType() == spa::BOOL);

    Assert::IsTrue(expectedFirstUses == testFirstUses);
    Assert::IsTrue(expectedSecondUses == testSecondUses);
    Assert::IsTrue(expectedThirdUses == testThirdUses);
    Assert::IsTrue(expectedFourthUses == testFourthUses);
    Assert::IsTrue(expectedFifthUses == testFifthUses);
  }
};
}  // namespace UnitTesting
