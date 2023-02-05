#include <iostream>
#include <vector>

#include "stdafx.h"

#include "DesignExtractor.h"
#include "PKB.h"
#include "SpParser.h"
#include "Token.h"
#include "UtilsFunction.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;
using namespace spa;
using namespace std;

namespace UnitTesting {
  TEST_CLASS(TestDesignExtractor) {
    string varA = "a";
    string varB = "b";
    string varC = "c";
    string varD = "d";
    string varE = "e";
    string constant = "1";

    string openBracket = "(";
    string closeBracket = ")";
    string openBrace = "{";
    string closeBrace = "}";
    string semiColon = ";";

    string minusOp = "-";
    string multiplyOp = "*";
    string greaterEqOp = ">=";
    string equal = "=";

    string readString = "read";
    string callString = "call";
    string printString = "print";
    string procedure = "procedure";
    string ifString = "if";
    string thenString = "then";
    string elseString = "else";
    string whileString = "while";

    Token tokenA = Token(TOKEN_NAME, varA);
    Token tokenB = Token(TOKEN_NAME, varB);
    Token tokenC = Token(TOKEN_NAME, varC);
    Token tokenD = Token(TOKEN_NAME, varD);
    Token tokenE = Token(TOKEN_NAME, varE);
    Token tokenConstant = Token(TOKEN_INTEGER, constant);

    Token tokenOpenBracket = Token(TOKEN_OPEN_BRACKET, openBracket);
    Token tokenCloseBracket = Token(TOKEN_CLOSE_BRACKET, closeBracket);
    Token tokenOpenBrace = Token(TOKEN_OPEN_BRACE, openBrace);
    Token tokenCloseBrace = Token(TOKEN_CLOSE_BRACE, closeBrace);
    Token tokenSemiColon = Token(TOKEN_SEMICOLON, semiColon);

    Token tokenMinusOp = Token(TOKEN_MINUS, minusOp);
    Token tokenMultiply = Token(TOKEN_MULTIPLY, multiplyOp);
    Token tokenAssign = Token(TOKEN_EQUAL, equal);
    Token tokenGreaterEqual = Token(TOKEN_COND_GTE, greaterEqOp);

    Token tokenRead = Token(TOKEN_READ, readString);
    Token tokenCall = Token(TOKEN_CALL, callString);
    Token tokenPrint = Token(TOKEN_PRINT, printString);
    Token tokenProcedure = Token(TOKEN_PROCEDURE, procedure);
    Token tokenIf = Token(TOKEN_IF, ifString);
    Token tokenThen = Token(TOKEN_THEN, thenString);
    Token tokenElse = Token(TOKEN_ELSE, elseString);
    Token tokenWhile = Token(TOKEN_WHILE, whileString);


    vector<Token> tokenList{};

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
      Stream<Token> tokenStream = Stream<Token>();
      for (auto& token : tokenList) {
        tokenStream.pushBack(token);
      }
      PKBManager* pkbManager = new PKB();
      auto parser = SpParser(tokenStream);
      vector<ProcedureStatement> procedureList = parser.parse();
      Assert::IsTrue(procedureList.size() == 1);

      DesignExtractor designExtractor = DesignExtractor(*pkbManager, procedureList);
      designExtractor.extractRelationship();

      QueryResult procedureRes = pkbManager->getEntity(PROCEDURE);
      QueryResult variablesRes = pkbManager->getEntity(VARIABLE);
      QueryResult constantRes = pkbManager->getEntity(CONSTANT);

      optional<vector<string>> testProcedure = procedureRes.getNames();
      optional<vector<string>> testVariable = variablesRes.getNames();
      optional<vector<string>> testConstant = constantRes.getNames();

      optional<vector<string>> expectedProcedure = {{varA}};
      optional<vector<string>> expectedVariable = {{varB, varC, varD}};
      optional<vector<string>> expectedConstant = {{constant}};

      Assert::IsTrue(UtilsFunction::isOptionalVectorEqual(expectedProcedure, testProcedure));
      Assert::IsTrue(UtilsFunction::isOptionalVectorEqual(expectedVariable, testVariable));
      Assert::IsTrue(UtilsFunction::isOptionalVectorEqual(expectedConstant, testConstant));
    }

    TEST_METHOD(TestExtractEntitiesWithNesting) {
      ///*
      // *  procedure a {
      // * 1. while (d >= 1) {
      // * 2. b = 1;
      // * 3. c = b;
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
      Stream<Token> tokenStream = Stream<Token>();
      for (auto& token : tokenList) {
        tokenStream.pushBack(token);
      }
      PKBManager* pkbManager = new PKB();
      auto parser = SpParser(tokenStream);
      vector<ProcedureStatement> procedureList = parser.parse();
      Assert::IsTrue(procedureList.size() == 1);

      DesignExtractor designExtractor = DesignExtractor(*pkbManager, procedureList);
      designExtractor.extractRelationship();

      QueryResult procedureRes = pkbManager->getEntity(PROCEDURE);
      QueryResult variablesRes = pkbManager->getEntity(VARIABLE);
      QueryResult constantRes = pkbManager->getEntity(CONSTANT);

      optional<vector<string>> testProcedure = procedureRes.getNames();
      optional<vector<string>> testVariable = variablesRes.getNames();
      optional<vector<string>> testConstant = constantRes.getNames();

      optional<vector<string>> expectedProcedure = {{varA}};
      optional<vector<string>> expectedVariable = {{varB, varC, varD}};
      optional<vector<string>> expectedConstant = {{constant}};

      Assert::IsTrue(UtilsFunction::isOptionalVectorEqual(expectedProcedure, testProcedure));
      Assert::IsTrue(UtilsFunction::isOptionalVectorEqual(expectedVariable, testVariable));
      Assert::IsTrue(UtilsFunction::isOptionalVectorEqual(expectedConstant, testConstant));
    }

    TEST_METHOD(TestExtractOneVarStatementWithNoNesting) {
      ///*
      // *  procedure a {
      // * 1. read b;
      // * 2. print c;
      // * 3. call d;
      // *  }
      // */
      tokenList = {
        tokenProcedure, tokenA, tokenOpenBrace,
        tokenRead, tokenB, tokenSemiColon,
        tokenPrint, tokenC, tokenSemiColon,
        tokenCall, tokenD, tokenSemiColon,
        tokenCloseBrace
      };
      Stream<Token> tokenStream = Stream<Token>();
      for (auto& token : tokenList) {
        tokenStream.pushBack(token);
      }
      PKBManager* pkbManager = new PKB();
      auto parser = SpParser(tokenStream);
      vector<ProcedureStatement> procedureList = parser.parse();
      Assert::IsTrue(procedureList.size() == 1);

      DesignExtractor designExtractor = DesignExtractor(*pkbManager, procedureList);
      designExtractor.extractRelationship();
      QueryResult procedureRes = pkbManager->getEntity(PROCEDURE);
      QueryResult variablesRes = pkbManager->getEntity(VARIABLE);
      QueryResult readStmtRes = pkbManager->getEntity(READ);
      QueryResult printStmtRes = pkbManager->getEntity(PRINT);
      QueryResult callStmtRes = pkbManager->getEntity(CALL);

      optional<vector<string>> testProcedure = procedureRes.getNames();
      optional<vector<string>> testVariable = variablesRes.getNames();
      optional<vector<int>> testReadStmt = readStmtRes.getLineNumbers();
      optional<vector<int>> testPrintStmt = printStmtRes.getLineNumbers();
      optional<vector<int>> testCallStmt = callStmtRes.getLineNumbers();

      optional<vector<string>> expectedProcedure = {{varA}};
      optional<vector<string>> expectedVariable = {{varB, varC, varD}};
      optional<vector<int>> expectedReadStmt = {{1}};
      optional<vector<int>> expectedPrintStmt = {{2}};
      optional<vector<int>> expectedCallStmt = {{3}};

      Assert::IsTrue(UtilsFunction::isOptionalVectorEqual(expectedProcedure, testProcedure));
      Assert::IsTrue(UtilsFunction::isOptionalVectorEqual(expectedVariable, testVariable));
      Assert::IsTrue(UtilsFunction::isOptionalVectorEqual(expectedReadStmt, testReadStmt));
      Assert::IsTrue(UtilsFunction::isOptionalVectorEqual(expectedPrintStmt, testPrintStmt));
      Assert::IsTrue(UtilsFunction::isOptionalVectorEqual(expectedCallStmt, testCallStmt));
    }

    TEST_METHOD(TestExtractOneVarStatementWithNesting) {
      ///*
      // *  procedure a {
      // * 1. while (e >= 1) {
      // * 2.   read b;
      // * 3.   print c;
      // * 4.   call d;
      // *    }
      // *  }
      // */
      tokenList = {
        tokenProcedure, tokenA, tokenOpenBrace,
        tokenWhile, tokenOpenBracket, tokenE, tokenGreaterEqual, tokenConstant, tokenCloseBracket,
        tokenOpenBrace,
        tokenRead, tokenB, tokenSemiColon,
        tokenPrint, tokenC, tokenSemiColon,
        tokenCall, tokenD, tokenSemiColon,
        tokenCloseBrace, tokenCloseBrace
      };
      Stream<Token> tokenStream = Stream<Token>();
      for (auto& token : tokenList) {
        tokenStream.pushBack(token);
      }
      PKBManager* pkbManager = new PKB();
      auto parser = SpParser(tokenStream);
      vector<ProcedureStatement> procedureList = parser.parse();
      Assert::IsTrue(procedureList.size() == 1);

      DesignExtractor designExtractor = DesignExtractor(*pkbManager, procedureList);
      designExtractor.extractRelationship();
      QueryResult procedureRes = pkbManager->getEntity(PROCEDURE);
      QueryResult variablesRes = pkbManager->getEntity(VARIABLE);
      QueryResult readStmtRes = pkbManager->getEntity(READ);
      QueryResult printStmtRes = pkbManager->getEntity(PRINT);
      QueryResult callStmtRes = pkbManager->getEntity(CALL);

      optional<vector<string>> testProcedure = procedureRes.getNames();
      optional<vector<string>> testVariable = variablesRes.getNames();
      optional<vector<int>> testReadStmt = readStmtRes.getLineNumbers();
      optional<vector<int>> testPrintStmt = printStmtRes.getLineNumbers();
      optional<vector<int>> testCallStmt = callStmtRes.getLineNumbers();

      optional<vector<string>> expectedProcedure = {{varA}};
      optional<vector<string>> expectedVariable = {{varB, varC, varD, varE}};
      optional<vector<int>> expectedReadStmt = {{2}};
      optional<vector<int>> expectedPrintStmt = {{3}};
      optional<vector<int>> expectedCallStmt = {{4}};

      Assert::IsTrue(UtilsFunction::isOptionalVectorEqual(expectedProcedure, testProcedure));
      Assert::IsTrue(UtilsFunction::isOptionalVectorEqual(expectedVariable, testVariable));
      Assert::IsTrue(UtilsFunction::isOptionalVectorEqual(expectedReadStmt, testReadStmt));
      Assert::IsTrue(UtilsFunction::isOptionalVectorEqual(expectedPrintStmt, testPrintStmt));
      Assert::IsTrue(UtilsFunction::isOptionalVectorEqual(expectedCallStmt, testCallStmt));
    }

    TEST_METHOD(TestExtractAssignStatementWithNoNesting) {
      ///*
      // *  procedure a {
      // * 1. d = b - b * c;
      // *  }
      // */
      tokenList = {
        tokenProcedure, tokenA, tokenOpenBrace,
        tokenD, tokenAssign, tokenB, tokenMinusOp, tokenB, tokenMultiply, tokenC,
        tokenSemiColon,tokenCloseBrace
      };
      Stream<Token> tokenStream = Stream<Token>();
      for (auto& token : tokenList) {
        tokenStream.pushBack(token);
      }
      PKBManager* pkbManager = new PKB();
      auto parser = SpParser(tokenStream);
      vector<ProcedureStatement> procedureList = parser.parse();
      Assert::IsTrue(procedureList.size() == 1);

      DesignExtractor designExtractor = DesignExtractor(*pkbManager, procedureList);
      designExtractor.extractRelationship();
      QueryResult procedureRes = pkbManager->getEntity(PROCEDURE);
      QueryResult variablesRes = pkbManager->getEntity(VARIABLE);
      QueryResult assignStmtRes = pkbManager->getEntity(ASSIGN);

      PqlArgument firstArg(ArgumentType::VARIABLE_NAME, varD, {});
      std::vector<Token> tokens = {tokenB, tokenMinusOp, tokenB, tokenMultiply, tokenC};
      Pattern pattern(PatternType::EXACT, tokens);
      QueryResult patternStatementRes = pkbManager->getPattern(PKBQueryArg(firstArg), pattern);

      Assert::IsTrue(patternStatementRes.getQueryResultType() == TUPLE);

      optional<vector<string>> testProcedure = procedureRes.getNames();
      optional<vector<string>> testVariable = variablesRes.getNames();
      optional<vector<int>> testAssignStmt = assignStmtRes.getLineNumbers();
      optional<vector<pair<int, string>>> testPatternStmt = patternStatementRes.getLineNumberVariablePairs();

      optional<vector<string>> expectedProcedure = {{varA}};
      optional<vector<string>> expectedVariable = {{varB, varC, varD}};
      optional<vector<int>> expectedAssignStmt = {{1}};
      optional<vector<pair<int, string>>> expectedPatternStmt = {{{1, varD}}};

      Assert::IsTrue(UtilsFunction::isOptionalVectorEqual(expectedProcedure, testProcedure));
      Assert::IsTrue(UtilsFunction::isOptionalVectorEqual(expectedVariable, testVariable));
      Assert::IsTrue(UtilsFunction::isOptionalVectorEqual(expectedAssignStmt, testAssignStmt));
      Assert::IsTrue(UtilsFunction::isOptionalVectorEqual(expectedPatternStmt, testPatternStmt));
    }

    TEST_METHOD(TestExtractIfStatementWithNoNesting) {
      ///*
      // *  procedure a {
      // * 1. if (b >= 1) then { } else { }
      // *  }
      // */
      tokenList = {
        tokenProcedure, tokenA, tokenOpenBrace,
        tokenIf, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenConstant, tokenCloseBrace,
        tokenThen, tokenOpenBrace, tokenCloseBrace, tokenElse, tokenOpenBrace, tokenCloseBrace,
        tokenCloseBrace
      };
      Stream<Token> tokenStream = Stream<Token>();
      for (auto& token : tokenList) {
        tokenStream.pushBack(token);
      }
      PKBManager* pkbManager = new PKB();
      auto parser = SpParser(tokenStream);
      vector<ProcedureStatement> procedureList = parser.parse();
      Assert::IsTrue(procedureList.size() == 1);

      DesignExtractor designExtractor = DesignExtractor(*pkbManager, procedureList);
      designExtractor.extractRelationship();
      QueryResult procedureRes = pkbManager->getEntity(PROCEDURE);
      QueryResult variablesRes = pkbManager->getEntity(VARIABLE);
      QueryResult constantRes = pkbManager->getEntity(CONSTANT);
      QueryResult ifStmtRes = pkbManager->getEntity(IF);

      optional<vector<string>> testProcedure = procedureRes.getNames();
      optional<vector<string>> testVariable = variablesRes.getNames();
      optional<vector<string>> testConstant = constantRes.getNames();
      optional<vector<int>> testIfStmt = ifStmtRes.getLineNumbers();

      optional<vector<string>> expectedProcedure = {{varA}};
      optional<vector<string>> expectedVariable = {{varB}};
      optional<vector<string>> expectedConstant = {{constant}};
      optional<vector<int>> expectedIfStmt = {{1}};

      Assert::IsTrue(UtilsFunction::isOptionalVectorEqual(expectedProcedure, testProcedure));
      Assert::IsTrue(UtilsFunction::isOptionalVectorEqual(expectedVariable, testVariable));
      Assert::IsTrue(UtilsFunction::isOptionalVectorEqual(expectedConstant, testConstant));
      Assert::IsTrue(UtilsFunction::isOptionalVectorEqual(expectedIfStmt, testIfStmt));
    }

    TEST_METHOD(TestExtractWhileStatementWithNoNesting) {
      ///*
      // *  procedure a {
      // * 1. while (b >= 1) { }
      // *  }
      // */
      tokenList = {
        tokenProcedure, tokenA, tokenOpenBrace,
        tokenWhile, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenConstant, tokenCloseBracket,
        tokenOpenBrace, tokenCloseBrace,
        tokenCloseBrace
      };
      Stream<Token> tokenStream = Stream<Token>();
      for (auto token : tokenList) {
        tokenStream.pushBack(token);
      }
      PKBManager* pkbManager = new PKB();
      auto parser = SpParser(tokenStream);
      vector<ProcedureStatement> procedureList = parser.parse();
      Assert::IsTrue(procedureList.size() == 1);

      DesignExtractor designExtractor = DesignExtractor(*pkbManager, procedureList);
      designExtractor.extractRelationship();
      QueryResult procedureRes = pkbManager->getEntity(PROCEDURE);
      QueryResult variablesRes = pkbManager->getEntity(VARIABLE);
      QueryResult constantRes = pkbManager->getEntity(CONSTANT);
      QueryResult whileStmtRes = pkbManager->getEntity(WHILE);

      optional<vector<string>> testProcedure = procedureRes.getNames();
      optional<vector<string>> testVariable = variablesRes.getNames();
      optional<vector<string>> testConstant = constantRes.getNames();
      optional<vector<int>> testIfStmt = whileStmtRes.getLineNumbers();

      optional<vector<string>> expectedProcedure = {{varA}};
      optional<vector<string>> expectedVariable = {{varB}};
      optional<vector<string>> expectedConstant = {{constant}};
      optional<vector<int>> expectedWhileStmt = {{1}};

      Assert::IsTrue(UtilsFunction::isOptionalVectorEqual(expectedProcedure, testProcedure));
      Assert::IsTrue(UtilsFunction::isOptionalVectorEqual(expectedVariable, testVariable));
      Assert::IsTrue(UtilsFunction::isOptionalVectorEqual(expectedWhileStmt, testIfStmt));
    }

    TEST_METHOD(TestExtractUsesSingleAssignment) {
      ///*
      // *  procedure a {
      // * 1. c = d - b;
      // *  }
      // */
      tokenList = {
        tokenProcedure, tokenA, tokenOpenBrace,
        tokenC, tokenAssign, tokenD, tokenMinusOp, tokenB, tokenSemiColon,
        tokenCloseBrace
      };
      Stream<Token> tokenStream = Stream<Token>();
      for (auto token : tokenList) {
        tokenStream.pushBack(token);
      }
      PKBManager* pkbManager = new PKB();
      auto parser = SpParser(tokenStream);
      vector<ProcedureStatement> procedureList = parser.parse();
      Assert::IsTrue(procedureList.size() == 1);

      DesignExtractor designExtractor = DesignExtractor(*pkbManager, procedureList);
      designExtractor.extractRelationship();

      PqlArgument lineNum(ArgumentType::LINE_NO, "1", {});
      PqlArgument variable(ArgumentType::VARIABLE_NAME, varD, {});
      QueryResult usesRes = pkbManager->getRelationship(USES, PKBQueryArg(lineNum), 
      PKBQueryArg(variable));

      Assert::IsTrue(usesRes.getQueryResultType() == BOOL);

      bool testResult = usesRes.getIsTrue();
      bool expectedResult = true;
      Assert::IsTrue(expectedResult == testResult);

      //optional<vector<string>> testProcedure = procedureRes.getNames();
      //optional<vector<string>> testVariable = variablesRes.getNames();
      //optional<vector<string>> testConstant = constantRes.getNames();
      //optional<vector<int>> testIfStmt = whileStmtRes.getLineNumbers();

      //optional<vector<string>> expectedProcedure = {{varA}};
      //optional<vector<string>> expectedVariable = {{varB}};
      //optional<vector<string>> expectedConstant = {{constant}};
      //optional<vector<int>> expectedWhileStmt = {{1}};

      //Assert::IsTrue(UtilsFunction::isOptionalVectorEqual(expectedProcedure, testProcedure));
      //Assert::IsTrue(UtilsFunction::isOptionalVectorEqual(expectedVariable, testVariable));
      //Assert::IsTrue(UtilsFunction::isOptionalVectorEqual(expectedWhileStmt, testIfStmt));
    }

    //TEST_METHOD(TestExtractFollowsWithNoNesting) {
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
    //tokenList = {
    //  tokenProcedure, tokenA, tokenOpenBrace,
    //  tokenB, tokenAssign, tokenConstant, tokenSemiColon,
    //  tokenC, tokenAssign, tokenB, tokenSemiColon,
    //  tokenWhile, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenConstant, tokenCloseBracket,
    //  tokenOpenBrace, tokenD, tokenAssign, tokenB, tokenMinusOp, tokenC, tokenSemiColon,
    //  tokenC, tokenAssign, tokenD, tokenMinusOp, tokenB, tokenSemiColon,
    //  tokenCloseBrace, tokenCloseBrace
    //};
    //Stream<Token> tokenStream = Stream<Token>();
    //for (auto token: tokenList) {
    //  tokenStream.pushBack(token);
    //}

    //PKBManager* pkbManager = new PKB();
    //auto parser = SpParser(tokenStream);
    //vector<ProcedureStatement> procedureList = parser.parse();
    //Assert::IsTrue(procedureList.size() == 1);
    //optional<DesignEntityType> designEntity;
    //PqlArgument pqlArgOne = PqlArgument(LINE_NO, "2", designEntity);
    //PqlArgument pqlArgtwo = PqlArgument(LINE_NO, "3", designEntity);
    //QueryResult results = pkbManager->getRelationship(FOLLOWS, PKBQueryArg(pqlArgOne), PKBQueryArg(pqlArgtwo));
    //}
  };
} // namespace UnitTesting
