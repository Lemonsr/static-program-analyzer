#include <stdafx.h>
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

#include "DesignExtractor.h"
#include "PKB.h"
#include "SpParser.h"
#include "Token.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace UnitTesting {
TEST_CLASS(TestDesignExtractorNestedUsesModifiesProc) {
  std::string varA = "a";
  std::string varB = "b";
  std::string varC = "c";
  std::string varD = "d";
  std::string varE = "e";
  std::string varF = "f";
  std::string varG = "g";
  std::string varH = "h";
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
  spa::Token tokenH = spa::Token(spa::TOKEN_NAME, varH);
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
  std::unique_ptr<spa::PKBManager> pkbManager;

  std::vector<std::string> retrieveResFromPkb(
    std::string procName, std::string synonym, spa::DesignEntityType type,
    spa::RelationshipType relType) {
    spa::PqlArgument firstArg = spa::PqlArgument(spa::LITERAL_STRING, procName, {});
    spa::PqlArgument secondArg = spa::PqlArgument(spa::SYNONYM, synonym,
      {type});
    spa::QueryResult queryResult = pkbManager->getRelationship(relType, spa::PKBQueryArg(firstArg),
      spa::PKBQueryArg(secondArg));
    std::vector<std::pair<std::string, std::string>> result = queryResult.getNameNamePairs();
    std::vector<std::string> trxResult(result.size());

    std::transform(result.begin(), result.end(), trxResult.begin(),
      [](std::pair<std::string, std::string>& x) { return x.second; });
    return trxResult;
  }

  bool areStringVectorsEqual(const std::vector<std::string>& expected,
                             const std::vector<std::string>& test) {
    // Check the size of the vectors
    if (expected.size() != test.size()) {
      return false;
    }

    // Sort the vectors
    std::vector<std::string> sorted_v1 = expected;
    std::vector<std::string> sorted_v2 = test;
    std::sort(sorted_v1.begin(), sorted_v1.end());
    std::sort(sorted_v2.begin(), sorted_v2.end());

    // Compare the sorted vectors
    return std::equal(sorted_v1.begin(), sorted_v1.end(), sorted_v2.begin());
  }

  void assertTrueTestCases(std::vector<std::pair<std::string, std::string>> testCases,
                           spa::RelationshipType relType) {
    for (auto pair : testCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first,
        {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, pair.second,
        {});
      spa::QueryResult results = pkbManager->getRelationship(relType,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool test = results.getIsTrue();
      Assert::IsTrue(test);
    }
  }

public:
  TEST_METHOD_INITIALIZE(BeforeEach) {
    pkbManager = std::make_unique<spa::PKB>();
  }

  TEST_METHOD(TestExtractNestedWhileUsesAndModifiesProc) {
    /*
     *  procedure a {
     * 1. while (1 >= 1) {
     * 2.   call b;
     *    }
     *  }
     *
     *  procedure b {
     * 3. c = d - e;
     *  }
     */

    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenWhile, tokenOpenBracket, tokenConstant, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenOpenBrace, tokenCall,
      tokenB, tokenSemiColon, tokenCloseBrace, tokenCloseBrace,
      tokenProcedure, tokenB, tokenOpenBrace,
      tokenC, tokenAssign, tokenD, tokenMinusOp, tokenE, tokenSemiColon,
      tokenCloseBrace
    };
    spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
    for (auto token : tokenList) {
      tokenStream.pushBack(token);
    }
    auto parser = spa::SpParser(tokenStream);
    std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();
    Assert::IsTrue(procedureList.size() == 2);

    spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
    designExtractor.extractRelationship();

    std::vector<std::pair<std::string, std::string>> positiveUsesTestCases = {
      {"1", varD}, {"1", varE}
    };

    assertTrueTestCases(positiveUsesTestCases, spa::USES);

    std::vector<std::pair<std::string, std::string>> positiveModifiesTestCases = {
      {"1", varC}
    };

    assertTrueTestCases(positiveModifiesTestCases, spa::MODIFIES);
  }

  TEST_METHOD(TestExtractNestedThenUsesAndModifiesProc) {
    /*
     *  procedure a {
     * 1. if (1 >= 1) then {
     * 2.   call b;
     *    } else {}
     *  }
     *
     *  procedure b {
     * 3. c = d - e;
     *  }
     */

    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenIf, tokenOpenBracket, tokenConstant, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenThen, tokenOpenBrace,
      tokenCall, tokenB, tokenSemiColon, tokenCloseBrace,
      tokenElse, tokenOpenBrace, tokenCloseBrace, tokenCloseBrace,
      tokenProcedure, tokenB, tokenOpenBrace,
      tokenC, tokenAssign, tokenD, tokenMinusOp, tokenE, tokenSemiColon,
      tokenCloseBrace
    };
    spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
    for (auto token : tokenList) {
      tokenStream.pushBack(token);
    }
    auto parser = spa::SpParser(tokenStream);
    std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();
    Assert::IsTrue(procedureList.size() == 2);

    spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
    designExtractor.extractRelationship();

    std::vector<std::pair<std::string, std::string>> positiveUsesTestCases = {
      {"1", varD}, {"1", varE}
    };

    assertTrueTestCases(positiveUsesTestCases, spa::USES);

    std::vector<std::pair<std::string, std::string>> positiveModifiesTestCases = {
      {"1", varC}
    };

    assertTrueTestCases(positiveModifiesTestCases, spa::MODIFIES);
  }

  TEST_METHOD(TestExtractNestedElseUsesAndModifiesProc) {
    /*
     *  procedure a {
     * 1. if (1 >= 1) then {
     *    } else {
     * 2.   call b;
     *    }
     *  }
     *
     *  procedure b {
     * 3. c = d - e;
     *  }
     */

    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenIf, tokenOpenBracket, tokenConstant, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenThen, tokenOpenBrace, tokenCloseBrace,
      tokenElse, tokenOpenBrace,
      tokenCall, tokenB, tokenSemiColon,
      tokenCloseBrace, tokenCloseBrace,
      tokenProcedure, tokenB, tokenOpenBrace,
      tokenC, tokenAssign, tokenD, tokenMinusOp, tokenE, tokenSemiColon,
      tokenCloseBrace
    };
    spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
    for (auto token : tokenList) {
      tokenStream.pushBack(token);
    }
    auto parser = spa::SpParser(tokenStream);
    std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();
    Assert::IsTrue(procedureList.size() == 2);

    spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
    designExtractor.extractRelationship();

    std::vector<std::pair<std::string, std::string>> positiveUsesTestCases = {
      {"1", varD}, {"1", varE}
    };

    assertTrueTestCases(positiveUsesTestCases, spa::USES);

    std::vector<std::pair<std::string, std::string>> positiveModifiesTestCases = {
      {"1", varC}
    };

    assertTrueTestCases(positiveModifiesTestCases, spa::MODIFIES);
  }

  TEST_METHOD(TestExtractNestedWhileUsesAndModifiesMultiProc) {
    /*
     *  procedure a {
     * 1. while (1 >= 1) {
     * 2.   call b;
     *    }
     *  }
     *
     *  procedure b {
     * 3. call g;
     *  }
     *
     *  procedure g {
     * 4. c = d - e;
     *  }
     */

    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenWhile, tokenOpenBracket, tokenConstant, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenOpenBrace,
      tokenCall, tokenB, tokenSemiColon,
      tokenCloseBrace, tokenCloseBrace,
      tokenProcedure, tokenB, tokenOpenBrace,
      tokenCall, tokenG, tokenSemiColon,
      tokenCloseBrace,
      tokenProcedure, tokenG, tokenOpenBrace,
      tokenC, tokenAssign, tokenD, tokenMinusOp, tokenE, tokenSemiColon,
      tokenCloseBrace
    };
    spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
    for (auto token : tokenList) {
      tokenStream.pushBack(token);
    }
    auto parser = spa::SpParser(tokenStream);
    std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();
    Assert::IsTrue(procedureList.size() == 3);

    spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
    designExtractor.extractRelationship();

    std::vector<std::pair<std::string, std::string>> positiveUsesTestCases = {
      {"1", varD}, {"1", varE}
    };

    assertTrueTestCases(positiveUsesTestCases, spa::USES);

    std::vector<std::pair<std::string, std::string>> positiveModifiesTestCases = {
      {"1", varC}
    };

    assertTrueTestCases(positiveModifiesTestCases, spa::MODIFIES);
  }

  TEST_METHOD(TestExtractNestedThenUsesAndModifiesMultiProc) {
    /*
     *  procedure a {
     * 1. if (1 >= 1) then {
     * 2.   call b;
     *    } else {}
     *  }
     *
     *  procedure b {
     * 3. call g;
     *  }
     *
     *  procedure g {
     * 4. c = d - e;
     *  }
     */

    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenIf, tokenOpenBracket, tokenConstant, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenThen,
      tokenOpenBrace,
      tokenCall, tokenB, tokenSemiColon,
      tokenCloseBrace,
      tokenElse, tokenOpenBrace,
      tokenCloseBrace, tokenCloseBrace,
      tokenProcedure, tokenB, tokenOpenBrace,
      tokenCall, tokenG, tokenSemiColon,
      tokenCloseBrace,
      tokenProcedure, tokenG, tokenOpenBrace,
      tokenC, tokenAssign, tokenD, tokenMinusOp, tokenE, tokenSemiColon,
      tokenCloseBrace
    };
    spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
    for (auto token : tokenList) {
      tokenStream.pushBack(token);
    }
    auto parser = spa::SpParser(tokenStream);
    std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();
    Assert::IsTrue(procedureList.size() == 3);

    spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
    designExtractor.extractRelationship();

    std::vector<std::pair<std::string, std::string>> positiveUsesTestCases = {
      {"1", varD}, {"1", varE}
    };

    assertTrueTestCases(positiveUsesTestCases, spa::USES);

    std::vector<std::pair<std::string, std::string>> positiveModifiesTestCases = {
      {"1", varC}
    };

    assertTrueTestCases(positiveModifiesTestCases, spa::MODIFIES);
  }

  TEST_METHOD(TestExtractNestedElseUsesAndModifiesMultiProc) {
    /*
     *  procedure a {
     * 1. if (1 >= 1) then {
     *    } else {
     * 2.   call b;
     *    }
     *  }
     *
     *  procedure b {
     * 3. call g;
     *  }
     *
     *  procedure g {
     * 4. c = d - e;
     *  }
     */

    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenIf, tokenOpenBracket, tokenConstant, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenThen,
      tokenOpenBrace, tokenCloseBrace,
      tokenElse, tokenOpenBrace,
      tokenCall, tokenB, tokenSemiColon,
      tokenCloseBrace, tokenCloseBrace,
      tokenProcedure, tokenB, tokenOpenBrace,
      tokenCall, tokenG, tokenSemiColon,
      tokenCloseBrace,
      tokenProcedure, tokenG, tokenOpenBrace,
      tokenC, tokenAssign, tokenD, tokenMinusOp, tokenE, tokenSemiColon,
      tokenCloseBrace
    };
    spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
    for (auto token : tokenList) {
      tokenStream.pushBack(token);
    }
    auto parser = spa::SpParser(tokenStream);
    std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();
    Assert::IsTrue(procedureList.size() == 3);

    spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
    designExtractor.extractRelationship();

    std::vector<std::pair<std::string, std::string>> positiveUsesTestCases = {
      {"1", varD}, {"1", varE}
    };

    assertTrueTestCases(positiveUsesTestCases, spa::USES);

    std::vector<std::pair<std::string, std::string>> positiveModifiesTestCases = {
      {"1", varC}
    };

    assertTrueTestCases(positiveModifiesTestCases, spa::MODIFIES);
  }

  TEST_METHOD(TestExtractWhileNestedWhileUsesAndModifiesProc) {
    /*
     *  procedure a {
     * 1. while (1 >= 1) {
     * 2.   while (1 >= 1) {
     * 3.     call b;
     *      }
     *    }
     *  }
     *
     *  procedure b {
     * 4. c = d - e;
     *  }
     */

    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenWhile, tokenOpenBracket, tokenConstant, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenOpenBrace,
      tokenWhile, tokenOpenBracket, tokenConstant, tokenGreaterEqual,
      tokenConstant, tokenCloseBracket, tokenOpenBrace,
      tokenCall, tokenB, tokenSemiColon,
      tokenCloseBrace, tokenCloseBrace, tokenCloseBrace,
      tokenProcedure, tokenB, tokenOpenBrace,
      tokenC, tokenAssign, tokenD, tokenMinusOp, tokenE, tokenSemiColon,
      tokenCloseBrace,
    };
    spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
    for (auto token : tokenList) {
      tokenStream.pushBack(token);
    }
    auto parser = spa::SpParser(tokenStream);
    std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();
    Assert::IsTrue(procedureList.size() == 2);

    spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
    designExtractor.extractRelationship();

    std::vector<std::pair<std::string, std::string>> positiveUsesTestCases = {
      {"1", varD}, {"1", varE}, {"2", varD}, {"2", varE}
    };

    assertTrueTestCases(positiveUsesTestCases, spa::USES);

    std::vector<std::pair<std::string, std::string>> positiveModifiesTestCases = {
      {"1", varC}, {"2", varC}
    };

    assertTrueTestCases(positiveModifiesTestCases, spa::MODIFIES);
  }

  TEST_METHOD(TestExtractIfNestedThenUsesAndModifiesProc) {
    /*
     *  procedure a {
     * 1. if (1 >= 1) then {
     * 2.   if (1 >= 1) then {
     * 3.     call b;
     *      } else {}
     *    } else {}
     *  }
     *
     *  procedure b {
     * 4. c = d - e;
     *  }
     */

    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenIf, tokenOpenBracket, tokenConstant, tokenGreaterEqual, tokenConstant, tokenCloseBracket,
      tokenThen, tokenOpenBrace, tokenIf, tokenOpenBracket, tokenConstant, tokenGreaterEqual,
      tokenConstant, tokenCloseBracket, tokenThen, tokenOpenBrace,
      tokenCall, tokenB, tokenSemiColon,
      tokenCloseBrace, tokenElse, tokenOpenBrace, tokenCloseBrace, tokenCloseBrace,
      tokenElse, tokenOpenBrace, tokenCloseBrace, tokenCloseBrace,
      tokenProcedure, tokenB, tokenOpenBrace,
      tokenC, tokenAssign, tokenD, tokenMinusOp, tokenE, tokenSemiColon,
      tokenCloseBrace,
    };
    spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
    for (auto token : tokenList) {
      tokenStream.pushBack(token);
    }
    auto parser = spa::SpParser(tokenStream);
    std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();
    Assert::IsTrue(procedureList.size() == 2);

    spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
    designExtractor.extractRelationship();

    std::vector<std::pair<std::string, std::string>> positiveUsesTestCases = {
      {"1", varD}, {"1", varE}, {"2", varD}, {"2", varE}
    };

    assertTrueTestCases(positiveUsesTestCases, spa::USES);

    std::vector<std::pair<std::string, std::string>> positiveModifiesTestCases = {
      {"1", varC}, {"2", varC}
    };

    assertTrueTestCases(positiveModifiesTestCases, spa::MODIFIES);
  }

  TEST_METHOD(TestExtractIfNestedElseUsesAndModifiesProc) {
    /*
     *  procedure a {
     * 1. if (1 >= 1) then {
     *    } else {
     * 2.   if (1 >= 1) then {
     * 3.     call b;
     *      } else {}
     *    }
     *  }
     *
     *  procedure b {
     * 4. c = d - e;
     *  }
     */

    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenIf, tokenOpenBracket, tokenConstant, tokenGreaterEqual, tokenConstant,
      tokenThen, tokenOpenBrace, tokenCloseBrace,
      tokenElse, tokenOpenBrace,
      tokenIf, tokenOpenBracket, tokenConstant, tokenGreaterEqual,
      tokenConstant, tokenCloseBracket, tokenThen, tokenOpenBrace,
      tokenCall, tokenB, tokenSemiColon,
      tokenCloseBrace, tokenElse, tokenOpenBrace, tokenCloseBrace,
      tokenCloseBrace, tokenCloseBrace,
      tokenProcedure, tokenB, tokenOpenBrace,
      tokenC, tokenAssign, tokenD, tokenMinusOp, tokenE, tokenSemiColon,
      tokenCloseBrace,
    };
    spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
    for (auto token : tokenList) {
      tokenStream.pushBack(token);
    }
    auto parser = spa::SpParser(tokenStream);
    std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();
    Assert::IsTrue(procedureList.size() == 2);

    spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
    designExtractor.extractRelationship();

    std::vector<std::pair<std::string, std::string>> positiveUsesTestCases = {
      {"1", varD}, {"1", varE}, {"2", varD}, {"2", varE}
    };

    assertTrueTestCases(positiveUsesTestCases, spa::USES);

    std::vector<std::pair<std::string, std::string>> positiveModifiesTestCases = {
      {"1", varC}, {"2", varC}
    };

    assertTrueTestCases(positiveModifiesTestCases, spa::MODIFIES);
  }
};
}  // namespace UnitTesting
