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
TEST_CLASS(TestDesignExtractorExtractNext) {
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

  bool retrieveResFromPkb(
    std::string lineNumOne, std::string lineNumTwo) {
    spa::PqlArgument firstArg = spa::PqlArgument(spa::LINE_NO, lineNumOne, {});
    spa::PqlArgument secondArg = spa::PqlArgument(spa::LINE_NO, lineNumTwo,
      {});
    spa::QueryResult queryResult = pkbManager->getRelationship(spa::NEXT,
      spa::PKBQueryArg(firstArg),
      spa::PKBQueryArg(secondArg));
    return queryResult.getIsTrue();
  }

  std::vector<std::pair<int, int>> generateNegTestCases(int totalLines,
                                                        std::vector<std::pair<int, int>> excluded) {
    std::vector<std::pair<int, int>> result;
    for (int i = 1; i <= totalLines; i++) {
      for (int j = 1; j <= totalLines; j++) {
        // Skip any pairs that are excluded.
        if (find(excluded.begin(), excluded.end(), std::make_pair(i, j))
          != excluded.end()) {
          continue;
        }

        // Add the pair to the result.
        result.push_back(std::make_pair(i, j));
      }
    }

    for (int i = 1; i <= totalLines; i++) {
      for (int j = i + 1; j <= totalLines; j++) {
        // Skip any pairs that are excluded.
        if (find(excluded.begin(), excluded.end(), std::make_pair(i, j))
          != excluded.end() ||
          find(excluded.begin(), excluded.end(), std::make_pair(j, i))
          != excluded.end()) {
          continue;
        }

        // Add the pair to the result.
        result.push_back(std::make_pair(i, j));
      }
    }
    return result;
  }

public:
  TEST_METHOD_INITIALIZE(BeforeEach) {
    pkbManager = std::make_unique<spa::PKB>();
  }

  TEST_METHOD(TestExtractNextSingleProc) {
    /*
     *  procedure a {
     * 1. c = d - e;
     * 2. print c;
     * 3. read g;
     * 4. g = h;
     *  }
     */

    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenC, tokenAssign, tokenD, tokenMinusOp, tokenE, tokenSemiColon,
      tokenPrint, tokenC, tokenSemiColon,
      tokenRead, tokenG, tokenSemiColon,
      tokenG, tokenAssign, tokenH, tokenSemiColon,
      tokenCloseBrace
    };
    spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
    for (auto token : tokenList) {
      tokenStream.pushBack(token);
    }
    auto parser = spa::SpParser(tokenStream);
    std::vector<spa::ProcedureStatement> procedureList = parser.parse();
    Assert::IsTrue(procedureList.size() == 1);

    spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
    designExtractor.extractRelationship();

    std::vector<std::pair<int, int>> positiveTestCases = {
      {1, 2}, {2, 3}, {3, 4}
    };

    for (auto& pair : positiveTestCases) {
      std::string lineNumOne = std::to_string(pair.first);
      std::string lineNumTwo = std::to_string(pair.second);
      bool testNext = retrieveResFromPkb(lineNumOne, lineNumTwo);
      Assert::IsTrue(testNext);
    }

    std::vector<std::pair<int, int>> negativeTestCases = generateNegTestCases(4, positiveTestCases);

    for (auto& pair : negativeTestCases) {
      std::string lineNumOne = std::to_string(pair.first);
      std::string lineNumTwo = std::to_string(pair.second);
      bool testNext = retrieveResFromPkb(lineNumOne, lineNumTwo);
      Assert::IsFalse(testNext);
    }
  }

  TEST_METHOD(TestExtractSingleProcNestedWhile) {
    /*
     *  procedure a {
     * 1. while (1 >= 1) {
     * 2.   c = d - e;
     * 3.   print c;
     * 4.   read g;
     *    }
     * 5.   g = h;
     *  }
     *
     */

    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenWhile, tokenOpenBracket, tokenConstant, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenOpenBrace,
      tokenC, tokenAssign, tokenD, tokenMinusOp, tokenE, tokenSemiColon,
      tokenPrint, tokenC, tokenSemiColon,
      tokenRead, tokenG, tokenSemiColon,
      tokenCloseBrace,
      tokenG, tokenAssign, tokenH, tokenSemiColon,
      tokenCloseBrace
    };
    spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
    for (auto token : tokenList) {
      tokenStream.pushBack(token);
    }
    auto parser = spa::SpParser(tokenStream);
    std::vector<spa::ProcedureStatement> procedureList = parser.parse();
    Assert::IsTrue(procedureList.size() == 1);

    spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
    designExtractor.extractRelationship();

    std::vector<std::pair<int, int>> positiveTestCases = {
      {1, 2}, {2, 3}, {3, 4}, {4, 1}, {1, 5}
    };

    for (auto& pair : positiveTestCases) {
      std::string lineNumOne = std::to_string(pair.first);
      std::string lineNumTwo = std::to_string(pair.second);
      bool testNext = retrieveResFromPkb(lineNumOne, lineNumTwo);
      Assert::IsTrue(testNext);
    }

    std::vector<std::pair<int, int>> negativeTestCases = generateNegTestCases(5, positiveTestCases);

    for (auto& pair : negativeTestCases) {
      std::string lineNumOne = std::to_string(pair.first);
      std::string lineNumTwo = std::to_string(pair.second);
      bool testNext = retrieveResFromPkb(lineNumOne, lineNumTwo);
      Assert::IsFalse(testNext);
    }
  }

  TEST_METHOD(TestExtractSingleProcNestedIf) {
    /*
     *  procedure a {
     * 1. if (1 >= 1) then {
     * 2.   c = d - e;
     * 3.   print c;
     *    } else {
     * 4.   read g;
     * 5.   print c;
     *    }
     * 6.   g = h;
     *  }
     *
     */

    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenIf, tokenOpenBracket, tokenConstant, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenThen, tokenOpenBrace,
      tokenC, tokenAssign, tokenD, tokenMinusOp, tokenE, tokenSemiColon,
      tokenPrint, tokenC, tokenSemiColon,
      tokenCloseBrace, tokenElse, tokenOpenBrace,
      tokenRead, tokenG, tokenSemiColon,
      tokenPrint, tokenC, tokenSemiColon,
      tokenCloseBrace,
      tokenG, tokenAssign, tokenH, tokenSemiColon,
      tokenCloseBrace
    };
    spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
    for (auto token : tokenList) {
      tokenStream.pushBack(token);
    }
    auto parser = spa::SpParser(tokenStream);
    std::vector<spa::ProcedureStatement> procedureList = parser.parse();
    Assert::IsTrue(procedureList.size() == 1);

    spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
    designExtractor.extractRelationship();

    std::vector<std::pair<int, int>> positiveTestCases = {
      {1, 2}, {2, 3}, {1, 4}, {4, 5}, {3, 6}, {5, 6}
    };

    for (auto& pair : positiveTestCases) {
      std::string lineNumOne = std::to_string(pair.first);
      std::string lineNumTwo = std::to_string(pair.second);
      bool testNext = retrieveResFromPkb(lineNumOne, lineNumTwo);
      Assert::IsTrue(testNext);
    }

    std::vector<std::pair<int, int>> negativeTestCases = generateNegTestCases(6, positiveTestCases);

    for (auto& pair : negativeTestCases) {
      std::string lineNumOne = std::to_string(pair.first);
      std::string lineNumTwo = std::to_string(pair.second);
      bool testNext = retrieveResFromPkb(lineNumOne, lineNumTwo);
      Assert::IsFalse(testNext);
    }
  }

  TEST_METHOD(TestExtractSingleProcWhileNestedWhile) {
    /*
     *  procedure a {
     * 1. while (1 >= 1) {
     * 2.   c = d - e;
     * 3.   print c;
     * 4.   while (1 >= 1) {
     * 5.     print c;
     * 6.     read g;
     *      }
     * 7.   print c;
     *    }
     * 8.   g = h;
     *  }
     *
     */

    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenWhile, tokenOpenBracket, tokenConstant, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenOpenBrace,
      tokenC, tokenAssign, tokenD, tokenMinusOp, tokenE, tokenSemiColon,
      tokenPrint, tokenC, tokenSemiColon,
      tokenWhile, tokenOpenBracket, tokenConstant, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenOpenBrace,
      tokenPrint, tokenC, tokenSemiColon,
      tokenRead, tokenG, tokenSemiColon,
      tokenCloseBrace,
      tokenPrint, tokenC, tokenSemiColon,
      tokenCloseBrace,
      tokenG, tokenAssign, tokenH, tokenSemiColon,
      tokenCloseBrace
    };
    spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
    for (auto token : tokenList) {
      tokenStream.pushBack(token);
    }
    auto parser = spa::SpParser(tokenStream);
    std::vector<spa::ProcedureStatement> procedureList = parser.parse();
    Assert::IsTrue(procedureList.size() == 1);

    spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
    designExtractor.extractRelationship();

    std::vector<std::pair<int, int>> positiveTestCases = {
      {1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 6}, {6, 4}, {4, 7}, {7, 1}, {1, 8}
    };

    for (auto& pair : positiveTestCases) {
      std::string lineNumOne = std::to_string(pair.first);
      std::string lineNumTwo = std::to_string(pair.second);
      bool testNext = retrieveResFromPkb(lineNumOne, lineNumTwo);
      Assert::IsTrue(testNext);
    }

    std::vector<std::pair<int, int>> negativeTestCases = generateNegTestCases(8, positiveTestCases);

    for (auto& pair : negativeTestCases) {
      std::string lineNumOne = std::to_string(pair.first);
      std::string lineNumTwo = std::to_string(pair.second);
      bool testNext = retrieveResFromPkb(lineNumOne, lineNumTwo);
      Assert::IsFalse(testNext);
    }
  }

  TEST_METHOD(TestExtractSingleProcIfNestedIf) {
    /*
     *  procedure a {
     * 1. if (1 >= 1) then {
     * 2.   c = d - e;
     * 3.   if (1 >= 1) then {
     * 4.     c = d - e;
     * 5.     print c;
     *      } else {
     * 6.     read g;
     * 7.     print c;
     *      }
     * 8.   print c;
     *    } else {
     * 9.   read g;
     * 10.  if (1 >= 1) then {
     * 11.     c = d - e;
     * 12.     print c;
     *      } else {
     * 13.     read g;
     * 14.     print c;
     *      }
     * 15.   print c;
     *    }
     * 16.   g = h;
     *  }
     *
     */

    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenIf, tokenOpenBracket, tokenConstant, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenThen, tokenOpenBrace,
      tokenC, tokenAssign, tokenD, tokenMinusOp, tokenE, tokenSemiColon,
      tokenIf, tokenOpenBracket, tokenConstant, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenThen, tokenOpenBrace,
      tokenC, tokenAssign, tokenD, tokenMinusOp, tokenE, tokenSemiColon,
      tokenPrint, tokenC, tokenSemiColon,
      tokenCloseBrace, tokenElse, tokenOpenBrace,
      tokenRead, tokenG, tokenSemiColon,
      tokenPrint, tokenC, tokenSemiColon,
      tokenCloseBrace,
      tokenPrint, tokenC, tokenSemiColon,
      tokenCloseBrace, tokenElse, tokenOpenBrace,
      tokenRead, tokenG, tokenSemiColon,
      tokenIf, tokenOpenBracket, tokenConstant, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenThen, tokenOpenBrace,
      tokenC, tokenAssign, tokenD, tokenMinusOp, tokenE, tokenSemiColon,
      tokenPrint, tokenC, tokenSemiColon,
      tokenCloseBrace, tokenElse, tokenOpenBrace,
      tokenRead, tokenG, tokenSemiColon,
      tokenPrint, tokenC, tokenSemiColon,
      tokenCloseBrace,
      tokenPrint, tokenC, tokenSemiColon,
      tokenCloseBrace,
      tokenG, tokenAssign, tokenH, tokenSemiColon,
      tokenCloseBrace
    };
    spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
    for (auto token : tokenList) {
      tokenStream.pushBack(token);
    }
    auto parser = spa::SpParser(tokenStream);
    std::vector<spa::ProcedureStatement> procedureList = parser.parse();
    Assert::IsTrue(procedureList.size() == 1);

    spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
    designExtractor.extractRelationship();

    std::vector<std::pair<int, int>> positiveTestCases = {
      {1, 2}, {1, 9}, {2, 3}, {3, 4}, {3, 6}, {4, 5}, {6, 7}, {5, 8}, {7, 8}, {9, 10}, {10, 11},
      {10, 13}, {11, 12}, {12, 15}, {13, 14}, {14, 15}, {15, 16}, {8, 16}
    };

    for (auto& pair : positiveTestCases) {
      std::string lineNumOne = std::to_string(pair.first);
      std::string lineNumTwo = std::to_string(pair.second);
      bool testNext = retrieveResFromPkb(lineNumOne, lineNumTwo);
      Assert::IsTrue(testNext);
    }

    std::vector<std::pair<int, int>> negativeTestCases =
      generateNegTestCases(16, positiveTestCases);

    for (auto& pair : negativeTestCases) {
      std::string lineNumOne = std::to_string(pair.first);
      std::string lineNumTwo = std::to_string(pair.second);
      bool testNext = retrieveResFromPkb(lineNumOne, lineNumTwo);
      Assert::IsFalse(testNext);
    }
  }

  TEST_METHOD(TestExtractSingleProcIfNestedWhile) {
    /*
     *  procedure a {
     * 1. if (1 >= 1) then {
     * 2.   c = d - e;
     * 3.   while (1 >= 1) {
     * 4.     c = d - e;
     * 5.     print c;
     *    }
     * 6.   print c;
     *    } else {
     * 7.   read g;
     * 8.     while (1 >= 1) {
     * 9.       c = d - e;
     * 10.       print c;
     *      }
     * 11.   print c;
     *    }
     * 12.   g = h;
     *  }
     *
     */

    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenIf, tokenOpenBracket, tokenConstant, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenThen, tokenOpenBrace,
      tokenC, tokenAssign, tokenD, tokenMinusOp, tokenE, tokenSemiColon,
      tokenWhile, tokenOpenBracket, tokenConstant, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenOpenBrace,
      tokenC, tokenAssign, tokenD, tokenMinusOp, tokenE, tokenSemiColon,
      tokenPrint, tokenC, tokenSemiColon,
      tokenCloseBrace,
      tokenPrint, tokenC, tokenSemiColon,
      tokenCloseBrace, tokenElse, tokenOpenBrace,
      tokenRead, tokenG, tokenSemiColon,
      tokenWhile, tokenOpenBracket, tokenConstant, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenOpenBrace,
      tokenC, tokenAssign, tokenD, tokenMinusOp, tokenE, tokenSemiColon,
      tokenPrint, tokenC, tokenSemiColon,
      tokenCloseBrace,
      tokenPrint, tokenC, tokenSemiColon,
      tokenCloseBrace,
      tokenG, tokenAssign, tokenH, tokenSemiColon,
      tokenCloseBrace
    };
    spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
    for (auto token : tokenList) {
      tokenStream.pushBack(token);
    }
    auto parser = spa::SpParser(tokenStream);
    std::vector<spa::ProcedureStatement> procedureList = parser.parse();
    Assert::IsTrue(procedureList.size() == 1);

    spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
    designExtractor.extractRelationship();

    std::vector<std::pair<int, int>> positiveTestCases = {
      {1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 3}, {3, 6}, {1, 7}, {7, 8}, {8, 9}, {9, 10}, {10, 8},
      {8, 11}, {6, 12}, {11, 12}
    };

    for (auto& pair : positiveTestCases) {
      std::string lineNumOne = std::to_string(pair.first);
      std::string lineNumTwo = std::to_string(pair.second);
      bool testNext = retrieveResFromPkb(lineNumOne, lineNumTwo);
      Assert::IsTrue(testNext);
    }

    std::vector<std::pair<int, int>> negativeTestCases =
      generateNegTestCases(12, positiveTestCases);

    for (auto& pair : negativeTestCases) {
      std::string lineNumOne = std::to_string(pair.first);
      std::string lineNumTwo = std::to_string(pair.second);
      bool testNext = retrieveResFromPkb(lineNumOne, lineNumTwo);
      Assert::IsFalse(testNext);
    }
  }

  TEST_METHOD(TestExtractSingleProcWhileNestedIf) {
    /*
     *  procedure a {
     * 1. while (1 >= 1) {
     * 2.   c = d - e;
     * 3.   print c;
     * 4.   while (1 >= 1) {
     * 5.     print c;
     * 6.     read g;
     *      }
     * 7.   print c;
     *    }
     * 8.   g = h;
     *  }
     *
     */

    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenWhile, tokenOpenBracket, tokenConstant, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenOpenBrace,
      tokenC, tokenAssign, tokenD, tokenMinusOp, tokenE, tokenSemiColon,
      tokenPrint, tokenC, tokenSemiColon,
      tokenWhile, tokenOpenBracket, tokenConstant, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenOpenBrace,
      tokenPrint, tokenC, tokenSemiColon,
      tokenRead, tokenG, tokenSemiColon,
      tokenCloseBrace,
      tokenPrint, tokenC, tokenSemiColon,
      tokenCloseBrace,
      tokenG, tokenAssign, tokenH, tokenSemiColon,
      tokenCloseBrace
    };
    spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
    for (auto token : tokenList) {
      tokenStream.pushBack(token);
    }
    auto parser = spa::SpParser(tokenStream);
    std::vector<spa::ProcedureStatement> procedureList = parser.parse();
    Assert::IsTrue(procedureList.size() == 1);

    spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
    designExtractor.extractRelationship();

    std::vector<std::pair<int, int>> positiveTestCases = {
      {1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 6}, {6, 4}, {4, 7}, {7, 1}, {1, 8}
    };

    for (auto& pair : positiveTestCases) {
      std::string lineNumOne = std::to_string(pair.first);
      std::string lineNumTwo = std::to_string(pair.second);
      bool testNext = retrieveResFromPkb(lineNumOne, lineNumTwo);
      Assert::IsTrue(testNext);
    }

    std::vector<std::pair<int, int>> negativeTestCases = generateNegTestCases(8, positiveTestCases);

    for (auto& pair : negativeTestCases) {
      std::string lineNumOne = std::to_string(pair.first);
      std::string lineNumTwo = std::to_string(pair.second);
      bool testNext = retrieveResFromPkb(lineNumOne, lineNumTwo);
      Assert::IsFalse(testNext);
    }
  }

  TEST_METHOD(TestExtractNextMultiProc) {
    /*
     *  procedure a {
     * 1. c = d - e;
     * 2. print c;
     * 3. read g;
     * 4. g = h;
     *  }
     *
     *  procedure b {
     * 5. c = d - e;
     * 6. print c;
     * 7. read g;
     * 8. g = h;
     *  }
     */

    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenC, tokenAssign, tokenD, tokenMinusOp, tokenE, tokenSemiColon,
      tokenPrint, tokenC, tokenSemiColon,
      tokenRead, tokenG, tokenSemiColon,
      tokenG, tokenAssign, tokenH, tokenSemiColon,
      tokenCloseBrace,
      tokenProcedure, tokenB, tokenOpenBrace,
      tokenC, tokenAssign, tokenD, tokenMinusOp, tokenE, tokenSemiColon,
      tokenPrint, tokenC, tokenSemiColon,
      tokenRead, tokenG, tokenSemiColon,
      tokenG, tokenAssign, tokenH, tokenSemiColon,
      tokenCloseBrace
    };
    spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
    for (auto token : tokenList) {
      tokenStream.pushBack(token);
    }
    auto parser = spa::SpParser(tokenStream);
    std::vector<spa::ProcedureStatement> procedureList = parser.parse();
    Assert::IsTrue(procedureList.size() == 2);

    spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
    designExtractor.extractRelationship();

    std::vector<std::pair<int, int>> positiveTestCases = {
      {1, 2}, {2, 3}, {3, 4}, {5, 6}, {6, 7}, {7, 8}
    };

    for (auto& pair : positiveTestCases) {
      std::string lineNumOne = std::to_string(pair.first);
      std::string lineNumTwo = std::to_string(pair.second);
      bool testNext = retrieveResFromPkb(lineNumOne, lineNumTwo);
      Assert::IsTrue(testNext);
    }

    std::vector<std::pair<int, int>> negativeTestCases = generateNegTestCases(8, positiveTestCases);

    for (auto& pair : negativeTestCases) {
      std::string lineNumOne = std::to_string(pair.first);
      std::string lineNumTwo = std::to_string(pair.second);
      bool testNext = retrieveResFromPkb(lineNumOne, lineNumTwo);
      Assert::IsFalse(testNext);
    }
  }
};
}  // namespace UnitTesting
