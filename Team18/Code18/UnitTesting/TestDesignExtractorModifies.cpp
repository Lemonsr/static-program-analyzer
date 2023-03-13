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
TEST_CLASS(TestDesignExtractorModifies) {
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
  TEST_METHOD(TestExtractModifiesSingleAssignment) {
    /*
     *  procedure a {
     * 1. c = d;
     *  }
     */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenC, tokenAssign, tokenD, tokenSemiColon,
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

    spa::PqlArgument lineNum(spa::ArgumentType::LINE_NO, "1", {});
    spa::PqlArgument variable(spa::ArgumentType::LITERAL_STRING, varC, {});
    spa::QueryResult modifiesRes = pkbManager->getRelationship(spa::MODIFIES,
      spa::PKBQueryArg(lineNum),
      spa::PKBQueryArg(variable));

    Assert::IsTrue(modifiesRes.getQueryResultType() == spa::BOOL);

    bool testModifies = modifiesRes.getIsTrue();

    bool expectedModifies = true;

    Assert::IsTrue(expectedModifies == testModifies);

    std::vector<std::pair<std::string, std::string>> negResTestCases = {{"1", varD}};

    for (auto pair : negResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::MODIFIES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testModifies = results.getIsTrue();
      Assert::IsFalse(testModifies);
    }
  }

  TEST_METHOD(TestExtractModifiesIfNestedAssignment) {
    /*
     *  procedure a {
     * 1. if (e >= 1) then {
     * 2.   c = d;
     *    } else {
     * 3.   b = d;
     *    }
     *  }
     */
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
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    auto parser = spa::SpParser(tokenStream);
    std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();
    Assert::IsTrue(procedureList.size() == 1);

    spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
    designExtractor.extractRelationship();

    std::vector<std::pair<std::string, std::string>> positiveResTestCases = {
      {"1", varC}, {"1", varB}, {"2", varC}, {"3", varB}
    };

    for (auto pair : positiveResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::MODIFIES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testModifies = results.getIsTrue();
      Assert::IsTrue(testModifies);
    }

    std::vector<std::pair<std::string, std::string>> negResTestCases = {
      {"1", varE}, {"1", varD}, {"2", varD}, {"3", varD}
    };

    for (auto pair : negResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::MODIFIES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testModifies = results.getIsTrue();
      Assert::IsFalse(testModifies);
    }
  }

  TEST_METHOD(TestExtractModifiesIfNestedThenAssignment) {
    /*
     *  procedure a {
     * 1. if (e >= 1) then {
     * 2.   c = g;
     * 3.   if (e >= 1) then {
     * 4.     f = g;
     *      } else {
     * 5.     d = g;
     *      }
     *    } else {
     * 6.   b = g;
     *    }
     *  }
     */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenIf, tokenOpenBracket, tokenE, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenThen, tokenOpenBrace,
      tokenC, tokenAssign, tokenG, tokenSemiColon,
      tokenIf, tokenOpenBracket, tokenE, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenThen, tokenOpenBrace,
      tokenF, tokenAssign, tokenG, tokenSemiColon,
      tokenCloseBrace, tokenElse, tokenOpenBrace,
      tokenD, tokenAssign, tokenG, tokenSemiColon,
      tokenCloseBrace, tokenCloseBrace, tokenElse, tokenOpenBrace,
      tokenB, tokenAssign, tokenG, tokenSemiColon,
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

    std::vector<std::pair<std::string, std::string>> positiveResTestCases = {
      {"1", varC}, {"1", varF}, {"1", varD}, {"1", varB}, {"2", varC},
      {"3", varF}, {"3", varD},
      {"4", varF}, {"5", varD}, {"6", varB}
    };

    for (auto pair : positiveResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::MODIFIES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testModifies = results.getIsTrue();
      Assert::IsTrue(testModifies);
    }

    std::vector<std::pair<std::string, std::string>> negResTestCases = {
      {"1", varE}, {"1", varG}, {"2", varG}, {"3", varE}, {"3", varE},
      {"3", varG}, {"4", varG},
      {"6", varG}, {"6", varG}
    };

    for (auto pair : negResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::MODIFIES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testModifies = results.getIsTrue();
      Assert::IsFalse(testModifies);
    }
  }

  TEST_METHOD(TestExtractModifiesIfNestedElseAssignment) {
    /*
     *  procedure a {
     * 1. if (e >= 1) then {
     * 2.   c = g;
     *    } else {
     * 3.   if (e >= 1) then {
     * 4.     f = g;
     *      } else {
     * 5.     d = g;
     *      }
     * 6.   b = g;
     *    }
     *  }
     */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenIf, tokenOpenBracket, tokenE, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenThen, tokenOpenBrace,
      tokenC, tokenAssign, tokenG, tokenSemiColon,
      tokenCloseBrace, tokenElse, tokenOpenBrace,
      tokenIf, tokenOpenBracket, tokenE, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenThen, tokenOpenBrace,
      tokenF, tokenAssign, tokenG, tokenSemiColon,
      tokenCloseBrace, tokenElse, tokenOpenBrace,
      tokenD, tokenAssign, tokenG, tokenSemiColon,
      tokenCloseBrace,
      tokenB, tokenAssign, tokenG, tokenSemiColon,
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

    std::vector<std::pair<std::string, std::string>> positiveResTestCases = {
      {"1", varC}, {"1", varF}, {"1", varD}, {"1", varB}, {"2", varC},
      {"3", varF}, {"3", varD},
      {"4", varF}, {"5", varD}, {"6", varB}
    };

    for (auto pair : positiveResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::MODIFIES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testModifies = results.getIsTrue();
      Assert::IsTrue(testModifies);
    }

    std::vector<std::pair<std::string, std::string>> negResTestCases = {
      {"1", varE}, {"1", varG}, {"2", varG}, {"3", varE}, {"3", varE},
      {"3", varG}, {"4", varG},
      {"6", varG}, {"6", varG}
    };

    for (auto pair : negResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::MODIFIES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testModifies = results.getIsTrue();
      Assert::IsFalse(testModifies);
    }
  }

  TEST_METHOD(TestExtractModifiesWhileNestedAssignment) {
    /*
     *  procedure a {
     * 1. while (e >= 1) {
     * 2.   c = d;
     * 3.   b = d;
     *    }
     *  }
     */
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
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    auto parser = spa::SpParser(tokenStream);
    std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();
    Assert::IsTrue(procedureList.size() == 1);

    spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
    designExtractor.extractRelationship();

    std::vector<std::pair<std::string, std::string>> positiveResTestCases = {
      {"1", varC}, {"1", varB}, {"2", varC}, {"3", varB}
    };

    for (auto pair : positiveResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first, {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second, {});
      spa::QueryResult results = pkbManager->getRelationship(spa::MODIFIES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testModifies = results.getIsTrue();
      Assert::IsTrue(testModifies);
    }

    std::vector<std::pair<std::string, std::string>> negResTestCases = {
      {"1", varD}, {"1", varE}, {"2", varD}, {"3", varD}
    };

    for (auto pair : negResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first, {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second, {});
      spa::QueryResult results = pkbManager->getRelationship(spa::MODIFIES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testModifies = results.getIsTrue();
      Assert::IsFalse(testModifies);
    }
  }

  TEST_METHOD(TestExtractModifiesWhileNestedWhileAssignment) {
    /*
     *  procedure a {
     * 1. while (e >= 1) {
     * 2.   c = d;
     * 3.   while (e >= 1) {
     * 4.     f = d;
     * 5.     g = d;
     *      }
     *    }
     *  }
     */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenWhile, tokenOpenBracket, tokenE, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenOpenBrace,
      tokenC, tokenAssign, tokenD, tokenSemiColon,
      tokenWhile, tokenOpenBracket, tokenE, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenOpenBrace,
      tokenF, tokenAssign, tokenD, tokenSemiColon,
      tokenG, tokenAssign, tokenD, tokenSemiColon,
      tokenCloseBrace, tokenCloseBrace, tokenCloseBrace
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

    std::vector<std::pair<std::string, std::string>> positiveResTestCases = {
      {"1", varC}, {"1", varF}, {"1", varG}, {"2", varC}, {"3", varF},
      {"3", varG}, {"4", varF},
      {"5", varG}
    };

    for (auto pair : positiveResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::MODIFIES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testModifies = results.getIsTrue();
      Assert::IsTrue(testModifies);
    }

    std::vector<std::pair<std::string, std::string>> negResTestCases = {
      {"1", varD}, {"1", varE}, {"3", varD}, {"3", varE}, {"4", varD},
      {"5", varD}
    };

    for (auto pair : negResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::MODIFIES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testModifies = results.getIsTrue();
      Assert::IsFalse(testModifies);
    }
  }

  TEST_METHOD(TestExtractModifiesIfNestedWhileAssignment) {
    /*
     *  procedure a {
     * 1. while (e >= 1) {
     * 2.   c = d;
     * 3.   if (e >= 1) then {
     * 4.     f = g;
     *      } else {
     * 5.     d = g;
     *      }
     *    }
     *  }
     */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenWhile, tokenOpenBracket, tokenE, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenOpenBrace,
      tokenC, tokenAssign, tokenD, tokenSemiColon,
      tokenIf, tokenOpenBracket, tokenE, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenThen, tokenOpenBrace,
      tokenF, tokenAssign, tokenG, tokenSemiColon,
      tokenCloseBrace, tokenElse, tokenOpenBrace,
      tokenD, tokenAssign, tokenG, tokenSemiColon,
      tokenCloseBrace, tokenCloseBrace, tokenCloseBrace
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

    std::vector<std::pair<std::string, std::string>> positiveResTestCases = {
      {"1", varC}, {"1", varF}, {"1", varD}, {"2", varC}, {"3", varF},
      {"3", varD}, {"4", varF},
      {"5", varD}
    };

    for (auto pair : positiveResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::MODIFIES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testModifies = results.getIsTrue();
      Assert::IsTrue(testModifies);
    }

    std::vector<std::pair<std::string, std::string>> negResTestCases = {
      {"1", varE}, {"1", varG}, {"3", varE}, {"3", varG}, {"4", varG},
      {"5", varG}
    };

    for (auto pair : negResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first, {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second, {});
      spa::QueryResult results = pkbManager->getRelationship(spa::MODIFIES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testModifies = results.getIsTrue();
      Assert::IsFalse(testModifies);
    }
  }

  TEST_METHOD(TestExtractModifiesWhileNestedThenAssignment) {
    /*
     *  procedure a {
     * 1. if (e >= 1) then {
     * 2.   c = g;
     * 3.   while (e >= 1) {
     * 4.     f = d;
     * 5.     g = d;
     *      }
     *    } else {
     * 6.   b = g;
     *    }
     *  }
     */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenIf, tokenOpenBracket, tokenE, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenThen, tokenOpenBrace,
      tokenC, tokenAssign, tokenG, tokenSemiColon,
      tokenWhile, tokenOpenBracket, tokenE, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenOpenBrace,
      tokenF, tokenAssign, tokenD, tokenSemiColon,
      tokenG, tokenAssign, tokenD, tokenSemiColon,
      tokenCloseBrace, tokenCloseBrace, tokenElse, tokenOpenBrace,
      tokenB, tokenAssign, tokenG, tokenSemiColon,
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

    std::vector<std::pair<std::string, std::string>> positiveResTestCases = {
      {"1", varC}, {"1", varF}, {"1", varG}, {"1", varB}, {"2", varC},
      {"3", varF}, {"3", varG},
      {"4", varF}, {"5", varG}, {"6", varB}
    };

    for (auto pair : positiveResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::MODIFIES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testModifies = results.getIsTrue();
      Assert::IsTrue(testModifies);
    }

    std::vector<std::pair<std::string, std::string>> negResTestCases = {
      {"1", varD}, {"1", varE}, {"3", varD}, {"3", varE}, {"4", varD},
      {"5", varD},
      {"6", varG}
    };

    for (auto pair : negResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::MODIFIES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testModifies = results.getIsTrue();
      Assert::IsFalse(testModifies);
    }
  }

  TEST_METHOD(TestExtractModifiesWhileNestedElseAssignment) {
    /*
     *  procedure a {
     * 1. if (e >= 1) then {
     * 2.   c = g;
     *    } else {
     * 3.   while (e >= 1) {
     * 4.     f = d;
     * 5.     g = d;
     *      }
     * 6.   b = g;
     *    }
     *  }
     */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenIf, tokenOpenBracket, tokenE, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenThen, tokenOpenBrace,
      tokenC, tokenAssign, tokenG, tokenSemiColon,
      tokenCloseBrace, tokenElse, tokenOpenBrace,
      tokenWhile, tokenOpenBracket, tokenE, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenOpenBrace,
      tokenF, tokenAssign, tokenD, tokenSemiColon,
      tokenG, tokenAssign, tokenD, tokenSemiColon,
      tokenCloseBrace,
      tokenB, tokenAssign, tokenG, tokenSemiColon,
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

    std::vector<std::pair<std::string, std::string>> positiveResTestCases = {
      {"1", varC}, {"1", varF}, {"1", varG}, {"1", varB}, {"2", varC}, {"3", varF},
      {"3", varG},
      {"4", varF}, {"5", varG}, {"6", varB}
    };

    for (auto pair : positiveResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::MODIFIES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testModifies = results.getIsTrue();
      Assert::IsTrue(testModifies);
    }

    std::vector<std::pair<std::string, std::string>> negResTestCases = {
      {"1", varD}, {"1", varE}, {"3", varD}, {"3", varE}, {"4", varD},
      {"5", varD},
      {"6", varG}
    };

    for (auto pair : negResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO,
        pair.first, {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING,
        pair.second, {});
      spa::QueryResult results = pkbManager->getRelationship(spa::MODIFIES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testModifies = results.getIsTrue();
      Assert::IsFalse(testModifies);
    }
  }

  TEST_METHOD(TestExtractModifiesReadStatement) {
    /*
     *  procedure a {
     * 1. read c;
     *  }
     */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenC, tokenAssign, tokenD, tokenSemiColon,
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

    spa::PqlArgument lineNum(spa::ArgumentType::LINE_NO, "1", {});
    spa::PqlArgument variable(spa::ArgumentType::LITERAL_STRING, varC, {});
    spa::QueryResult modifiesRes = pkbManager->getRelationship(spa::MODIFIES,
      spa::PKBQueryArg(lineNum),
      spa::PKBQueryArg(variable));

    Assert::IsTrue(modifiesRes.getQueryResultType() == spa::BOOL);

    bool testModifies = modifiesRes.getIsTrue();

    bool expectedModifies = true;

    Assert::IsTrue(expectedModifies == testModifies);
  }


  TEST_METHOD(TestExtractModifiesIfNestedReadStatement) {
    /*
     *  procedure a {
     * 1. if (e >= 1) then {
     * 2.   read c;
     *    } else {
     * 3.   read b;
     *    }
     *  }
     */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenIf, tokenOpenBracket, tokenE, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenThen, tokenOpenBrace,
      tokenRead, tokenC, tokenSemiColon,
      tokenCloseBrace,
      tokenElse, tokenOpenBrace,
      tokenRead, tokenB, tokenSemiColon,
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

    std::vector<std::pair<std::string, std::string>> positiveResTestCases = {
      {"1", varC}, {"1", varB}, {"2", varC}, {"3", varB}
    };

    for (auto pair : positiveResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::MODIFIES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testModifies = results.getIsTrue();
      Assert::IsTrue(testModifies);
    }

    std::vector<std::pair<std::string, std::string>> negResTestCases = {
      {"1", varE}
    };

    for (auto pair : negResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::MODIFIES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testModifies = results.getIsTrue();
      Assert::IsFalse(testModifies);
    }
  }

  TEST_METHOD(TestExtractModifiesIfNestedThenReadStatement) {
    /*
     *  procedure a {
     * 1. if (e >= 1) then {
     * 2.   read c;
     * 3.   if (e >= 1) then {
     * 4.     read f;
     *      } else {
     * 5.     read d;
     *      }
     *    } else {
     * 6.   read b;
     *    }
     *  }
     */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenIf, tokenOpenBracket, tokenE, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenThen, tokenOpenBrace,
      tokenRead, tokenC, tokenSemiColon,
      tokenIf, tokenOpenBracket, tokenE, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenThen, tokenOpenBrace,
      tokenRead, tokenF, tokenSemiColon,
      tokenCloseBrace, tokenElse, tokenOpenBrace,
      tokenRead, tokenD, tokenSemiColon,
      tokenCloseBrace, tokenCloseBrace, tokenElse, tokenOpenBrace,
      tokenRead, tokenB, tokenSemiColon,
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

    std::vector<std::pair<std::string, std::string>> positiveResTestCases = {
      {"1", varC}, {"1", varF}, {"1", varD}, {"1", varB}, {"2", varC},
      {"3", varF}, {"3", varD},
      {"4", varF}, {"5", varD}, {"6", varB}
    };

    for (auto pair : positiveResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::MODIFIES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testModifies = results.getIsTrue();
      Assert::IsTrue(testModifies);
    }

    std::vector<std::pair<std::string, std::string>> negResTestCases = {
      {"1", varE}
    };

    for (auto pair : negResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::MODIFIES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testModifies = results.getIsTrue();
      Assert::IsFalse(testModifies);
    }
  }

  TEST_METHOD(TestExtractModifiesIfNestedElseReadStatement) {
    /*
     *  procedure a {
     * 1. if (e >= 1) then {
     * 2.   read c;
     *    } else {
     * 3.   if (e >= 1) then {
     * 4.     read f;
     *      } else {
     * 5.     read d;
     *      }
     * 6.   read b;
     *    }
     *  }
     */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenIf, tokenOpenBracket, tokenE, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenThen, tokenOpenBrace,
      tokenRead, tokenC, tokenSemiColon,
      tokenCloseBrace, tokenElse, tokenOpenBrace,
      tokenIf, tokenOpenBracket, tokenE, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenThen, tokenOpenBrace,
      tokenRead, tokenF, tokenSemiColon,
      tokenCloseBrace, tokenElse, tokenOpenBrace,
      tokenRead, tokenD, tokenSemiColon,
      tokenCloseBrace,
      tokenRead, tokenB, tokenSemiColon,
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

    std::vector<std::pair<std::string, std::string>> positiveResTestCases = {
      {"1", varC}, {"1", varF}, {"1", varD}, {"1", varB}, {"2", varC},
      {"3", varF}, {"3", varD},
      {"4", varF}, {"5", varD}, {"6", varB}
    };

    for (auto pair : positiveResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::MODIFIES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testModifies = results.getIsTrue();
      Assert::IsTrue(testModifies);
    }

    std::vector<std::pair<std::string, std::string>> negResTestCases = {
      {"1", varE}
    };

    for (auto pair : negResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::MODIFIES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testModifies = results.getIsTrue();
      Assert::IsFalse(testModifies);
    }
  }

  TEST_METHOD(TestExtractModifiesWhileNestedReadStatement) {
    /*
     *  procedure a {
     * 1. while (e >= 1) {
     * 2.   c = d;
     * 3.   b = d;
     *    }
     *  }
     */
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
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    auto parser = spa::SpParser(tokenStream);
    std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();
    Assert::IsTrue(procedureList.size() == 1);

    spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
    designExtractor.extractRelationship();

    std::vector<std::pair<std::string, std::string>> positiveResTestCases = {
      {"1", varC}, {"1", varB}, {"2", varC}, {"3", varB}
    };

    for (auto pair : positiveResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first, {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second, {});
      spa::QueryResult results = pkbManager->getRelationship(spa::MODIFIES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testModifies = results.getIsTrue();
      Assert::IsTrue(testModifies);
    }

    std::vector<std::pair<std::string, std::string>> negResTestCases = {
      {"1", varD}, {"1", varE}, {"2", varD}, {"3", varD}
    };

    for (auto pair : negResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first, {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second, {});
      spa::QueryResult results = pkbManager->getRelationship(spa::MODIFIES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testModifies = results.getIsTrue();
      Assert::IsFalse(testModifies);
    }
  }

  TEST_METHOD(TestExtractModifiesWhileNestedWhileReadStatement) {
    /*
     *  procedure a {
     * 1. while (e >= 1) {
     * 2.   read c;
     * 3.   while (e >= 1) {
     * 4.     read f;
     * 5.     read g;
     *      }
     *    }
     *  }
     */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenWhile, tokenOpenBracket, tokenE, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenOpenBrace,
      tokenRead, tokenC, tokenSemiColon,
      tokenWhile, tokenOpenBracket, tokenE, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenOpenBrace,
      tokenRead, tokenF, tokenSemiColon,
      tokenRead, tokenG, tokenSemiColon,
      tokenCloseBrace, tokenCloseBrace, tokenCloseBrace
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

    std::vector<std::pair<std::string, std::string>> positiveResTestCases = {
      {"1", varC}, {"1", varF}, {"1", varG}, {"2", varC}, {"3", varF},
      {"3", varG}, {"4", varF},
      {"5", varG}
    };

    for (auto pair : positiveResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::MODIFIES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testModifies = results.getIsTrue();
      Assert::IsTrue(testModifies);
    }

    std::vector<std::pair<std::string, std::string>> negResTestCases = {
      {"1", varE}
    };

    for (auto pair : negResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::MODIFIES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testModifies = results.getIsTrue();
      Assert::IsFalse(testModifies);
    }
  }

  TEST_METHOD(TestExtractModifiesIfNestedWhileReadStatement) {
    /*
     *  procedure a {
     * 1. while (e >= 1) {
     * 2.   read c;
     * 3.   if (e >= 1) then {
     * 4.     read f;
     *      } else {
     * 5.     read d;
     *      }
     *    }
     *  }
     */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenWhile, tokenOpenBracket, tokenE, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenOpenBrace,
      tokenRead, tokenC, tokenSemiColon,
      tokenIf, tokenOpenBracket, tokenE, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenThen, tokenOpenBrace,
      tokenRead, tokenF, tokenSemiColon,
      tokenCloseBrace, tokenElse, tokenOpenBrace,
      tokenRead, tokenD, tokenSemiColon,
      tokenCloseBrace, tokenCloseBrace, tokenCloseBrace
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

    std::vector<std::pair<std::string, std::string>> positiveResTestCases = {
      {"1", varC}, {"1", varF}, {"1", varD}, {"2", varC}, {"3", varF},
      {"3", varD}, {"4", varF},
      {"5", varD}
    };

    for (auto pair : positiveResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::MODIFIES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testModifies = results.getIsTrue();
      Assert::IsTrue(testModifies);
    }

    std::vector<std::pair<std::string, std::string>> negResTestCases = {
      {"1", varE}
    };

    for (auto pair : negResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first, {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second, {});
      spa::QueryResult results = pkbManager->getRelationship(spa::MODIFIES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testModifies = results.getIsTrue();
      Assert::IsFalse(testModifies);
    }
  }

  TEST_METHOD(TestExtractModifiesWhileNestedThenReadStatement) {
    /*
     *  procedure a {
     * 1. if (e >= 1) then {
     * 2.   read c;
     * 3.   while (e >= 1) {
     * 4.     read f;
     * 5.     read g;
     *      }
     *    } else {
     * 6.   read b;
     *    }
     *  }
     */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenIf, tokenOpenBracket, tokenE, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenThen, tokenOpenBrace,
      tokenRead, tokenC, tokenSemiColon,
      tokenWhile, tokenOpenBracket, tokenE, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenOpenBrace,
      tokenRead, tokenF, tokenSemiColon,
      tokenRead, tokenG, tokenSemiColon,
      tokenCloseBrace, tokenCloseBrace, tokenElse, tokenOpenBrace,
      tokenRead, tokenB, tokenSemiColon,
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

    std::vector<std::pair<std::string, std::string>> positiveResTestCases = {
      {"1", varC}, {"1", varF}, {"1", varG}, {"1", varB}, {"2", varC},
      {"3", varF}, {"3", varG},
      {"4", varF}, {"5", varG}, {"6", varB}
    };

    for (auto pair : positiveResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::MODIFIES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testModifies = results.getIsTrue();
      Assert::IsTrue(testModifies);
    }

    std::vector<std::pair<std::string, std::string>> negResTestCases = {
      {"1", varE}
    };

    for (auto pair : negResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::MODIFIES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testModifies = results.getIsTrue();
      Assert::IsFalse(testModifies);
    }
  }

  TEST_METHOD(TestExtractModifiesWhileNestedElseReadStatement) {
    /*
     *  procedure a {
     * 1. if (e >= 1) then {
     * 2.   read c;
     *    } else {
     * 3.   while (e >= 1) {
     * 4.     read f;
     * 5.     read g;
     *      }
     * 6.   read b;
     *    }
     *  }
     */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenIf, tokenOpenBracket, tokenE, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenThen, tokenOpenBrace,
      tokenRead, tokenC, tokenSemiColon,
      tokenCloseBrace, tokenElse, tokenOpenBrace,
      tokenWhile, tokenOpenBracket, tokenE, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenOpenBrace,
      tokenRead, tokenF, tokenSemiColon,
      tokenRead, tokenG, tokenSemiColon,
      tokenCloseBrace,
      tokenRead, tokenB, tokenSemiColon,
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

    std::vector<std::pair<std::string, std::string>> positiveResTestCases = {
      {"1", varC}, {"1", varF}, {"1", varG}, {"1", varB}, {"2", varC},
      {"3", varF},
      {"3", varG},
      {"4", varF}, {"5", varG}, {"6", varB}
    };

    for (auto pair : positiveResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::MODIFIES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testModifies = results.getIsTrue();
      Assert::IsTrue(testModifies);
    }

    std::vector<std::pair<std::string, std::string>> negResTestCases = {
      {"1", varE}
    };

    for (auto pair : negResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO,
        pair.first, {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING,
        pair.second, {});
      spa::QueryResult results = pkbManager->getRelationship(spa::MODIFIES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testModifies = results.getIsTrue();
      Assert::IsFalse(testModifies);
    }
  }

  TEST_METHOD(TestExtractModifiesSingleReadAssignment) {
    /*
     *  procedure a {
     * 1. c = d;
     * 2. read b;
     *  }
     */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenC, tokenAssign, tokenD, tokenSemiColon,
      tokenRead, tokenB, tokenSemiColon,
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

    std::vector<std::pair<std::string, std::string>> positiveResTestCases = {
      {"1", varC}, {"2", varB}
    };

    for (auto pair : positiveResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::MODIFIES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testModifies = results.getIsTrue();
      Assert::IsTrue(testModifies);
    }

    std::vector<std::pair<std::string, std::string>> negResTestCases = {{"1", varD}};

    for (auto pair : negResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::MODIFIES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testModifies = results.getIsTrue();
      Assert::IsFalse(testModifies);
    }
  }

  TEST_METHOD(TestExtractModifiesIfNestedReadAssignment) {
    /*
     *  procedure a {
     * 1. if (e >= 1) then {
     * 2.   c = d;
     * 3.   read f;
     *    } else {
     * 4.   b = d;
     * 5.   read g;
     *    }
     *  }
     */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenIf, tokenOpenBracket, tokenE, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenThen, tokenOpenBrace,
      tokenC, tokenAssign, tokenD, tokenSemiColon,
      tokenRead, tokenF, tokenSemiColon,
      tokenCloseBrace,
      tokenElse, tokenOpenBrace,
      tokenB, tokenAssign, tokenD, tokenSemiColon,
      tokenRead, tokenG, tokenSemiColon,
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

    std::vector<std::pair<std::string, std::string>> positiveResTestCases = {
      {"1", varC}, {"1", varB}, {"1", varF}, {"1", varG}, {"2", varC},
      {"4", varB}, {"3", varF},
      {"5", varG},
    };

    for (auto pair : positiveResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::MODIFIES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testModifies = results.getIsTrue();
      Assert::IsTrue(testModifies);
    }

    std::vector<std::pair<std::string, std::string>> negResTestCases = {
      {"1", varE}, {"1", varD}, {"2", varD}, {"4", varD}
    };

    for (auto pair : negResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::MODIFIES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testModifies = results.getIsTrue();
      Assert::IsFalse(testModifies);
    }
  }


  TEST_METHOD(TestExtractModifiesWhileNestedReadAssignment) {
    /*
     *  procedure a {
     * 1. while (e >= 1) {
     * 2.   read c;
     * 3.   b = d;
     *    }
     *  }
     */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenWhile, tokenOpenBracket, tokenE, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenOpenBrace,
      tokenRead, tokenC, tokenSemiColon,
      tokenB, tokenAssign, tokenD, tokenSemiColon,
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

    std::vector<std::pair<std::string, std::string>> positiveResTestCases = {
      {"1", varC}, {"1", varB}, {"2", varC}, {"3", varB}
    };

    for (auto pair : positiveResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::MODIFIES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testModifies = results.getIsTrue();
      Assert::IsTrue(testModifies);
    }

    std::vector<std::pair<std::string, std::string>> negResTestCases = {
      {"1", varD}, {"1", varE}, {"3", varD}
    };

    for (auto pair : negResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::MODIFIES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testModifies = results.getIsTrue();
      Assert::IsFalse(testModifies);
    }
  }
};
}  // namespace UnitTesting
