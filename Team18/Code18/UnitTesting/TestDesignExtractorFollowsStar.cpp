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
TEST_CLASS(TestDesignExtractorFollowsStar) {
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
  TEST_METHOD(TestExtractSingleFollowsStarWithNoNesting) {
    /*
     *   procedure a {
     * 1. b = 1;
     * 2. c = b;
     *  }
     */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenB, tokenAssign, tokenConstant, tokenSemiColon,
      tokenC, tokenAssign, tokenB, tokenSemiColon,
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

    spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, "1", {});
    spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LINE_NO, "2", {});
    spa::QueryResult firstRes = pkbManager->getRelationship(spa::FOLLOWS_STAR,
      spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));

    Assert::IsTrue(firstRes.getQueryResultType() == spa::BOOL);

    bool testFirstFollows = firstRes.getIsTrue();

    bool expectedFirstFollows = true;

    Assert::IsTrue(expectedFirstFollows == testFirstFollows);
  }

  TEST_METHOD(TestExtractMultipleFollowsStarWithNoNesting) {
    /*
     *   procedure a {
     * 1. b = 1;
     * 2. c = b;
     * 3. d = b - c;
     *  }
     */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenB, tokenAssign, tokenConstant, tokenSemiColon,
      tokenC, tokenAssign, tokenB, tokenSemiColon,
      tokenD, tokenAssign, tokenB, tokenMinusOp, tokenC, tokenSemiColon,
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

    std::vector<std::pair<std::string, std::string>> positiveResTestCases = {
      {"1", "2"}, {"2", "3"}, {"1", "3"}
    };

    for (auto pair : positiveResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first, {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LINE_NO, pair.second, {});
      spa::QueryResult results = pkbManager->getRelationship(spa::FOLLOWS_STAR,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testFollowStar = results.getIsTrue();
      Assert::IsTrue(testFollowStar);
    }
  }

  TEST_METHOD(TestExtractFollowsStarWithWhileNesting) {
    /*
     *   procedure a {
     * 1. b = 1;
     * 2. while (b >= 1) {
     * 3.   d = b;
     * 4.   d = b;
     *    }
     * 5. d = b;
     *  }
     */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenB, tokenAssign, tokenConstant, tokenSemiColon,
      tokenWhile, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenOpenBrace,
      tokenD, tokenAssign, tokenB, tokenSemiColon,
      tokenD, tokenAssign, tokenB, tokenSemiColon,
      tokenCloseBrace,
      tokenD, tokenAssign, tokenB, tokenSemiColon,
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

    std::vector<std::pair<std::string, std::string>> positiveResTestCases = {
      {"1", "2"}, {"3", "4"}, {"2", "5"}, {"1", "5"}
    };

    for (auto pair : positiveResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first, {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LINE_NO, pair.second, {});
      spa::QueryResult results = pkbManager->getRelationship(spa::FOLLOWS_STAR,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testFollowStar = results.getIsTrue();
      Assert::IsTrue(testFollowStar);
    }

    std::vector<std::pair<std::string, std::string>> negResTestCases = {
      {"1", "3"}, {"1", "4"}, {"2", "3"}, {"2", "4"}, {"3", "5"}, {"4", "5"}
    };

    for (auto pair : negResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first, {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LINE_NO, pair.second, {});
      spa::QueryResult results = pkbManager->getRelationship(spa::FOLLOWS_STAR,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testFollowStar = results.getIsTrue();
      Assert::IsFalse(testFollowStar);
    }
  }

  TEST_METHOD(TestExtractFollowsStarWithIfNesting) {
    /*
     *   procedure a {
     * 1. b = 1;
     * 2. if (b >= 1) then {
     * 3.   d = b;
     * 4.   d = b;
     *    } else {
     * 5.   d = b;
     * 6.   d = b;
     *    }
     * 7. d = b;
     *  }
     */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenB, tokenAssign, tokenConstant, tokenSemiColon,
      tokenIf, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenThen, tokenOpenBrace,
      tokenD, tokenAssign, tokenB, tokenSemiColon,
      tokenD, tokenAssign, tokenB, tokenSemiColon,
      tokenCloseBrace, tokenElse, tokenOpenBrace,
      tokenD, tokenAssign, tokenB, tokenSemiColon,
      tokenD, tokenAssign, tokenB, tokenSemiColon,
      tokenCloseBrace,
      tokenD, tokenAssign, tokenB, tokenSemiColon,
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

    std::vector<std::pair<std::string, std::string>> positiveResTestCases = {
      {"1", "2"}, {"3", "4"}, {"5", "6"}, {"1", "7"}, {"2", "7"},
    };

    for (auto pair : positiveResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first, {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LINE_NO, pair.second, {});
      spa::QueryResult results = pkbManager->getRelationship(spa::FOLLOWS_STAR,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testFollowStar = results.getIsTrue();
      Assert::IsTrue(testFollowStar);
    }

    std::vector<std::pair<std::string, std::string>> negResTestCases = {
      {"1", "3"}, {"2", "3"}, {"2", "4"}, {"3", "5"}, {"3", "6"}, {"4", "5"}, {"4", "6"},
      {"3", "7"}, {"3", "7"}, {"4", "7"}, {"5", "7"}, {"6", "7"}
    };

    for (auto pair : negResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first, {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LINE_NO, pair.second, {});
      spa::QueryResult results = pkbManager->getRelationship(spa::FOLLOWS_STAR,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testFollowStar = results.getIsTrue();
      Assert::IsFalse(testFollowStar);
    }
  }

  TEST_METHOD(TestExtractFollowsStarWhileNestedWhile) {
    /*
    *   procedure a {
    * 1. d = b;
    * 2. while (b >= 1) {
    * 3.   d = b;
    * 4.   while (b >= 1) {
    * 5.    d = b;
    * 6.    d = b;
    *      }
    *    }
    * 7. d = b;
    *  }
    */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenD, tokenAssign, tokenB, tokenSemiColon,
      tokenWhile, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenOpenBrace,
      tokenD, tokenAssign, tokenB, tokenSemiColon,
      tokenWhile, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenOpenBrace,
      tokenD, tokenAssign, tokenB, tokenSemiColon,
      tokenD, tokenAssign, tokenB, tokenSemiColon,
      tokenCloseBrace, tokenCloseBrace,
      tokenD, tokenAssign, tokenB, tokenSemiColon,
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

    std::vector<std::pair<std::string, std::string>> positiveResTestCases = {
      {"1", "2"}, {"1", "7"}, {"2", "7"}, {"3", "4"}, {"5", "6"}
    };

    for (auto pair : positiveResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first, {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LINE_NO, pair.second, {});
      spa::QueryResult results = pkbManager->getRelationship(spa::FOLLOWS_STAR,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testFollowStar = results.getIsTrue();
      Assert::IsTrue(testFollowStar);
    }

    std::vector<std::pair<std::string, std::string>> negResTestCases = {
      {"1", "3"}, {"1", "4"}, {"1", "5"}, {"1", "6"}, {"2", "3"}, {"2", "4"}, {"2", "5"},
      {"2", "6"}, {"3", "5"}, {"3", "6"}, {"4", "5"}, {"4", "6"}, {"5", "7"}, {"6", "7"}
    };

    for (auto pair : negResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first, {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LINE_NO, pair.second, {});
      spa::QueryResult results = pkbManager->getRelationship(spa::FOLLOWS_STAR,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testFollowStar = results.getIsTrue();
      Assert::IsFalse(testFollowStar);
    }
  }

  TEST_METHOD(TestExtractFollowsStarIfNestedThen) {
    /*
     *   procedure a {
     * 1. b = 1;
     * 2. if (b >= 1) then {
     * 3.   d = b;
     * 4.   if (b >= 1) then {
     * 5.     d = b;
     * 6.     d = b;
     *      } else {
     * 7.     d = b;
     * 8.     d = b;
     *      }
     *    } else {
     * 9.   d = b;
     *    }
     * 10. d = b;
     *  }
     */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenB, tokenAssign, tokenConstant, tokenSemiColon,
      tokenIf, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenThen, tokenOpenBrace,
      tokenD, tokenAssign, tokenB, tokenSemiColon,
      tokenIf, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenConstant, tokenThen,
      tokenOpenBrace, tokenD, tokenAssign, tokenB, tokenSemiColon,
      tokenD, tokenAssign, tokenB, tokenSemiColon, tokenCloseBrace,
      tokenElse, tokenOpenBrace,
      tokenD, tokenAssign, tokenB, tokenSemiColon,
      tokenD, tokenAssign, tokenB, tokenSemiColon, tokenCloseBrace,
      tokenCloseBrace, tokenElse, tokenOpenBrace,
      tokenD, tokenAssign, tokenB, tokenSemiColon,
      tokenCloseBrace,
      tokenD, tokenAssign, tokenB, tokenSemiColon,
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

    std::vector<std::pair<std::string, std::string>> positiveResTestCases = {
      {"1", "2"}, {"1", "10"}, {"2", "10"}, {"3", "4"}, {"5", "6"}, {"7", "8"}
    };

    for (auto pair : positiveResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first, {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LINE_NO, pair.second, {});
      spa::QueryResult results = pkbManager->getRelationship(spa::FOLLOWS_STAR,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testFollowStar = results.getIsTrue();
      Assert::IsTrue(testFollowStar);
    }

    std::vector<std::pair<std::string, std::string>> negResTestCases = {
      {"1", "3"}, {"1", "4"}, {"1", "5"}, {"1", "6"}, {"1", "7"}, {"1", "8"}, {"1", "9"},
      {"2", "3"}, {"2", "4"}, {"2", "5"}, {"2", "6"}, {"2", "7"}, {"2", "8"}, {"2", "9"},
      {"3", "5"}, {"3", "6"}, {"3", "7"}, {"3", "8"}, {"3", "9"}, {"3", "10"}, {"4", "5"},
      {"4", "6"}, {"4", "7"}, {"4", "8"}, {"4", "9"}, {"4", "10"}, {"5", "7"}, {"5", "8"},
      {"5", "9"}, {"5", "10"}, {"6", "7"}, {"6", "8"}, {"6", "9"}, {"6", "10"}, {"7", "9"},
      {"7", "10"}, {"8", "9"}, {"8", "10"}, {"9", "10"}
    };

    for (auto pair : negResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first, {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LINE_NO, pair.second, {});
      spa::QueryResult results = pkbManager->getRelationship(spa::FOLLOWS_STAR,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testFollowStar = results.getIsTrue();
      Assert::IsFalse(testFollowStar);
    }
  }

  TEST_METHOD(TestExtractFollowsStarIfNestedElse) {
    /*
     *   procedure a {
     * 1. b = 1;
     * 2. if (b >= 1) then {
     * 3.   d = b;
     *    } else {
     * 4.   if (b >= 1) then {
     * 5.     d = b;
     * 6.     d = b;
     *      } else {
     * 7.     d = b;
     * 8.     d = b;
     *      }
     * 9.   d = b;
     *    }
     * 10. d = b;
     *  }
     */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenB, tokenAssign, tokenConstant, tokenSemiColon,
      tokenIf, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenThen, tokenOpenBrace,
      tokenD, tokenAssign, tokenB, tokenSemiColon,
      tokenCloseBrace, tokenElse, tokenOpenBrace,
      tokenIf, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenConstant, tokenThen,
      tokenOpenBrace, tokenD, tokenAssign, tokenB, tokenSemiColon,
      tokenD, tokenAssign, tokenB, tokenSemiColon, tokenCloseBrace,
      tokenElse, tokenOpenBrace,
      tokenD, tokenAssign, tokenB, tokenSemiColon,
      tokenD, tokenAssign, tokenB, tokenSemiColon, tokenCloseBrace,
      tokenD, tokenAssign, tokenB, tokenSemiColon,
      tokenCloseBrace,
      tokenD, tokenAssign, tokenB, tokenSemiColon,
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

    std::vector<std::pair<std::string, std::string>> positiveResTestCases = {
      {"1", "2"}, {"1", "10"}, {"2", "10"}, {"5", "6"}, {"7", "8"}, {"4", "9"}
    };

    for (auto pair : positiveResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first, {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LINE_NO, pair.second, {});
      spa::QueryResult results = pkbManager->getRelationship(spa::FOLLOWS_STAR,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testFollowStar = results.getIsTrue();
      Assert::IsTrue(testFollowStar);
    }

    std::vector<std::pair<std::string, std::string>> negResTestCases = {
      {"1", "3"}, {"1", "4"}, {"1", "5"}, {"1", "6"}, {"1", "7"}, {"1", "8"}, {"1", "9"},
      {"2", "3"}, {"2", "4"}, {"2", "5"}, {"2", "6"}, {"2", "7"}, {"2", "8"}, {"2", "9"},
      {"3", "4"}, {"3", "5"}, {"3", "6"}, {"3", "7"}, {"3", "8"}, {"3", "9"}, {"3", "10"},
      {"4", "5"}, {"4", "6"}, {"4", "7"}, {"4", "8"}, {"4", "10"}, {"5", "7"}, {"5", "8"},
      {"5", "9"}, {"5", "10"}, {"6", "7"}, {"6", "8"}, {"6", "9"}, {"6", "10"}, {"7", "9"},
      {"7", "10"}, {"8", "9"}, {"8", "10"}, {"9", "10"}
    };

    for (auto pair : negResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first, {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LINE_NO, pair.second, {});
      spa::QueryResult results = pkbManager->getRelationship(spa::FOLLOWS_STAR,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testFollowStar = results.getIsTrue();
      Assert::IsFalse(testFollowStar);
    }
  }

  TEST_METHOD(TestExtractFollowsStarWhileNestedThen) {
    /*
     *   procedure a {
     * 1. b = 1;
     * 2. if (b >= 1) then {
     * 3.   d = b;
     * 4.   while (b >= 1) {
     * 5.     d = b;
     * 6.     d = b;
     *      }
     *    } else {
     * 7.   d = b;
     *    }
     * 8. d = b;
     *  }
     */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenB, tokenAssign, tokenConstant, tokenSemiColon,
      tokenIf, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenThen, tokenOpenBrace,
      tokenD, tokenAssign, tokenB, tokenSemiColon,
      tokenWhile, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenConstant,
      tokenOpenBrace, tokenD, tokenAssign, tokenB, tokenSemiColon,
      tokenD, tokenAssign, tokenB, tokenSemiColon, tokenCloseBrace,
      tokenCloseBrace, tokenElse, tokenOpenBrace,
      tokenD, tokenAssign, tokenB, tokenSemiColon,
      tokenCloseBrace,
      tokenD, tokenAssign, tokenB, tokenSemiColon,
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

    std::vector<std::pair<std::string, std::string>> positiveResTestCases = {
      {"1", "2"}, {"3", "4"}, {"5", "6"}, {"1", "8"}, {"2", "8"},
    };

    for (auto pair : positiveResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first, {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LINE_NO, pair.second, {});
      spa::QueryResult results = pkbManager->getRelationship(spa::FOLLOWS_STAR,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testFollowStar = results.getIsTrue();
      Assert::IsTrue(testFollowStar);
    }

    std::vector<std::pair<std::string, std::string>> negResTestCases = {
      {"1", "3"}, {"1", "4"}, {"1", "5"}, {"1", "6"}, {"1", "7"}, {"2", "3"}, {"2", "4"},
      {"2", "5"}, {"2", "6"}, {"2", "7"}, {"3", "5"}, {"3", "6"}, {"3", "7"}, {"3", "8"},
      {"4", "5"}, {"4", "6"}, {"4", "7"}, {"4", "8"}, {"5", "7"}, {"5", "8"}, {"6", "7"},
      {"6", "8"}, {"7", "8"}
    };

    for (auto pair : negResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first, {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LINE_NO, pair.second, {});
      spa::QueryResult results = pkbManager->getRelationship(spa::FOLLOWS_STAR,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testFollowStar = results.getIsTrue();
      Assert::IsFalse(testFollowStar);
    }
  }

  TEST_METHOD(TestExtractFollowsStarWhileNestedElse) {
    /*
     *   procedure a {
     * 1. b = 1;
     * 2. if (b >= 1) then {
     * 3.   d = b;
     *    } else {
     * 4.   while (b >= 1) {
     * 5.     d = b;
     * 6.     d = b;
     *      }
     * 7.   d = b;
     *    }
     * 8. d = b;
     *  }
     */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenB, tokenAssign, tokenConstant, tokenSemiColon,
      tokenIf, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenThen, tokenOpenBrace,
      tokenD, tokenAssign, tokenB, tokenSemiColon,
      tokenCloseBrace, tokenElse, tokenOpenBrace,
      tokenWhile, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenConstant,
      tokenOpenBrace, tokenD, tokenAssign, tokenB, tokenSemiColon,
      tokenD, tokenAssign, tokenB, tokenSemiColon, tokenCloseBrace,
      tokenD, tokenAssign, tokenB, tokenSemiColon,
      tokenCloseBrace,
      tokenD, tokenAssign, tokenB, tokenSemiColon,
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

    std::vector<std::pair<std::string, std::string>> positiveResTestCases = {
      {"1", "2"}, {"1", "8"}, {"2", "8"}, {"4", "7"}, {"5", "6"},
    };

    for (auto pair : positiveResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first, {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LINE_NO, pair.second, {});
      spa::QueryResult results = pkbManager->getRelationship(spa::FOLLOWS_STAR,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testFollowStar = results.getIsTrue();
      Assert::IsTrue(testFollowStar);
    }

    std::vector<std::pair<std::string, std::string>> negResTestCases = {
      {"1", "3"}, {"1", "4"}, {"1", "5"}, {"1", "6"}, {"1", "7"}, {"2", "3"}, {"2", "4"},
      {"2", "5"}, {"2", "6"}, {"2", "7"}, {"3", "4"}, {"3", "5"}, {"3", "6"}, {"3", "7"},
      {"3", "8"}, {"4", "5"}, {"4", "6"}, {"4", "8"}, {"5", "7"}, {"5", "8"}, {"6", "7"},
      {"6", "8"}, {"7", "8"}
    };

    for (auto pair : negResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first, {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LINE_NO, pair.second, {});
      spa::QueryResult results = pkbManager->getRelationship(spa::FOLLOWS_STAR,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testFollowStar = results.getIsTrue();
      Assert::IsFalse(testFollowStar);
    }
  }

  TEST_METHOD(TestExtractFollowsStarIfNestedWhile) {
    /*
     *   procedure a {
     * 1. d = b;
     * 2. while (b >= 1) {
     * 3.   d = b;
     * 4.   if (b >= 1) then {
     * 5.    d = b;
     * 6.    d = b;
     *      } else {
     * 7.    d = b;
     * 8.    d = b;
     *      }
     *    }
     * 9. d = b;
     *  }
     */
    tokenList = {
      tokenProcedure, tokenA, tokenOpenBrace,
      tokenD, tokenAssign, tokenB, tokenSemiColon,
      tokenWhile, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenOpenBrace,
      tokenD, tokenAssign, tokenB, tokenSemiColon,
      tokenIf, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenConstant,
      tokenCloseBracket, tokenThen, tokenOpenBrace,
      tokenD, tokenAssign, tokenB, tokenSemiColon,
      tokenD, tokenAssign, tokenB, tokenSemiColon,
      tokenCloseBrace, tokenElse, tokenOpenBrace,
      tokenD, tokenAssign, tokenB, tokenSemiColon,
      tokenD, tokenAssign, tokenB, tokenSemiColon,
      tokenCloseBrace, tokenCloseBrace,
      tokenD, tokenAssign, tokenB, tokenSemiColon,
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

    std::vector<std::pair<std::string, std::string>> positiveResTestCases = {
      {"1", "2"}, {"1", "9"}, {"2", "9"}, {"3", "4"}, {"5", "6"}, {"7", "8"}
    };

    for (auto pair : positiveResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first, {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LINE_NO, pair.second, {});
      spa::QueryResult results = pkbManager->getRelationship(spa::FOLLOWS_STAR,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testFollowStar = results.getIsTrue();
      Assert::IsTrue(testFollowStar);
    }

    std::vector<std::pair<std::string, std::string>> negResTestCases = {
      {"1", "3"}, {"1", "4"}, {"1", "5"}, {"1", "6"}, {"1", "7"}, {"1", "8"}, {"2", "3"},
      {"2", "4"}, {"2", "5"}, {"2", "6"}, {"2", "7"}, {"2", "8"}, {"3", "5"}, {"3", "6"},
      {"3", "7"},
      {"3", "8"}, {"3", "9"}, {"4", "5"}, {"4", "6"}, {"4", "7"}, {"4", "8"}, {"4", "9"},
      {"5", "7"}, {"5", "8"},
      {"5", "9"}, {"6", "7"}, {"6", "8"}, {"6", "9"}, {"7", "9"}, {"8", "9"}
    };

    for (auto pair : negResTestCases) {
      spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LINE_NO, pair.first, {});
      spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LINE_NO, pair.second, {});
      spa::QueryResult results = pkbManager->getRelationship(spa::FOLLOWS_STAR,
        spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));
      bool testFollowStar = results.getIsTrue();
      Assert::IsFalse(testFollowStar);
    }
  }
};
}  // namespace UnitTesting
