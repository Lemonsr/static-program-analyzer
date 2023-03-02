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
TEST_CLASS(TestDesignExtractorStatements) {
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
  TEST_METHOD(TestExtractOneVarStatementWithNoNesting) {
    /*
     *  procedure a {
     * 1. read b;
     * 2. print c;
     * 3. call d;
     *  }
     *  procedure d {
     * 4. read b;
     *  }
     */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenRead, tokenB, tokenSemiColon,
      tokenPrint, tokenC, tokenSemiColon,
      tokenCall, tokenD, tokenSemiColon,
      tokenCloseBrace,
      tokenProcedure, tokenD, tokenOpenBrace,
      tokenRead, tokenB, tokenSemiColon,
      tokenCloseBrace
    };
    spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
    for (auto& token : tokenList) {
      tokenStream.pushBack(token);
    }
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    auto parser = spa::SpParser(tokenStream);
    std::vector<spa::ProcedureStatement> procedureList = parser.parse();
    Assert::IsTrue(procedureList.size() == 2);

    spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
    designExtractor.extractRelationship();
    spa::QueryResult procedureRes = pkbManager->getEntity(spa::PROCEDURE);
    spa::QueryResult variablesRes = pkbManager->getEntity(spa::VARIABLE);
    spa::QueryResult readStmtRes = pkbManager->getEntity(spa::READ);
    spa::QueryResult printStmtRes = pkbManager->getEntity(spa::PRINT);
    spa::QueryResult callStmtRes = pkbManager->getEntity(spa::CALL);

    std::optional<std::vector<std::string>> testProcedure = procedureRes.getNames();
    std::optional<std::vector<std::string>> testVariable = variablesRes.getNames();
    std::optional<std::vector<int>> testReadStmt = readStmtRes.getLineNumbers();
    std::optional<std::vector<int>> testPrintStmt = printStmtRes.getLineNumbers();
    std::optional<std::vector<int>> testCallStmt = callStmtRes.getLineNumbers();

    std::optional<std::vector<std::string>> expectedProcedure = {{varA, varD}};
    std::optional<std::vector<std::string>> expectedVariable = {{varB, varC}};
    std::optional<std::vector<int>> expectedReadStmt = {{1, 4}};
    std::optional<std::vector<int>> expectedPrintStmt = {{2}};
    std::optional<std::vector<int>> expectedCallStmt = {{3}};

    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedProcedure,
      testProcedure));
    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedVariable,
      testVariable));
    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedReadStmt,
      testReadStmt));
    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedPrintStmt,
      testPrintStmt));
    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedCallStmt,
      testCallStmt));
  }

  TEST_METHOD(TestExtractMultipleOneVarStatementWithNoNesting) {
    /*
     *  procedure a {
     * 1. read b;
     * 2. print c;
     * 3. call d;
     * 4. read d;
     * 5. print b;
     * 6. call c;
     *  }
     *  procedure d {
     * 7. read b;
     * }
     * procedure c {
     * 8. read b;
     * }
     */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenRead, tokenB, tokenSemiColon,
      tokenPrint, tokenC, tokenSemiColon,
      tokenCall, tokenD, tokenSemiColon,
      tokenRead, tokenD, tokenSemiColon,
      tokenPrint, tokenB, tokenSemiColon,
      tokenCall, tokenC, tokenSemiColon,
      tokenCloseBrace,
      tokenProcedure, tokenD, tokenOpenBrace,
      tokenRead, tokenB, tokenSemiColon,
      tokenCloseBrace,
      tokenProcedure, tokenC, tokenOpenBrace,
      tokenRead, tokenB, tokenSemiColon,
      tokenCloseBrace
    };
    spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
    for (auto& token : tokenList) {
      tokenStream.pushBack(token);
    }
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    auto parser = spa::SpParser(tokenStream);
    std::vector<spa::ProcedureStatement> procedureList = parser.parse();
    Assert::IsTrue(procedureList.size() == 3);

    spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
    designExtractor.extractRelationship();
    spa::QueryResult procedureRes = pkbManager->getEntity(spa::PROCEDURE);
    spa::QueryResult variablesRes = pkbManager->getEntity(spa::VARIABLE);
    spa::QueryResult readStmtRes = pkbManager->getEntity(spa::READ);
    spa::QueryResult printStmtRes = pkbManager->getEntity(spa::PRINT);
    spa::QueryResult callStmtRes = pkbManager->getEntity(spa::CALL);

    std::optional<std::vector<std::string>> testProcedure = procedureRes.getNames();
    std::optional<std::vector<std::string>> testVariable = variablesRes.getNames();
    std::optional<std::vector<int>> testReadStmt = readStmtRes.getLineNumbers();
    std::optional<std::vector<int>> testPrintStmt = printStmtRes.getLineNumbers();
    std::optional<std::vector<int>> testCallStmt = callStmtRes.getLineNumbers();

    std::optional<std::vector<std::string>> expectedProcedure = {{varA, varD, varC}};
    std::optional<std::vector<std::string>> expectedVariable = {{varB, varC, varD}};
    std::optional<std::vector<int>> expectedReadStmt = {{1, 4, 7, 8}};
    std::optional<std::vector<int>> expectedPrintStmt = {{2, 5}};
    std::optional<std::vector<int>> expectedCallStmt = {{3, 6}};

    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedProcedure,
      testProcedure));
    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedVariable,
      testVariable));
    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedReadStmt,
      testReadStmt));
    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedPrintStmt,
      testPrintStmt));
    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedCallStmt,
      testCallStmt));
  }

  TEST_METHOD(TestExtractOneVarStatementWithWhileNesting) {
    /*
    *  procedure a {
    * 1. while (e >= 1) {
    * 2.   read b;
    * 3.   print c;
    * 4.   call d;
    *    }
    *  }
    *  procedure d {
    *    a = 1;
    *  }
    */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenWhile, tokenOpenBracket, tokenE, tokenGreaterEqual, tokenConstant, tokenCloseBracket,
      tokenOpenBrace,
      tokenRead, tokenB, tokenSemiColon,
      tokenPrint, tokenC, tokenSemiColon,
      tokenCall, tokenD, tokenSemiColon,
      tokenCloseBrace, tokenCloseBrace,
      tokenProcedure, tokenD, tokenOpenBrace,
      tokenA, tokenAssign, tokenConstant, tokenSemiColon,
      tokenCloseBrace
    };
    spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
    for (auto& token : tokenList) {
      tokenStream.pushBack(token);
    }
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    auto parser = spa::SpParser(tokenStream);
    std::vector<spa::ProcedureStatement> procedureList = parser.parse();
    Assert::IsTrue(procedureList.size() == 2);

    spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
    designExtractor.extractRelationship();
    spa::QueryResult procedureRes = pkbManager->getEntity(spa::PROCEDURE);
    spa::QueryResult variablesRes = pkbManager->getEntity(spa::VARIABLE);
    spa::QueryResult readStmtRes = pkbManager->getEntity(spa::READ);
    spa::QueryResult printStmtRes = pkbManager->getEntity(spa::PRINT);
    spa::QueryResult callStmtRes = pkbManager->getEntity(spa::CALL);

    std::optional<std::vector<std::string>> testProcedure = procedureRes.getNames();
    std::optional<std::vector<std::string>> testVariable = variablesRes.getNames();
    std::optional<std::vector<int>> testReadStmt = readStmtRes.getLineNumbers();
    std::optional<std::vector<int>> testPrintStmt = printStmtRes.getLineNumbers();
    std::optional<std::vector<int>> testCallStmt = callStmtRes.getLineNumbers();

    std::optional<std::vector<std::string>> expectedProcedure = {{varA, varD}};
    std::optional<std::vector<std::string>> expectedVariable = {{varB, varC, varE, varA}};
    std::optional<std::vector<int>> expectedReadStmt = {{2}};
    std::optional<std::vector<int>> expectedPrintStmt = {{3}};
    std::optional<std::vector<int>> expectedCallStmt = {{4}};

    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedProcedure,
      testProcedure));
    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedVariable,
      testVariable));
    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedReadStmt,
      testReadStmt));
    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedPrintStmt,
      testPrintStmt));
    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedCallStmt,
      testCallStmt));
  }

  TEST_METHOD(TestExtractOneVarStatementWithIfNesting) {
    /*
     *  procedure a {
     * 1. if (e >= 1) then {
     * 2.   read b;
     *    }
     *    else {
     * 3.   print c;
     * 4.   call d;
     *    }
     *  }
     *  procedure d {
     *     a = 1;
        }
     */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenIf, tokenOpenBracket, tokenE, tokenGreaterEqual, tokenConstant, tokenCloseBracket,
      tokenThen, tokenOpenBrace,
      tokenRead, tokenB, tokenSemiColon,
      tokenCloseBrace, tokenElse, tokenOpenBrace,
      tokenPrint, tokenC, tokenSemiColon,
      tokenCall, tokenD, tokenSemiColon,
      tokenCloseBrace, tokenCloseBrace,
      tokenProcedure, tokenD, tokenOpenBrace,
      tokenA, tokenAssign, tokenConstant, tokenSemiColon,
      tokenCloseBrace
    };
    spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
    for (auto& token : tokenList) {
      tokenStream.pushBack(token);
    }
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    auto parser = spa::SpParser(tokenStream);
    std::vector<spa::ProcedureStatement> procedureList = parser.parse();
    Assert::IsTrue(procedureList.size() == 2);

    spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
    designExtractor.extractRelationship();
    spa::QueryResult procedureRes = pkbManager->getEntity(spa::PROCEDURE);
    spa::QueryResult variablesRes = pkbManager->getEntity(spa::VARIABLE);
    spa::QueryResult readStmtRes = pkbManager->getEntity(spa::READ);
    spa::QueryResult printStmtRes = pkbManager->getEntity(spa::PRINT);
    spa::QueryResult callStmtRes = pkbManager->getEntity(spa::CALL);

    std::optional<std::vector<std::string>> testProcedure = procedureRes.getNames();
    std::optional<std::vector<std::string>> testVariable = variablesRes.getNames();
    std::optional<std::vector<int>> testReadStmt = readStmtRes.getLineNumbers();
    std::optional<std::vector<int>> testPrintStmt = printStmtRes.getLineNumbers();
    std::optional<std::vector<int>> testCallStmt = callStmtRes.getLineNumbers();

    std::optional<std::vector<std::string>> expectedProcedure = {{varA, varD}};
    std::optional<std::vector<std::string>> expectedVariable = {{varB, varC, varE, varA}};
    std::optional<std::vector<int>> expectedReadStmt = {{2}};
    std::optional<std::vector<int>> expectedPrintStmt = {{3}};
    std::optional<std::vector<int>> expectedCallStmt = {{4}};

    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedProcedure,
      testProcedure));
    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedVariable,
      testVariable));
    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedReadStmt,
      testReadStmt));
    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedPrintStmt,
      testPrintStmt));
    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedCallStmt,
      testCallStmt));
  }

  TEST_METHOD(TestExtractAssignStatementWithNoNesting) {
    /*
     *  procedure a {
     * 1. d = b - b * c;
     *  }
     */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenD, tokenAssign, tokenB, tokenMinusOp, tokenB, tokenMultiply, tokenC,
      tokenSemiColon, tokenCloseBrace
    };
    spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
    for (auto& token : tokenList) {
      tokenStream.pushBack(token);
    }
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    auto parser = spa::SpParser(tokenStream);
    std::vector<spa::ProcedureStatement> procedureList = parser.parse();
    Assert::IsTrue(procedureList.size() == 1);

    spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
    designExtractor.extractRelationship();
    spa::QueryResult procedureRes = pkbManager->getEntity(spa::PROCEDURE);
    spa::QueryResult variablesRes = pkbManager->getEntity(spa::VARIABLE);
    spa::QueryResult assignStmtRes = pkbManager->getEntity(spa::ASSIGN);

    spa::PqlArgument firstArg(spa::ArgumentType::LITERAL_STRING, varD, {});
    std::vector<spa::Token> tokens = {tokenB, tokenMinusOp, tokenB, tokenMultiply, tokenC};
    spa::Pattern pattern(spa::PatternType::EXACT, tokens);
    spa::QueryResult patternStatementRes = pkbManager->getPattern(spa::PKBQueryArg(firstArg),
      pattern);

    Assert::IsTrue(patternStatementRes.getQueryResultType() == spa::TUPLE);

    std::optional<std::vector<std::string>> testProcedure = procedureRes.getNames();
    std::optional<std::vector<std::string>> testVariable = variablesRes.getNames();
    std::optional<std::vector<int>> testAssignStmt = assignStmtRes.getLineNumbers();
    std::optional<std::vector<std::pair<int, std::string>>> testPatternStmt =
      patternStatementRes.getLineNumberNamePairs();

    std::optional<std::vector<std::string>> expectedProcedure = {{varA}};
    std::optional<std::vector<std::string>> expectedVariable = {{varB, varC, varD}};
    std::optional<std::vector<int>> expectedAssignStmt = {{1}};
    std::optional<std::vector<std::pair<int, std::string>>> expectedPatternStmt = {{{1, varD}}};

    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedProcedure,
      testProcedure));
    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedVariable,
      testVariable));
    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedAssignStmt,
      testAssignStmt));
    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedPatternStmt,
      testPatternStmt));
  }

  TEST_METHOD(TestExtractAssignStatementWithIfNesting) {
    /*
     *  procedure a {
     * 1. if (b >= c) then {
     * 2.   d = d - d * c;
     *    }
     *    else {
     * 3.   e = 1;
     *    }
     *  }
     */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenIf, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenC, tokenCloseBracket,
      tokenThen,
      tokenOpenBrace, tokenD, tokenAssign, tokenD, tokenMinusOp, tokenD, tokenMultiply, tokenC,
      tokenSemiColon, tokenCloseBrace,
      tokenElse, tokenOpenBrace,
      tokenE, tokenAssign, tokenConstant, tokenSemiColon,
      tokenCloseBrace, tokenCloseBrace
    };
    spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
    for (auto& token : tokenList) {
      tokenStream.pushBack(token);
    }
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    auto parser = spa::SpParser(tokenStream);
    std::vector<spa::ProcedureStatement> procedureList = parser.parse();
    Assert::IsTrue(procedureList.size() == 1);

    spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
    designExtractor.extractRelationship();
    spa::QueryResult procedureRes = pkbManager->getEntity(spa::PROCEDURE);
    spa::QueryResult variablesRes = pkbManager->getEntity(spa::VARIABLE);
    spa::QueryResult assignStmtRes = pkbManager->getEntity(spa::ASSIGN);

    spa::PqlArgument firstArg(spa::ArgumentType::LITERAL_STRING, varD, {});
    std::vector<spa::Token> tokens = {tokenD, tokenMinusOp, tokenD, tokenMultiply, tokenC};
    spa::Pattern pattern(spa::PatternType::EXACT, tokens);
    spa::QueryResult firstPatternStatementRes = pkbManager->getPattern(spa::PKBQueryArg(firstArg),
      pattern);

    firstArg = spa::PqlArgument(spa::ArgumentType::LITERAL_STRING, varE, {});
    tokens = {tokenConstant};
    pattern = spa::Pattern(spa::PatternType::EXACT, tokens);
    spa::QueryResult secondPatternStatementRes = pkbManager->getPattern(spa::PKBQueryArg(firstArg),
      pattern);

    Assert::IsTrue(firstPatternStatementRes.getQueryResultType() == spa::TUPLE);
    Assert::IsTrue(secondPatternStatementRes.getQueryResultType() == spa::TUPLE);

    std::optional<std::vector<std::string>> testProcedure = procedureRes.getNames();
    std::optional<std::vector<std::string>> testVariable = variablesRes.getNames();
    std::optional<std::vector<int>> testAssignStmt = assignStmtRes.getLineNumbers();
    std::optional<std::vector<std::pair<int, std::string>>> testFirstPatternStmt =
      firstPatternStatementRes.getLineNumberNamePairs();
    std::optional<std::vector<std::pair<int, std::string>>> testSecondPatternStmt =
      secondPatternStatementRes.getLineNumberNamePairs();

    std::optional<std::vector<std::string>> expectedProcedure = {{varA}};
    std::optional<std::vector<std::string>> expectedVariable = {{varB, varC, varD, varE}};
    std::optional<std::vector<int>> expectedAssignStmt = {{2, 3}};
    std::optional<std::vector<std::pair<int, std::string>>> expectedFirstPatternStmt =
      {{{2, varD}}};
    std::optional<std::vector<std::pair<int, std::string>>> expectedSecondPatternStmt = {
      {{3, varE}}
    };

    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedProcedure,
      testProcedure));
    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedVariable,
      testVariable));
    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedAssignStmt,
      testAssignStmt));
    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedFirstPatternStmt,
      testFirstPatternStmt));
    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedSecondPatternStmt,
      testSecondPatternStmt));
  }

  TEST_METHOD(TestExtractAssignStatementWithWhileNesting) {
    /*
     *  procedure a {
     * 1. while (b >= c) {
     * 2.   d = d - d * c;
     *    }
     *  }
     */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenWhile, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenC, tokenCloseBracket,
      tokenOpenBrace,
      tokenD, tokenAssign, tokenD, tokenMinusOp, tokenD, tokenMultiply, tokenC,
      tokenSemiColon, tokenCloseBrace,
      tokenCloseBrace
    };
    spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
    for (auto& token : tokenList) {
      tokenStream.pushBack(token);
    }
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    auto parser = spa::SpParser(tokenStream);
    std::vector<spa::ProcedureStatement> procedureList = parser.parse();
    Assert::IsTrue(procedureList.size() == 1);

    spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
    designExtractor.extractRelationship();
    spa::QueryResult procedureRes = pkbManager->getEntity(spa::PROCEDURE);
    spa::QueryResult variablesRes = pkbManager->getEntity(spa::VARIABLE);
    spa::QueryResult assignStmtRes = pkbManager->getEntity(spa::ASSIGN);

    spa::PqlArgument firstArg(spa::ArgumentType::LITERAL_STRING, varD, {});
    std::vector<spa::Token> tokens = {tokenD, tokenMinusOp, tokenD, tokenMultiply, tokenC};
    spa::Pattern pattern(spa::PatternType::EXACT, tokens);
    spa::QueryResult firstPatternStatementRes = pkbManager->getPattern(spa::PKBQueryArg(firstArg),
      pattern);

    Assert::IsTrue(firstPatternStatementRes.getQueryResultType() == spa::TUPLE);

    std::optional<std::vector<std::string>> testProcedure = procedureRes.getNames();
    std::optional<std::vector<std::string>> testVariable = variablesRes.getNames();
    std::optional<std::vector<int>> testAssignStmt = assignStmtRes.getLineNumbers();
    std::optional<std::vector<std::pair<int, std::string>>> testFirstPatternStmt =
      firstPatternStatementRes.getLineNumberNamePairs();

    std::optional<std::vector<std::string>> expectedProcedure = {{varA}};
    std::optional<std::vector<std::string>> expectedVariable = {{varB, varC, varD}};
    std::optional<std::vector<int>> expectedAssignStmt = {{2}};
    std::optional<std::vector<std::pair<int, std::string>>> expectedFirstPatternStmt =
      {{{2, varD}}};

    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedProcedure,
      testProcedure));
    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedVariable,
      testVariable));
    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedAssignStmt,
      testAssignStmt));
    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedFirstPatternStmt,
      testFirstPatternStmt));
  }

  TEST_METHOD(TestExtractIfStatementWithNoNesting) {
    /*
     *  procedure a {
     * 1. if (b >= 1) then { } else { }
     *  }
     */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenIf, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenConstant, tokenCloseBracket,
      tokenCloseBrace, tokenThen, tokenOpenBrace,
      tokenCloseBrace, tokenElse, tokenOpenBrace, tokenCloseBrace,
      tokenCloseBrace
    };
    spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
    for (auto& token : tokenList) {
      tokenStream.pushBack(token);
    }
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    auto parser = spa::SpParser(tokenStream);
    std::vector<spa::ProcedureStatement> procedureList = parser.parse();
    Assert::IsTrue(procedureList.size() == 1);

    spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
    designExtractor.extractRelationship();
    spa::QueryResult procedureRes = pkbManager->getEntity(spa::PROCEDURE);
    spa::QueryResult variablesRes = pkbManager->getEntity(spa::VARIABLE);
    spa::QueryResult constantRes = pkbManager->getEntity(spa::CONSTANT);
    spa::QueryResult ifStmtRes = pkbManager->getEntity(spa::IF);

    std::optional<std::vector<std::string>> testProcedure = procedureRes.getNames();
    std::optional<std::vector<std::string>> testVariable = variablesRes.getNames();
    std::optional<std::vector<std::string>> testConstant = constantRes.getNames();
    std::optional<std::vector<int>> testIfStmt = ifStmtRes.getLineNumbers();

    std::optional<std::vector<std::string>> expectedProcedure = {{varA}};
    std::optional<std::vector<std::string>> expectedVariable = {{varB}};
    std::optional<std::vector<std::string>> expectedConstant = {{constant}};
    std::optional<std::vector<int>> expectedIfStmt = {{1}};

    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedProcedure,
      testProcedure));
    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedVariable,
      testVariable));
    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedConstant,
      testConstant));
    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedIfStmt,
      testIfStmt));
  }

  TEST_METHOD(TestExtractIfStatementWithIfNesting) {
    /*
     *  procedure a {
     * 1. if (b >= 1) then {
     * 2.   if (c >= 1) then { } else { }
     *    }
     *    else {
     * 3.   if (d >= 1) then { } else { }
     *    }
     *  }
     */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenIf, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenConstant, tokenCloseBracket,
      tokenThen, tokenOpenBrace,
      tokenIf, tokenOpenBracket, tokenC, tokenGreaterEqual, tokenConstant, tokenCloseBracket,
      tokenThen, tokenOpenBrace, tokenCloseBrace, tokenElse, tokenOpenBrace, tokenCloseBrace,
      tokenCloseBrace, tokenElse, tokenOpenBrace,
      tokenIf, tokenOpenBracket, tokenD, tokenGreaterEqual, tokenConstant, tokenCloseBracket,
      tokenThen, tokenOpenBrace, tokenCloseBrace, tokenElse, tokenOpenBrace, tokenCloseBrace,
      tokenCloseBrace, tokenCloseBrace
    };
    spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
    for (auto& token : tokenList) {
      tokenStream.pushBack(token);
    }
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    auto parser = spa::SpParser(tokenStream);
    std::vector<spa::ProcedureStatement> procedureList = parser.parse();
    Assert::IsTrue(procedureList.size() == 1);

    spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
    designExtractor.extractRelationship();
    spa::QueryResult procedureRes = pkbManager->getEntity(spa::PROCEDURE);
    spa::QueryResult variablesRes = pkbManager->getEntity(spa::VARIABLE);
    spa::QueryResult constantRes = pkbManager->getEntity(spa::CONSTANT);
    spa::QueryResult ifStmtRes = pkbManager->getEntity(spa::IF);

    std::optional<std::vector<std::string>> testProcedure = procedureRes.getNames();
    std::optional<std::vector<std::string>> testVariable = variablesRes.getNames();
    std::optional<std::vector<std::string>> testConstant = constantRes.getNames();
    std::optional<std::vector<int>> testIfStmt = ifStmtRes.getLineNumbers();

    std::optional<std::vector<std::string>> expectedProcedure = {{varA}};
    std::optional<std::vector<std::string>> expectedVariable = {{varB, varC, varD}};
    std::optional<std::vector<std::string>> expectedConstant = {{constant}};
    std::optional<std::vector<int>> expectedIfStmt = {{1, 2, 3}};

    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedProcedure,
      testProcedure));
    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedVariable,
      testVariable));
    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedConstant,
      testConstant));
    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedIfStmt,
      testIfStmt));
  }

  TEST_METHOD(TestExtractIfStatementWithWhileNesting) {
    /*
     *  procedure a {
     * 1. while (1 >= 1) {
     * 2.  if (b >= c) then { } else { }
     *    }
     *  }
     */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenWhile, tokenOpenBracket, tokenConstant, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket,
      tokenOpenBrace,
      tokenIf, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenC, tokenCloseBracket,
      tokenThen, tokenOpenBrace, tokenCloseBrace, tokenElse, tokenOpenBrace, tokenCloseBrace,
      tokenCloseBrace, tokenCloseBrace
    };
    spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
    for (auto& token : tokenList) {
      tokenStream.pushBack(token);
    }
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    auto parser = spa::SpParser(tokenStream);
    std::vector<spa::ProcedureStatement> procedureList = parser.parse();
    Assert::IsTrue(procedureList.size() == 1);

    spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
    designExtractor.extractRelationship();
    spa::QueryResult procedureRes = pkbManager->getEntity(spa::PROCEDURE);
    spa::QueryResult variablesRes = pkbManager->getEntity(spa::VARIABLE);
    spa::QueryResult ifStmtRes = pkbManager->getEntity(spa::IF);

    std::optional<std::vector<std::string>> testProcedure = procedureRes.getNames();
    std::optional<std::vector<std::string>> testVariable = variablesRes.getNames();
    std::optional<std::vector<int>> testIfStmt = ifStmtRes.getLineNumbers();

    std::optional<std::vector<std::string>> expectedProcedure = {{varA}};
    std::optional<std::vector<std::string>> expectedVariable = {{varB, varC}};
    std::optional<std::vector<int>> expectedIfStmt = {{2}};

    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedProcedure,
      testProcedure));
    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedVariable,
      testVariable));
    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedIfStmt,
      testIfStmt));
  }

  TEST_METHOD(TestExtractWhileStatementWithNoNesting) {
    /*
     *  procedure a {
     * 1. while (b >= 1) { }
     *  }
     */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenWhile, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenConstant, tokenCloseBracket,
      tokenOpenBrace, tokenCloseBrace,
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
    spa::QueryResult procedureRes = pkbManager->getEntity(spa::PROCEDURE);
    spa::QueryResult variablesRes = pkbManager->getEntity(spa::VARIABLE);
    spa::QueryResult constantRes = pkbManager->getEntity(spa::CONSTANT);
    spa::QueryResult whileStmtRes = pkbManager->getEntity(spa::WHILE);

    std::optional<std::vector<std::string>> testProcedure = procedureRes.getNames();
    std::optional<std::vector<std::string>> testVariable = variablesRes.getNames();
    std::optional<std::vector<std::string>> testConstant = constantRes.getNames();
    std::optional<std::vector<int>> testWhileStmt = whileStmtRes.getLineNumbers();

    std::optional<std::vector<std::string>> expectedProcedure = {{varA}};
    std::optional<std::vector<std::string>> expectedVariable = {{varB}};
    std::optional<std::vector<std::string>> expectedConstant = {{constant}};
    std::optional<std::vector<int>> expectedWhileStmt = {{1}};

    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedProcedure,
      testProcedure));
    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedVariable,
      testVariable));
    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedConstant,
      testConstant));
    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedWhileStmt,
      testWhileStmt));
  }

  TEST_METHOD(TestExtractWhileStatementWithIfNesting) {
    /*
     *  procedure a {
     * 1. if (1 >= 1) then {
     * 2.   while (b >= c) { }
     *    } else {
     * 3.   while (d >= e) { }
     *    }
     *  }
     */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenIf, tokenOpenBracket, tokenConstant, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenThen, tokenOpenBrace,
      tokenWhile, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenC, tokenCloseBracket,
      tokenOpenBrace, tokenCloseBrace, tokenCloseBrace, tokenElse, tokenOpenBrace,
      tokenWhile, tokenOpenBracket, tokenD, tokenGreaterEqual, tokenE, tokenCloseBracket,
      tokenOpenBrace, tokenCloseBrace, tokenCloseBrace,
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
    spa::QueryResult procedureRes = pkbManager->getEntity(spa::PROCEDURE);
    spa::QueryResult variablesRes = pkbManager->getEntity(spa::VARIABLE);
    spa::QueryResult whileStmtRes = pkbManager->getEntity(spa::WHILE);

    std::optional<std::vector<std::string>> testProcedure = procedureRes.getNames();
    std::optional<std::vector<std::string>> testVariable = variablesRes.getNames();
    std::optional<std::vector<int>> testWhileStmt = whileStmtRes.getLineNumbers();

    std::optional<std::vector<std::string>> expectedProcedure = {{varA}};
    std::optional<std::vector<std::string>> expectedVariable = {{varB, varC, varD, varE}};
    std::optional<std::vector<int>> expectedWhileStmt = {{2, 3}};

    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedProcedure,
      testProcedure));
    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedVariable,
      testVariable));
    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedWhileStmt,
      testWhileStmt));
  }

  TEST_METHOD(TestExtractWhileStatementWithWhileNesting) {
    /*
     *  procedure a {
     * 1. while (b >= 1) {
     * 2.   while (c >= d) { }
     *    }
     *  }
     */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenWhile, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenConstant, tokenCloseBracket,
      tokenOpenBrace,
      tokenWhile, tokenOpenBracket, tokenC, tokenGreaterEqual, tokenD, tokenCloseBracket,
      tokenOpenBrace, tokenCloseBrace,
      tokenCloseBrace, tokenCloseBrace
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
    spa::QueryResult procedureRes = pkbManager->getEntity(spa::PROCEDURE);
    spa::QueryResult variablesRes = pkbManager->getEntity(spa::VARIABLE);
    spa::QueryResult constantRes = pkbManager->getEntity(spa::CONSTANT);
    spa::QueryResult whileStmtRes = pkbManager->getEntity(spa::WHILE);

    std::optional<std::vector<std::string>> testProcedure = procedureRes.getNames();
    std::optional<std::vector<std::string>> testVariable = variablesRes.getNames();
    std::optional<std::vector<std::string>> testConstant = constantRes.getNames();
    std::optional<std::vector<int>> testWhileStmt = whileStmtRes.getLineNumbers();

    std::optional<std::vector<std::string>> expectedProcedure = {{varA}};
    std::optional<std::vector<std::string>> expectedVariable = {{varB, varC, varD}};
    std::optional<std::vector<std::string>> expectedConstant = {{constant}};
    std::optional<std::vector<int>> expectedWhileStmt = {{1, 2}};

    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedProcedure,
      testProcedure));
    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedVariable,
      testVariable));
    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedConstant,
      testConstant));
    Assert::IsTrue(spa::UtilsFunction::isOptionalVectorEqual(expectedWhileStmt,
      testWhileStmt));
  }
};
}  // namespace UnitTesting
