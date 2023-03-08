#include <stdafx.h>
#include <iostream>
#include <vector>
#include <algorithm>

#include "DesignExtractor.h"
#include "PKB.h"
#include "SpParser.h"
#include "Token.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace UnitTesting {
TEST_CLASS(TestSpCreateCFG) {
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

  void addEdges(spa::CFGNode* cfgNode, std::unordered_set<spa::CFGNode*> incoming,
                std::unordered_set<spa::CFGNode*> outgoing) {
    cfgNode->addInEdges(incoming);
    cfgNode->addOutEdges(outgoing);
  }

  void assertCFGMatch(std::unordered_set<spa::CFGNode*> expectedNodes) {
    for (auto& expected : expectedNodes) {
      spa::QueryResult queryResult = pkbManager->getCfgNode(expected->getLineNumber());
      std::vector<spa::CFGNode*> testNodes = queryResult.getCfgNodes();
      if (!testNodes.empty()) {
        spa::CFGNode* test = testNodes[0];
        Assert::IsTrue(expected->equal(test));
        Assert::IsTrue(expected->getIncomingEdges().size() == test->getIncomingEdges().size());
        Assert::IsTrue(expected->getOutgoingEdges().size() == test->getOutgoingEdges().size());
        //for (spa::CFGNode* node : expected->getIncomingEdges()) {
        //    std::cout << node->getLineNumber();
        //}
        //for (spa::CFGNode* node : test->getIncomingEdges()) {
        //    std::cout << node->getLineNumber();
        //}
        std::vector<spa::CFGNode*> expectedInEdgeVec = convertToSortedVector(
          expected->getIncomingEdges());
        std::vector<spa::CFGNode*> testInEdgeVec = convertToSortedVector(test->getIncomingEdges());
        for (int i = 0; i < expectedInEdgeVec.size(); i++) {
          Assert::IsTrue(expectedInEdgeVec[i]->equal(testInEdgeVec[i]));
        }
        std::vector<spa::CFGNode*> expectedOutEdgeVec = convertToSortedVector(
          expected->getOutgoingEdges());
        std::vector<spa::CFGNode*> testOutEdgeVec = convertToSortedVector(test->getOutgoingEdges());
        for (int i = 0; i < expectedOutEdgeVec.size(); i++) {
          Assert::IsTrue(expectedOutEdgeVec[i]->equal(testOutEdgeVec[i]));
        }
      }
    }
  }

  std::vector<spa::CFGNode*> convertToSortedVector(std::unordered_set<spa::CFGNode*> edges) {
    std::vector<spa::CFGNode*> edgesVector(edges.begin(), edges.end());
    std::sort(edgesVector.begin(), edgesVector.end(),
      [](const spa::CFGNode* a, const spa::CFGNode* b) {
        return a->getLineNumber() < b->getLineNumber();
      });
    return edgesVector;
  }

public:
  TEST_METHOD_INITIALIZE(BeforeEach) {
    pkbManager = std::make_unique<spa::PKB>();
  }

  TEST_METHOD(TestSingleProcCFGCreation) {
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

    auto lineOne = new spa::CFGNode(1, std::string("c"));
    auto lineTwo = new spa::CFGNode(2);
    auto lineThree = new spa::CFGNode(3, std::string("g"));
    auto lineFour = new spa::CFGNode(4, std::string("g"));
    addEdges(lineOne, {}, {lineTwo});
    addEdges(lineTwo, {lineOne}, {lineThree});
    addEdges(lineThree, {lineTwo}, {lineFour});
    addEdges(lineFour, {lineThree}, {});
    std::unordered_set<spa::CFGNode*> expectedNodes = {lineOne, lineTwo, lineThree, lineFour};
    assertCFGMatch(expectedNodes);
  }

  TEST_METHOD(TestSingleProcCFGCreationWhileLast) {
    /*
     *  procedure a {
     * 1. while (1 >= 1) {
     * 2.   c = d - e;
     * 3.   print c;
     * 4.   read g;
     *    }
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
      tokenCloseBrace, tokenCloseBrace
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

    auto lineOne = new spa::CFGNode(1);
    auto lineTwo = new spa::CFGNode(2, std::string("c"));
    auto lineThree = new spa::CFGNode(3);
    auto lineFour = new spa::CFGNode(4, std::string("g"));
    addEdges(lineOne, {lineFour}, {lineTwo});
    addEdges(lineTwo, {lineOne}, {lineThree});
    addEdges(lineThree, {lineTwo}, {lineFour});
    addEdges(lineFour, {lineThree}, {lineOne});
    std::unordered_set<spa::CFGNode*> expectedNodes = {lineOne, lineTwo, lineThree, lineFour};
    assertCFGMatch(expectedNodes);
  }

  TEST_METHOD(TestSingleProcCFGCreationWhile) {
      /*
       *  procedure a {
       * 1. while (1 >= 1) {
       * 2.   c = d - e;
       * 3.   print c;
       * 4.   read g;
       *    }
       * 5. g = h;
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

      auto lineOne = new spa::CFGNode(1);
      auto lineTwo = new spa::CFGNode(2, std::string("c"));
      auto lineThree = new spa::CFGNode(3);
      auto lineFour = new spa::CFGNode(4, std::string("g"));
      auto lineFive = new spa::CFGNode(5, std::string("g"));
      addEdges(lineOne, {lineFour}, { lineTwo, lineFive });
      addEdges(lineTwo, { lineOne }, { lineThree });
      addEdges(lineThree, { lineTwo }, { lineFour });
      addEdges(lineFour, { lineThree }, { lineOne });
      addEdges(lineFive, { lineOne}, {});
      std::unordered_set<spa::CFGNode*> expectedNodes = { lineOne, lineTwo, lineThree, lineFour, lineFive };
      assertCFGMatch(expectedNodes);
  }

  TEST_METHOD(TestSingleProcCFGCreationIfLast) {
    /*
     *  procedure a {
     * 1. if (1 >= 1) then {
     * 2.   c = d - e;
     * 3.   print c;
     *    } else {
     * 4.   read g;
     * 5.   print c;
     *    }
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
      tokenCloseBrace, tokenCloseBrace
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

    auto lineOne = new spa::CFGNode(1);
    auto lineTwo = new spa::CFGNode(2, std::string("c"));
    auto lineThree = new spa::CFGNode(3);
    auto lineFour = new spa::CFGNode(4, std::string("g"));
    auto lineFive = new spa::CFGNode(5);
    auto dummyNode = spa::CFGNode::createDummyNode();
    addEdges(lineOne, {}, {lineTwo, lineFour});
    addEdges(lineTwo, {lineOne}, {lineThree});
    addEdges(lineThree, {lineTwo}, {dummyNode});
    addEdges(lineFour, {lineOne}, {lineFive});
    addEdges(lineFive, {lineFour}, {dummyNode});
    std::unordered_set<spa::CFGNode*> expectedNodes = {
      lineOne, lineTwo, lineThree, lineFour, lineFive
    };
    assertCFGMatch(expectedNodes);
  }

  TEST_METHOD(TestSingleProcCFGCreationIf) {
      /*
       *  procedure a {
       * 1. if (1 >= 1) then {
       * 2.   c = d - e;
       * 3.   print c;
       *    } else {
       * 4.   read g;
       * 5.   print c;
       *    }
       * 6. g = h;
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

      auto lineOne = new spa::CFGNode(1);
      auto lineTwo = new spa::CFGNode(2, std::string("c"));
      auto lineThree = new spa::CFGNode(3);
      auto lineFour = new spa::CFGNode(4, std::string("g"));
      auto lineFive = new spa::CFGNode(5);
      auto lineSix = new spa::CFGNode(6, std::string("g"));
      addEdges(lineOne, {}, { lineTwo, lineFour});
      addEdges(lineTwo, { lineOne }, { lineThree });
      addEdges(lineThree, { lineTwo }, { lineSix });
      addEdges(lineFour, { lineOne }, { lineFive });
      addEdges(lineFive, { lineFour }, { lineSix });
      addEdges(lineSix, { lineThree, lineFive }, {});
      std::unordered_set<spa::CFGNode*> expectedNodes = {
        lineOne, lineTwo, lineThree, lineFour, lineFive, lineSix
      };
      assertCFGMatch(expectedNodes);
  }

  TEST_METHOD(TestSingleProcCFGCreationWhileNestedWhileLast) {
      /*
       *  procedure a {
       * 1. while (1 >= 1) {
       * 2.   c = d - e;
       * 3.   print c;
       * 4.   while (1 >= 1) {
       * 5.     print c;
       * 6.     read g;
       *      }
       *    }
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
        tokenCloseBrace,
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

      auto lineOne = new spa::CFGNode(1);
      auto lineTwo = new spa::CFGNode(2, std::string("c"));
      auto lineThree = new spa::CFGNode(3);
      auto lineFour = new spa::CFGNode(4);
      auto lineFive = new spa::CFGNode(5);
      auto lineSix = new spa::CFGNode(6, std::string("g"));
      addEdges(lineOne, { lineFour }, { lineTwo });
      addEdges(lineTwo, { lineOne }, { lineThree });
      addEdges(lineThree, { lineTwo }, { lineFour });
      addEdges(lineFour, { lineThree,lineSix }, { lineOne, lineFive });
      addEdges(lineFive, { lineFour }, { lineSix });
      addEdges(lineSix, { lineFive }, { lineFour });
      std::unordered_set<spa::CFGNode*> expectedNodes = {
        lineOne, lineTwo, lineThree, lineFour, lineFive, lineSix
      };
      assertCFGMatch(expectedNodes);
  }

  TEST_METHOD(TestSingleProcCFGCreationIfNestedIfLast) {
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
       *    }
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
        tokenCloseBrace,
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

      auto lineOne = new spa::CFGNode(1);
      auto lineTwo = new spa::CFGNode(2, std::string("c"));
      auto lineThree = new spa::CFGNode(3);
      auto lineFour = new spa::CFGNode(4, std::string("c"));
      auto lineFive = new spa::CFGNode(5);
      auto lineSix = new spa::CFGNode(6, std::string("g"));
      auto lineSeven = new spa::CFGNode(7);
      auto lineEight = new spa::CFGNode(8);
      auto lineNine = new spa::CFGNode(9, std::string("g"));
      auto lineTen = new spa::CFGNode(10);
      auto lineEleven = new spa::CFGNode(11, std::string("c"));
      auto lineTwelve = new spa::CFGNode(12);
      auto lineThirteen = new spa::CFGNode(13, std::string("g"));
      auto lineFourteen = new spa::CFGNode(14);
      auto dummyNode = spa::CFGNode::createDummyNode();
      addEdges(lineOne, {}, { lineTwo, lineNine } );
      addEdges(lineTwo, { lineOne }, { lineThree });
      addEdges(lineThree, { lineTwo }, { lineFour, lineSix });
      addEdges(lineFour, { lineThree }, { lineFive });
      addEdges(lineFive, { lineFour }, { lineEight});
      addEdges(lineSix, { lineThree }, { lineSeven });
      addEdges(lineSeven, { lineSix }, { lineEight});
      addEdges(lineEight, { lineFive, lineSeven }, { dummyNode });
      addEdges(lineNine, { lineOne}, { lineTen });
      addEdges(lineTen, { lineNine }, { lineEleven, lineThirteen });
      addEdges(lineEleven, { lineTen }, { lineTwelve });
      addEdges(lineTwelve, { lineEleven }, { dummyNode });
      addEdges(lineThirteen, { lineTen }, { lineFourteen });
      addEdges(lineFourteen, { lineThirteen}, {dummyNode});
      std::unordered_set<spa::CFGNode*> expectedNodes = {
        lineOne, lineTwo, lineThree, lineFour, lineFive, lineSix, lineSeven, lineEight, lineNine, lineTen, lineEleven, lineTwelve, lineThirteen, lineFourteen
      };
      assertCFGMatch(expectedNodes);
      }

  TEST_METHOD(TestExtractSingleProcIfNestedWhileLast) {
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
       *    }
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
        tokenCloseBrace,
        tokenCloseBrace
      };

      auto lineOne = new spa::CFGNode(1);
      auto lineTwo = new spa::CFGNode(2, std::string("c"));
      auto lineThree = new spa::CFGNode(3);
      auto lineFour = new spa::CFGNode(4, std::string("c"));
      auto lineFive = new spa::CFGNode(5);
      auto lineSix = new spa::CFGNode(6);
      auto lineSeven = new spa::CFGNode(7, std::string("g"));
      auto lineEight = new spa::CFGNode(8);
      auto lineNine = new spa::CFGNode(9, std::string("c"));
      auto lineTen = new spa::CFGNode(10);
      auto dummyNode = spa::CFGNode::createDummyNode();
      addEdges(lineOne, {}, { lineTwo, lineSeven });
      addEdges(lineTwo, { lineOne }, { lineThree });
      addEdges(lineThree, { lineTwo }, { lineFour, lineSix });
      addEdges(lineFour, { lineThree }, { lineFive });
      addEdges(lineFive, { lineFour }, { lineThree });
      addEdges(lineSix, { lineThree }, { dummyNode});
      addEdges(lineSeven, { lineSix }, {});
      addEdges(lineEight, { lineSeven }, { dummyNode });
      addEdges(lineNine, { lineEight}, { lineTen });
      addEdges(lineTen, { lineNine }, { lineEight });
      std::unordered_set<spa::CFGNode*> expectedNodes = {
        lineOne, lineTwo, lineThree, lineFour, lineFive, lineSix, lineSeven, lineEight, lineNine, lineTen
      };
      assertCFGMatch(expectedNodes);
  }

  TEST_METHOD(TestSingleProcCFGCreationWhileNestedIf) {
      /*
       *  procedure a {
       * 1. while (1 >= 1) {
       * 2.   c = d - e;
       * 3.   print c;
       * 4.   if (1 >= 1) then {
       * 5.     c = d - e;
       * 6.     print c;
       *    } else {
       * 7.     read g;
       * 8.     print c;
       *    }
       *  }
       *}
       */

      tokenList = {
        tokenProcedure, tokenA, tokenOpenBrace,
        tokenWhile, tokenOpenBracket, tokenConstant, tokenGreaterEqual, tokenConstant,
        tokenCloseBracket, tokenOpenBrace,
        tokenC, tokenAssign, tokenD, tokenMinusOp, tokenE, tokenSemiColon,
        tokenPrint, tokenC, tokenSemiColon,
        tokenIf, tokenOpenBracket, tokenConstant, tokenGreaterEqual, tokenConstant,
        tokenCloseBracket, tokenThen, tokenOpenBrace,
        tokenC, tokenAssign, tokenD, tokenMinusOp, tokenE, tokenSemiColon,
        tokenPrint, tokenC, tokenSemiColon,
        tokenCloseBrace, tokenElse, tokenOpenBrace,
        tokenRead, tokenG, tokenSemiColon,
        tokenPrint, tokenC, tokenSemiColon,
        tokenCloseBrace,
        tokenCloseBrace,
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

      auto lineOne = new spa::CFGNode(1);
      auto lineTwo = new spa::CFGNode(2, std::string("c"));
      auto lineThree = new spa::CFGNode(3);
      auto lineFour = new spa::CFGNode(4);
      auto lineFive = new spa::CFGNode(5, std::string("c"));
      auto lineSix = new spa::CFGNode(6);
      auto lineSeven = new spa::CFGNode(7, std::string("g"));
      auto lineEight = new spa::CFGNode(8);
      addEdges(lineOne, {lineSix,lineEight}, { lineTwo });
      addEdges(lineTwo, { lineOne }, { lineThree });
      addEdges(lineThree, { lineTwo }, { lineFour });
      addEdges(lineFour, { lineThree }, { lineFive, lineSeven });
      addEdges(lineFive, { lineFour }, { lineSix });
      addEdges(lineSix, { lineFive }, { lineOne });
      addEdges(lineSeven, { lineFour }, { lineEight });
      addEdges(lineEight, { lineSeven }, { lineOne });
      std::unordered_set<spa::CFGNode*> expectedNodes = {
        lineOne, lineTwo, lineThree, lineFour, lineFive, lineSix, lineSeven, lineEight
      };
      assertCFGMatch(expectedNodes);
  }

  TEST_METHOD(TestMultiProcCFGCreation) {
    /*
     *  procedure a {
     * 1. c = d - e;
     * 2. print c;
     * 3. call b;
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
      tokenCall, tokenB, tokenSemiColon,
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

    auto lineOne = new spa::CFGNode(1, std::string("c"));
    auto lineTwo = new spa::CFGNode(2);
    std::unordered_set<std::string> varModified = {"g", "c"};
    auto lineThree = new spa::CFGNode(3, varModified);
    auto lineFour = new spa::CFGNode(4, std::string("g"));
    auto lineFive = new spa::CFGNode(5, std::string("c"));
    auto lineSix = new spa::CFGNode(6);
    auto lineSeven = new spa::CFGNode(7, std::string("g"));
    auto lineEight = new spa::CFGNode(8, std::string("g"));
    addEdges(lineOne, {}, {lineTwo});
    addEdges(lineTwo, {lineOne}, {lineThree});
    addEdges(lineThree, {lineTwo}, {lineFour});
    addEdges(lineFour, {lineThree}, {});
    addEdges(lineFive, {}, {lineSix});
    addEdges(lineSix, {lineFive}, {lineSeven});
    addEdges(lineSeven, {lineSix}, {lineEight});
    addEdges(lineEight, {lineSeven}, {});
    std::unordered_set<spa::CFGNode*> expectedNodes = {
      lineOne, lineTwo, lineThree, lineFour, lineFive, lineSix, lineSeven, lineEight
    };
    assertCFGMatch(expectedNodes);
  }
};
}  // namespace UnitTesting
