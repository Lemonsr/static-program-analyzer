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
  TEST_METHOD(TestExtractUsesSingleAssignment) {
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
    std::vector<spa::ProcedureStatement> procedureList = parser.parse();
    Assert::IsTrue(procedureList.size() == 1);

    spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
    designExtractor.extractRelationship();

    spa::PqlArgument lineNum(spa::ArgumentType::LINE_NO, "1", {});
    spa::PqlArgument variable(spa::ArgumentType::LITERAL_STRING, varD, {});
    spa::QueryResult usesRes = pkbManager->getRelationship(spa::USES, spa::PKBQueryArg(lineNum),
      spa::PKBQueryArg(variable));

    Assert::IsTrue(usesRes.getQueryResultType() == spa::BOOL);

    bool testUses = usesRes.getIsTrue();

    bool expectedUses = true;

    Assert::IsTrue(expectedUses == testUses);
  }

  TEST_METHOD(TestExtractUsesMultipleVarAssignment) {
    /*
     *  procedure a {
     * 1. c = d - b * e;
     *  }
     */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenC, tokenAssign, tokenD, tokenMinusOp, tokenB, tokenMultiply, tokenE,
      tokenSemiColon, tokenCloseBrace
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

    std::vector<std::pair<std::string, std::string>> positiveResTestCases = {
      {"1", varD}, {"1", varB}, {"1", varE}
    };

    for (auto pair : positiveResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::USES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testUSES = results.getIsTrue();
      Assert::IsTrue(testUSES);
    }

    std::vector<std::pair<std::string, std::string>> negResTestCases = {
      {"1", varC}
    };

    for (auto pair : negResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::USES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testUSES = results.getIsTrue();
      Assert::IsFalse(testUSES);
    }
  }

  TEST_METHOD(TestExtractUsesIfNestedAssignment) {
    /*
     *  procedure a {
     * 1. if (e >= 1) then {
     * 2.   c = d;
     *    } else {
     * 3.   c = b;
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
      tokenC, tokenAssign, tokenB, tokenSemiColon,
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

    std::vector<std::pair<std::string, std::string>> positiveResTestCases = {
      {"1", varD}, {"1", varB}, {"1", varE}, {"2", varD}, {"3", varB}
    };

    for (auto pair : positiveResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::USES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testUSES = results.getIsTrue();
      Assert::IsTrue(testUSES);
    }

    std::vector<std::pair<std::string, std::string>> negResTestCases = {
      {"1", varC}, {"2", varC}, {"3", varC}
    };

    for (auto pair : negResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::USES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testUSES = results.getIsTrue();
      Assert::IsFalse(testUSES);
    }
  }

  TEST_METHOD(TestExtractUsesWhileNestedAssignment) {
    /*
     *  procedure a {
     * 1. while (e >= 1) {
     * 2.   c = d;
     * 3.   c = b;
     *    }
     *  }
     */
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
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    auto parser = spa::SpParser(tokenStream);
    std::vector<spa::ProcedureStatement> procedureList = parser.parse();
    Assert::IsTrue(procedureList.size() == 1);

    spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
    designExtractor.extractRelationship();

    std::vector<std::pair<std::string, std::string>> positiveResTestCases = {
      {"1", varD}, {"1", varB}, {"1", varE}, {"2", varD}, {"3", varB}
    };

    for (auto pair : positiveResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::USES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testUSES = results.getIsTrue();
      Assert::IsTrue(testUSES);
    }

    std::vector<std::pair<std::string, std::string>> negResTestCases = {
      {"1", varC}, {"2", varC}, {"3", varC}
    };

    for (auto pair : negResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::USES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testUSES = results.getIsTrue();
      Assert::IsFalse(testUSES);
    }
  }

  TEST_METHOD(TestExtractUsesWhileNestedWhileAssignment) {
    /*
     *  procedure a {
     * 1. while (e >= 1) {
     * 2.   while (d >= 1) {
     * 3.     c = b;
     *      }
     *    }
     *  }
     */
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
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    auto parser = spa::SpParser(tokenStream);
    std::vector<spa::ProcedureStatement> procedureList = parser.parse();
    Assert::IsTrue(procedureList.size() == 1);

    spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
    designExtractor.extractRelationship();

    std::vector<std::pair<std::string, std::string>> positiveResTestCases = {
      {"1", varD}, {"1", varB}, {"1", varE}, {"2", varD}, {"2", varB},
      {"3", varB}
    };

    for (auto pair : positiveResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::USES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testUSES = results.getIsTrue();
      Assert::IsTrue(testUSES);
    }

    std::vector<std::pair<std::string, std::string>> negResTestCases = {
      {"1", varC}, {"2", varC}, {"3", varC}
    };

    for (auto pair : negResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::USES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testUSES = results.getIsTrue();
      Assert::IsFalse(testUSES);
    }
  }

  TEST_METHOD(TestExtractUsesIfNestedThenAssignment) {
    /*
     *  procedure a {
     * 1. if (d >= 1) then {
     * 2.   if (e >= 1) then {
     * 3.     c = b;
     *      } else {
     * 4.     c = f;
     *      }
     *    } else { }
     *  }
     */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenIf, tokenOpenBracket, tokenD, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenThen, tokenOpenBrace,
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
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    auto parser = spa::SpParser(tokenStream);
    std::vector<spa::ProcedureStatement> procedureList = parser.parse();
    Assert::IsTrue(procedureList.size() == 1);

    spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
    designExtractor.extractRelationship();

    std::vector<std::pair<std::string, std::string>> positiveResTestCases = {
      {"1", varD}, {"1", varB}, {"1", varE}, {"1", varF}, {"2", varB},
      {"2", varE}, {"2", varF},
      {"3", varB}, {"4", varF}
    };

    for (auto pair : positiveResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::USES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testUSES = results.getIsTrue();
      Assert::IsTrue(testUSES);
    }

    std::vector<std::pair<std::string, std::string>> negResTestCases = {
      {"1", varC}, {"2", varC}, {"3", varC}, {"4", varC}
    };

    for (auto pair : negResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::USES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testUSES = results.getIsTrue();
      Assert::IsFalse(testUSES);
    }
  }

  TEST_METHOD(TestExtractUsesIfNestedElseAssignment) {
    /*
     *  procedure a {
     * 1. if (d >= 1) then {
     *    } else {
     * 2.   if (e >= 1) then {
     * 3.     c = b;
     *      } else {
     * 4.     c = f;
     *      }
     *    }
     *  }
     */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenIf, tokenOpenBracket, tokenD, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenThen, tokenOpenBrace,
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
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    auto parser = spa::SpParser(tokenStream);
    std::vector<spa::ProcedureStatement> procedureList = parser.parse();
    Assert::IsTrue(procedureList.size() == 1);

    spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
    designExtractor.extractRelationship();

    std::vector<std::pair<std::string, std::string>> positiveResTestCases = {
      {"1", varD}, {"1", varB}, {"1", varE}, {"1", varF}, {"2", varB},
      {"2", varE}, {"2", varF},
      {"3", varB}, {"4", varF}
    };

    for (auto pair : positiveResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::USES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testUSES = results.getIsTrue();
      Assert::IsTrue(testUSES);
    }

    std::vector<std::pair<std::string, std::string>> negResTestCases = {
      {"1", varC}, {"2", varC}, {"3", varC}, {"4", varC}
    };

    for (auto pair : negResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::USES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testUSES = results.getIsTrue();
      Assert::IsFalse(testUSES);
    }
  }

  TEST_METHOD(TestExtractUsesIfNestedWhileAssignment) {
    /*
     *  procedure a {
     * 1. while (e >= 1) {
     * 2.   if (d >= 1) then {
     * 3.     c = b;
     *      } else {
     * 4.     c = f;
     *      }
     *    }
     *  }
     */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenWhile, tokenOpenBracket, tokenE, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenOpenBrace,
      tokenIf, tokenOpenBracket, tokenD, tokenGreaterEqual, tokenConstant, tokenCloseBracket,
      tokenThen, tokenOpenBrace,
      tokenC, tokenAssign, tokenB, tokenSemiColon,
      tokenCloseBrace, tokenElse, tokenOpenBrace,
      tokenC, tokenAssign, tokenF, tokenSemiColon,
      tokenCloseBrace, tokenCloseBrace, tokenCloseBrace
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

    std::vector<std::pair<std::string, std::string>> positiveResTestCases = {
      {"1", varD}, {"1", varB}, {"1", varE}, {"1", varF}, {"2", varB},
      {"2", varD}, {"2", varF}, {"3", varB}, {"4", varF}
    };

    for (auto pair : positiveResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::USES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testUSES = results.getIsTrue();
      Assert::IsTrue(testUSES);
    }

    std::vector<std::pair<std::string, std::string>> negResTestCases = {
      {"1", varC}, {"2", varC}, {"3", varC}, {"4", varC}
    };

    for (auto pair : negResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::USES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testUSES = results.getIsTrue();
      Assert::IsFalse(testUSES);
    }
  }

  TEST_METHOD(TestExtractUsesWhileNestedThenAssignment) {
    /*
     *  procedure a {
     * 1. if (d >= 1) then {
     * 2.   while (e >= 1) {
     * 3.     c = b;
     *      }
     *    } else { }
     *  }
     */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenIf, tokenOpenBracket, tokenD, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenThen, tokenOpenBrace,
      tokenWhile, tokenOpenBracket, tokenE, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenOpenBrace,
      tokenC, tokenAssign, tokenB, tokenSemiColon,
      tokenCloseBrace, tokenCloseBrace, tokenElse,
      tokenOpenBrace, tokenCloseBrace, tokenCloseBrace
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

    std::vector<std::pair<std::string, std::string>> positiveResTestCases = {
      {"1", varD}, {"1", varB}, {"1", varE}, {"2", varB}, {"2", varE},
      {"3", varB}
    };

    for (auto pair : positiveResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::USES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testUSES = results.getIsTrue();
      Assert::IsTrue(testUSES);
    }

    std::vector<std::pair<std::string, std::string>> negResTestCases = {
      {"1", varC}, {"2", varC}, {"3", varC}
    };

    for (auto pair : negResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::USES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testUSES = results.getIsTrue();
      Assert::IsFalse(testUSES);
    }
  }

  TEST_METHOD(TestExtractUsesWhileNestedElseAssignment) {
    /*
     *  procedure a {
     * 1. if (d >= 1) then {
     *    } else {
     * 2.   while (e >= 1) {
     * 3.     c = b;
     *      }
     *    }
     *  }
     */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenIf, tokenOpenBracket, tokenD, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenThen, tokenOpenBrace,
      tokenCloseBrace, tokenElse, tokenOpenBrace,
      tokenWhile, tokenOpenBracket, tokenE, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenOpenBrace,
      tokenC, tokenAssign, tokenB, tokenSemiColon,
      tokenCloseBrace, tokenCloseBrace, tokenCloseBrace
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

    std::vector<std::pair<std::string, std::string>> positiveResTestCases = {
      {"1", varD}, {"1", varB}, {"1", varE}, {"2", varB}, {"2", varE},
      {"3", varB}
    };

    for (auto pair : positiveResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::USES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testUSES = results.getIsTrue();
      Assert::IsTrue(testUSES);
    }

    std::vector<std::pair<std::string, std::string>> negResTestCases = {
      {"1", varC}, {"2", varC}, {"3", varC}
    };

    for (auto pair : negResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::USES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testUSES = results.getIsTrue();
      Assert::IsFalse(testUSES);
    }
  }

  TEST_METHOD(TestExtractUsesPrintStatement) {
    /*
     *  procedure a {
     * 1. print d;
     *  }
     */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenPrint, tokenD, tokenSemiColon,
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

    spa::PqlArgument lineNum(spa::ArgumentType::LINE_NO, "1", {});
    spa::PqlArgument variable(spa::ArgumentType::LITERAL_STRING, varD, {});
    spa::QueryResult usesRes = pkbManager->getRelationship(spa::USES, spa::PKBQueryArg(lineNum),
      spa::PKBQueryArg(variable));

    Assert::IsTrue(usesRes.getQueryResultType() == spa::BOOL);

    bool testUses = usesRes.getIsTrue();

    bool expectedUses = true;

    Assert::IsTrue(expectedUses == testUses);
  }

  TEST_METHOD(TestExtractUsesIfNestedPrintStatement) {
    /*
     *  procedure a {
     * 1. if (e >= 1) then {
     * 2.   print d;
     *    } else {
     * 3.   print b;
     *    }
     *  }
     */
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
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    auto parser = spa::SpParser(tokenStream);
    std::vector<spa::ProcedureStatement> procedureList = parser.parse();
    Assert::IsTrue(procedureList.size() == 1);

    spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
    designExtractor.extractRelationship();

    std::vector<std::pair<std::string, std::string>> positiveResTestCases = {
      {"1", varD}, {"1", varB}, {"1", varE}, {"2", varD},
      {"3", varB}
    };

    for (auto pair : positiveResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::USES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testUSES = results.getIsTrue();
      Assert::IsTrue(testUSES);
    }
  }

  TEST_METHOD(TestExtractUsesWhileNestedPrintStatement) {
    /*
     *  procedure a {
     * 1. while (e >= 1) {
     * 2.   print d;
     * 3.   print b;
     *    }
     *  }
     */
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
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    auto parser = spa::SpParser(tokenStream);
    std::vector<spa::ProcedureStatement> procedureList = parser.parse();
    Assert::IsTrue(procedureList.size() == 1);

    spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
    designExtractor.extractRelationship();

    std::vector<std::pair<std::string, std::string>> positiveResTestCases = {
      {"1", varD}, {"1", varB}, {"1", varE}, {"2", varD},
      {"3", varB}
    };

    for (auto pair : positiveResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::USES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testUSES = results.getIsTrue();
      Assert::IsTrue(testUSES);
    }
  }

  TEST_METHOD(TestExtractUsesWhileNestedWhilePrintStatement) {
    /*
     *  procedure a {
     * 1. while (e >= 1) {
     * 2.   while (c >= 1) {
     * 3.     print d;
     *      }
     *    }
     *  }
     */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenWhile, tokenOpenBracket, tokenE, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenOpenBrace,
      tokenWhile, tokenOpenBracket, tokenC, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenOpenBrace,
      tokenPrint, tokenD, tokenSemiColon,
      tokenCloseBrace, tokenCloseBrace, tokenCloseBrace
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

    std::vector<std::pair<std::string, std::string>> positiveResTestCases = {
      {"1", varC}, {"1", varD}, {"1", varE}, {"2", varD}, {"2", varC},
      {"3", varD}
    };

    for (auto pair : positiveResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::USES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testUSES = results.getIsTrue();
      Assert::IsTrue(testUSES);
    }
  }

  TEST_METHOD(TestExtractUsesIfNestedThenPrintStatement) {
    /*
     *  procedure a {
     * 1. if (e >= 1) then {
     * 2.   if (d >= 1) then {
     * 3.     print f;
     *        } else {
     * 4.     print b;
     *    } else { }
     *  }
     */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenIf, tokenOpenBracket, tokenE, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenThen, tokenOpenBrace,
      tokenIf, tokenOpenBracket, tokenD, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenThen, tokenOpenBrace,
      tokenPrint, tokenF, tokenSemiColon,
      tokenCloseBrace,
      tokenElse, tokenOpenBrace,
      tokenPrint, tokenB, tokenSemiColon, tokenCloseBrace,
      tokenCloseBrace, tokenElse, tokenOpenBrace, tokenCloseBrace, tokenCloseBrace
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

    std::vector<std::pair<std::string, std::string>> positiveResTestCases = {
      {"1", varE}, {"1", varD}, {"1", varB}, {"1", varF}, {"2", varD},
      {"2", varF},
      {"2", varB}, {"3", varF}, {"4", varB}
    };

    for (auto pair : positiveResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::USES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testUSES = results.getIsTrue();
      Assert::IsTrue(testUSES);
    }
  }

  TEST_METHOD(TestExtractUsesIfNestedElsePrintStatement) {
    /*
     *  procedure a {
     * 1. if (e >= 1) then {
     *    } else {
     * 2.   if (d >= 1) then {
     * 3.     print f;
     *      } else {
     * 4.     print b;
     *      }
     *    }
     *  }
     */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenIf, tokenOpenBracket, tokenE, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenThen, tokenOpenBrace,
      tokenCloseBrace, tokenElse, tokenOpenBrace,
      tokenIf, tokenOpenBracket, tokenD, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenThen, tokenOpenBrace,
      tokenPrint, tokenF, tokenSemiColon,
      tokenCloseBrace,
      tokenElse, tokenOpenBrace,
      tokenPrint, tokenB, tokenSemiColon, tokenCloseBrace,
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

    std::vector<std::pair<std::string, std::string>> positiveResTestCases = {
      {"1", varE}, {"1", varD}, {"1", varB}, {"1", varF}, {"2", varD},
      {"2", varF},
      {"2", varB}, {"3", varF}, {"4", varB}
    };

    for (auto pair : positiveResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::USES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testUSES = results.getIsTrue();
      Assert::IsTrue(testUSES);
    }
  }

  TEST_METHOD(TestExtractUsesWhileNestedThenPrintStatement) {
    /*
     *  procedure a {
     * 1. if (e >= 1) then {
     * 2.   while (d >= 1) {
     * 3.     print f;
     *      }
     *    } else { }
     *  }
     */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenIf, tokenOpenBracket, tokenE, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenThen, tokenOpenBrace,
      tokenWhile, tokenOpenBracket, tokenD, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenOpenBrace, tokenPrint, tokenF, tokenSemiColon,
      tokenCloseBrace, tokenCloseBrace, tokenElse, tokenOpenBrace,
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

    std::vector<std::pair<std::string, std::string>> positiveResTestCases = {
      {"1", varE}, {"1", varD}, {"1", varF}, {"2", varD}, {"2", varF},
      {"3", varF}
    };

    for (auto pair : positiveResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::USES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testUSES = results.getIsTrue();
      Assert::IsTrue(testUSES);
    }
  }

  TEST_METHOD(TestExtractUsesWhileNestedElsePrintStatement) {
    /*
     *  procedure a {
     * 1. if (e >= 1) then {
     *      }
     *    } else {
     * 2.   while (d >= 1) {
     * 3.     print f;
     *      }
     *    }
     *  }
     */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenIf, tokenOpenBracket, tokenE, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenThen, tokenOpenBrace, tokenCloseBrace,
      tokenElse, tokenOpenBrace,
      tokenWhile, tokenOpenBracket, tokenD, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenOpenBrace, tokenPrint, tokenF, tokenSemiColon,
      tokenCloseBrace,
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

    std::vector<std::pair<std::string, std::string>> positiveResTestCases = {
      {"1", varE}, {"1", varD}, {"1", varF}, {"2", varD}, {"2", varF},
      {"3", varF}
    };

    for (auto pair : positiveResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::USES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testUSES = results.getIsTrue();
      Assert::IsTrue(testUSES);
    }
  }

  TEST_METHOD(TestExtractUsesIfNestedWhilePrintStatement) {
    /*
     *  procedure a {
     * 1. while (e >= 1) {
     * 2.   if (c >= 1) {
     * 3.     print d;
     *      } else {
     * 4.        print f;
     *      }
     *    }
     *  }
     */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenWhile, tokenOpenBracket, tokenE, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenOpenBrace,
      tokenIf, tokenOpenBracket, tokenC, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenThen, tokenOpenBrace,
      tokenPrint, tokenD, tokenSemiColon,
      tokenCloseBrace, tokenElse, tokenOpenBrace,
      tokenPrint, tokenF, tokenSemiColon,
      tokenCloseBrace, tokenCloseBrace, tokenCloseBrace
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

    std::vector<std::pair<std::string, std::string>> positiveResTestCases = {
      {"1", varC}, {"1", varE}, {"1", varD}, {"1", varF}, {"2", varD}, {"2", varF}, {"2", varC},
      {"3", varD},
      {"4", varF}
    };

    for (auto pair : positiveResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::USES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testUSES = results.getIsTrue();
      Assert::IsTrue(testUSES);
    }
  }

  TEST_METHOD(TestExtractUsesVarPrintAssignment) {
    /*
     *  procedure a {
     * 1. c = d;
     * 2. print f;
     *  }
     */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenC, tokenAssign, tokenD, tokenSemiColon,
      tokenPrint, tokenF, tokenSemiColon, tokenCloseBrace
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

    std::vector<std::pair<std::string, std::string>> positiveResTestCases = {
      {"1", varD}, {"2", varF}
    };

    for (auto pair : positiveResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::USES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testUSES = results.getIsTrue();
      Assert::IsTrue(testUSES);
    }

    std::vector<std::pair<std::string, std::string>> negResTestCases = {
      {"1", varC}
    };

    for (auto pair : negResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::USES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testUSES = results.getIsTrue();
      Assert::IsFalse(testUSES);
    }
  }

  TEST_METHOD(TestExtractUsesIfNestedPrintAssignment) {
    /*
     *  procedure a {
     * 1. if (e >= 1) then {
     * 2.   c = d;
     * 3.   print f;
     *    } else {
     * 3.   c = b;
     * 5.   print g;
     *    }
     *  }
     */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenIf, tokenOpenBracket, tokenE, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenThen, tokenOpenBrace,
      tokenC, tokenAssign, tokenD, tokenSemiColon,
      tokenPrint, tokenF, tokenSemiColon,
      tokenCloseBrace,
      tokenElse, tokenOpenBrace,
      tokenC, tokenAssign, tokenB, tokenSemiColon,
      tokenPrint, tokenG, tokenSemiColon,
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

    std::vector<std::pair<std::string, std::string>> positiveResTestCases = {
      {"1", varD}, {"1", varB}, {"1", varF}, {"1", varG}, {"1", varE},
      {"2", varD}, {"3", varF}, {"4", varB}, {"5", varG}
    };

    for (auto pair : positiveResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::USES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testUSES = results.getIsTrue();
      Assert::IsTrue(testUSES);
    }

    std::vector<std::pair<std::string, std::string>> negResTestCases = {
      {"1", varC}, {"2", varC}, {"3", varC}
    };

    for (auto pair : negResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::USES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testUSES = results.getIsTrue();
      Assert::IsFalse(testUSES);
    }
  }

  TEST_METHOD(TestExtractUsesWhileNestedPrintAssignment) {
    /*
     *  procedure a {
     * 1. while (e >= 1) {
     * 2.   c = d;
     * 3.   c = b;
     * 4.   print f;
     *    }
     *  }
     */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenWhile, tokenOpenBracket, tokenE, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenOpenBrace,
      tokenC, tokenAssign, tokenD, tokenSemiColon,
      tokenC, tokenAssign, tokenB, tokenSemiColon,
      tokenPrint, tokenF, tokenSemiColon,
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

    std::vector<std::pair<std::string, std::string>> positiveResTestCases = {
      {"1", varD}, {"1", varB}, {"1", varE}, {"1", varF}, {"2", varD},
      {"3", varB}, {"4", varF},
    };

    for (auto pair : positiveResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::USES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testUSES = results.getIsTrue();
      Assert::IsTrue(testUSES);
    }

    std::vector<std::pair<std::string, std::string>> negResTestCases = {
      {"1", varC}, {"2", varC}, {"3", varC}
    };

    for (auto pair : negResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(spa::USES,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testUSES = results.getIsTrue();
      Assert::IsFalse(testUSES);
    }
  }
};
}  // namespace UnitTesting
