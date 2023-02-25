#include <string>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <utility>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "PatternStorage.h"
#include "PqlArgument.h"
#include "PKBQueryArg.h"
#include "Argument.h"
#include "Token.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace UnitTesting {
  TEST_CLASS(TestPKBPatternStorage) {
    std::unordered_map<int, std::pair<std::string, std::string>> assignTable = {
      {1, {"a", "v x y * + z t * +"}},
    };
    std::unordered_map<int, std::unordered_set<std::string>> patternIfTable = {
      {1, {"x"}},
      {4, {"y", "z"}},
    };
    std::unordered_map<int, std::unordered_set<std::string>> patternWhileTable = {
      {7, {"a", "b"}},
      {8, {"c"}},
    };

    std::vector<spa::Token> tokens1 = {
      spa::Token(spa::TokenType::TOKEN_NAME, "v"),
      spa::Token(spa::TokenType::TOKEN_PLUS, "+"),
      spa::Token(spa::TokenType::TOKEN_NAME, "x"),
      spa::Token(spa::TokenType::TOKEN_MULTIPLY, "*"),
      spa::Token(spa::TokenType::TOKEN_NAME, "y"),
      spa::Token(spa::TokenType::TOKEN_PLUS, "+"),
      spa::Token(spa::TokenType::TOKEN_NAME, "z"),
      spa::Token(spa::TokenType::TOKEN_MULTIPLY, "*"),
      spa::Token(spa::TokenType::TOKEN_NAME, "t"),
    };

    TEST_METHOD(TestAddAssign) {
      spa::PatternStorage patternStorage;
      patternStorage.setAssignTable(assignTable);

      Assert::IsTrue(patternStorage.addAssign("2", "x", "1 2 +"));
      Assert::IsFalse(patternStorage.addAssign("2", "x", "1 2 +"));
    }

    TEST_METHOD(TestAddPatternIf) {
      spa::PatternStorage patternStorage;
      patternStorage.setPatternIfTable(patternIfTable);

      Assert::IsTrue(patternStorage.addPatternIf("1", "y"));
      Assert::IsFalse(patternStorage.addPatternIf("1", "y"));
    }

    TEST_METHOD(TestAddPatternWhile) {
      spa::PatternStorage patternStorage;
      patternStorage.setPatternWhileTable(patternWhileTable);

      Assert::IsTrue(patternStorage.addPatternWhile("8", "a"));
      Assert::IsFalse(patternStorage.addPatternWhile("8", "a"));
    }

    TEST_METHOD(TestGetAssignUnderscoreExact) {
      spa::PatternStorage patternStorage;
      patternStorage.setAssignTable(assignTable);
      std::vector<spa::Token> tokens = {
        spa::Token(spa::TokenType::TOKEN_NAME, "v"),
        spa::Token(spa::TokenType::TOKEN_PLUS, "+"),
        spa::Token(spa::TokenType::TOKEN_NAME, "x"),
        spa::Token(spa::TokenType::TOKEN_MULTIPLY, "*"),
        spa::Token(spa::TokenType::TOKEN_NAME, "y"),
        spa::Token(spa::TokenType::TOKEN_PLUS, "+"),
        spa::Token(spa::TokenType::TOKEN_NAME, "z"),
        spa::Token(spa::TokenType::TOKEN_MULTIPLY, "*"),
        spa::Token(spa::TokenType::TOKEN_NAME, "t"),
      };
      std::vector<std::pair<int, std::string>> expected = { {1, "a"} };

      spa::Pattern pattern(spa::PatternType::EXACT, tokens);
      spa::PKBQueryArg lhs = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
      spa::QueryResult queryResult = patternStorage.getAssignUnderscore(lhs, pattern);

      Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
      Assert::IsTrue(queryResult.getLineNumberNamePairs() == expected);

      tokens = { spa::Token(spa::TokenType::TOKEN_NAME, "v") };
      pattern = spa::Pattern(spa::PatternType::EXACT, tokens);
      queryResult = patternStorage.getAssignUnderscore(lhs, pattern);

      Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
      Assert::IsTrue(queryResult.getLineNumberNamePairs().empty());
    }

    TEST_METHOD(TestGetAssignUnderscorePartial) {
      spa::PatternStorage patternStorage;
      patternStorage.setAssignTable(assignTable);
      std::vector<spa::Token> tokens = { spa::Token(spa::TokenType::TOKEN_NAME, "v") };
      std::vector<std::pair<int, std::string>> expected = { {1, "a"} };

      spa::Pattern pattern(spa::PatternType::PARTIAL, tokens);
      spa::PKBQueryArg lhs = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
      spa::QueryResult queryResult = patternStorage.getAssignUnderscore(lhs, pattern);

      Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
      Assert::IsTrue(queryResult.getLineNumberNamePairs() == expected);

      tokens = {
        spa::Token(spa::TokenType::TOKEN_NAME, "v"),
        spa::Token(spa::TokenType::TOKEN_PLUS, "+"),
        spa::Token(spa::TokenType::TOKEN_NAME, "x"),
      };
      pattern = spa::Pattern(spa::PatternType::PARTIAL, tokens);
      queryResult = patternStorage.getAssignUnderscore(lhs, pattern);

      Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
      Assert::IsTrue(queryResult.getLineNumberNamePairs().empty());
    }

    TEST_METHOD(TestGetAssignUnderscoreAny) {
      spa::PatternStorage patternStorage;
      patternStorage.setAssignTable(assignTable);
      std::vector<spa::Token> tokens = { spa::Token(spa::TokenType::TOKEN_NAME, "v") };
      std::vector<std::pair<int, std::string>> expected = { {1, "a"} };

      spa::Pattern pattern(spa::PatternType::ANY);
      spa::PKBQueryArg lhs = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
      spa::QueryResult queryResult = patternStorage.getAssignUnderscore(lhs, pattern);

      Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
      Assert::IsTrue(queryResult.getLineNumberNamePairs() == expected);
    }

    TEST_METHOD(TestGetAssignVarExact) {
      spa::PatternStorage patternStorage;
      patternStorage.setAssignTable(assignTable);
      std::vector<spa::Token> tokens = {
        spa::Token(spa::TokenType::TOKEN_NAME, "v"),
        spa::Token(spa::TokenType::TOKEN_PLUS, "+"),
        spa::Token(spa::TokenType::TOKEN_NAME, "x"),
        spa::Token(spa::TokenType::TOKEN_MULTIPLY, "*"),
        spa::Token(spa::TokenType::TOKEN_NAME, "y"),
        spa::Token(spa::TokenType::TOKEN_PLUS, "+"),
        spa::Token(spa::TokenType::TOKEN_NAME, "z"),
        spa::Token(spa::TokenType::TOKEN_MULTIPLY, "*"),
        spa::Token(spa::TokenType::TOKEN_NAME, "t"),
      };
      std::vector<std::pair<int, std::string>> expected = { {1, "a"} };

      spa::Pattern pattern(spa::PatternType::EXACT, tokens);
      spa::PKBQueryArg lhs = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "_",
                                                               spa::DesignEntityType::VARIABLE));
      spa::QueryResult queryResult = patternStorage.getAssignVar(lhs, pattern);

      Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
      Assert::IsTrue(queryResult.getLineNumberNamePairs() == expected);

      tokens = { spa::Token(spa::TokenType::TOKEN_NAME, "v") };
      pattern = spa::Pattern(spa::PatternType::EXACT, tokens);
      queryResult = patternStorage.getAssignVar(lhs, pattern);

      Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
      Assert::IsTrue(queryResult.getLineNumberNamePairs().empty());
    }

    TEST_METHOD(TestGetAssignVarPartial) {
      spa::PatternStorage patternStorage;
      patternStorage.setAssignTable(assignTable);
      std::vector<spa::Token> tokens = { spa::Token(spa::TokenType::TOKEN_NAME, "v") };
      std::vector<std::pair<int, std::string>> expected = { {1, "a"} };

      spa::Pattern pattern(spa::PatternType::PARTIAL, tokens);
      spa::PKBQueryArg lhs = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_",
                                                               spa::DesignEntityType::VARIABLE));
      spa::QueryResult queryResult = patternStorage.getAssignVar(lhs, pattern);

      Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
      Assert::IsTrue(queryResult.getLineNumberNamePairs() == expected);

      tokens = {
        spa::Token(spa::TokenType::TOKEN_NAME, "v"),
        spa::Token(spa::TokenType::TOKEN_PLUS, "+"),
        spa::Token(spa::TokenType::TOKEN_NAME, "x"),
      };
      pattern = spa::Pattern(spa::PatternType::PARTIAL, tokens);
      queryResult = patternStorage.getAssignVar(lhs, pattern);

      Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
      Assert::IsTrue(queryResult.getLineNumberNamePairs().empty());
    }

    TEST_METHOD(TestGetAssignVarAny) {
      spa::PatternStorage patternStorage;
      patternStorage.setAssignTable(assignTable);
      std::vector<spa::Token> tokens = { spa::Token(spa::TokenType::TOKEN_NAME, "v") };
      std::vector<std::pair<int, std::string>> expected = { {1, "a"} };

      spa::Pattern pattern(spa::PatternType::ANY);
      spa::PKBQueryArg lhs = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_",
                                                               spa::DesignEntityType::VARIABLE));
      spa::QueryResult queryResult = patternStorage.getAssignVar(lhs, pattern);

      Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
      Assert::IsTrue(queryResult.getLineNumberNamePairs() == expected);
    }

    TEST_METHOD(TestGetAssignVarNameExact) {
      spa::PatternStorage patternStorage;
      patternStorage.setAssignTable(assignTable);
      std::vector<spa::Token> tokens = {
        spa::Token(spa::TokenType::TOKEN_NAME, "v"),
        spa::Token(spa::TokenType::TOKEN_PLUS, "+"),
        spa::Token(spa::TokenType::TOKEN_NAME, "x"),
        spa::Token(spa::TokenType::TOKEN_MULTIPLY, "*"),
        spa::Token(spa::TokenType::TOKEN_NAME, "y"),
        spa::Token(spa::TokenType::TOKEN_PLUS, "+"),
        spa::Token(spa::TokenType::TOKEN_NAME, "z"),
        spa::Token(spa::TokenType::TOKEN_MULTIPLY, "*"),
        spa::Token(spa::TokenType::TOKEN_NAME, "t"),
      };
      std::vector<std::pair<int, std::string>> expected = { {1, "a"} };

      spa::Pattern pattern(spa::PatternType::EXACT, tokens);
      spa::PKBQueryArg lhs = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LITERAL_STRING, "a", {}));
      spa::QueryResult queryResult = patternStorage.getAssignVarName(lhs, pattern);

      Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
      Assert::IsTrue(queryResult.getLineNumberNamePairs() == expected);

      lhs = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LITERAL_STRING, "b", {}));
      queryResult = patternStorage.getAssignVarName(lhs, pattern);

      Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
      Assert::IsTrue(queryResult.getLineNumberNamePairs().empty());

      tokens = { spa::Token(spa::TokenType::TOKEN_NAME, "v") };
      pattern = spa::Pattern(spa::PatternType::EXACT, tokens);
      lhs = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LITERAL_STRING, "a", {}));
      queryResult = patternStorage.getAssignVarName(lhs, pattern);

      Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
      Assert::IsTrue(queryResult.getLineNumberNamePairs().empty());
    }

    TEST_METHOD(TestGetAssignVarNamePartial) {
      spa::PatternStorage patternStorage;
      patternStorage.setAssignTable(assignTable);
      std::vector<spa::Token> tokens = { spa::Token(spa::TokenType::TOKEN_NAME, "v") };
      std::vector<std::pair<int, std::string>> expected = { {1, "a"} };

      spa::Pattern pattern(spa::PatternType::PARTIAL, tokens);
      spa::PKBQueryArg lhs = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LITERAL_STRING, "a", {}));
      spa::QueryResult queryResult = patternStorage.getAssignVarName(lhs, pattern);

      Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
      Assert::IsTrue(queryResult.getLineNumberNamePairs() == expected);

      lhs = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LITERAL_STRING, "b", {}));
      queryResult = patternStorage.getAssignVarName(lhs, pattern);

      Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
      Assert::IsTrue(queryResult.getLineNumberNamePairs().empty());

      tokens = {
        spa::Token(spa::TokenType::TOKEN_NAME, "v"),
        spa::Token(spa::TokenType::TOKEN_PLUS, "+"),
        spa::Token(spa::TokenType::TOKEN_NAME, "x"),
      };
      pattern = spa::Pattern(spa::PatternType::PARTIAL, tokens);
      lhs = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LITERAL_STRING, "a", {}));
      queryResult = patternStorage.getAssignVarName(lhs, pattern);

      Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
      Assert::IsTrue(queryResult.getLineNumberNamePairs().empty());
    }

    TEST_METHOD(TestGetAssignVarNameAny) {
      spa::PatternStorage patternStorage;
      patternStorage.setAssignTable(assignTable);
      std::vector<spa::Token> tokens = { spa::Token(spa::TokenType::TOKEN_NAME, "v") };
      std::vector<std::pair<int, std::string>> expected = { {1, "a"} };

      spa::Pattern pattern(spa::PatternType::ANY);
      spa::PKBQueryArg lhs = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LITERAL_STRING, "a", {}));
      spa::QueryResult queryResult = patternStorage.getAssignVarName(lhs, pattern);

      Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
      Assert::IsTrue(queryResult.getLineNumberNamePairs() == expected);

      lhs = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LITERAL_STRING, "b", {}));
      queryResult = patternStorage.getAssignVarName(lhs, pattern);

      Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
      Assert::IsTrue(queryResult.getLineNumberNamePairs().empty());
    }

    TEST_METHOD(TestGetPatternIfUnderscore) {
      spa::PatternStorage patternStorage;
      patternStorage.setPatternIfTable(patternIfTable);
      std::vector<std::pair<int, std::string>> expected = { {1, "x"}, {4, "y"}, {4, "z"}};

      spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
      spa::QueryResult queryResult = patternStorage.getPatternIfUnderscore(firstArg);

      Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
      Assert::IsTrue(queryResult.getLineNumberNamePairs() == expected);

      patternStorage.setPatternIfTable({});
      queryResult = patternStorage.getPatternIfUnderscore(firstArg);

      Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
      Assert::IsTrue(queryResult.getLineNumberNamePairs().empty());
    }

    TEST_METHOD(TestGetPatternIfVar) {
      spa::PatternStorage patternStorage;
      patternStorage.setPatternIfTable(patternIfTable);
      std::vector<std::pair<int, std::string>> expected = { {1, "x"}, {4, "y"}, {4, "z"} };

      spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "v",
                                                                    { spa::DesignEntityType::VARIABLE }));
      spa::QueryResult queryResult = patternStorage.getPatternIfUnderscore(firstArg);

      Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
      Assert::IsTrue(queryResult.getLineNumberNamePairs() == expected);

      patternStorage.setPatternIfTable({});
      queryResult = patternStorage.getPatternIfUnderscore(firstArg);

      Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
      Assert::IsTrue(queryResult.getLineNumberNamePairs().empty());
    }

    TEST_METHOD(TestGetPatternIfVarName) {
      spa::PatternStorage patternStorage;
      patternStorage.setPatternIfTable(patternIfTable);
      std::vector<std::pair<int, std::string>> expected = { {4, "y"} };

      spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LITERAL_STRING, "y", {}));
      spa::QueryResult queryResult = patternStorage.getPatternIfVarName(firstArg);

      Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
      Assert::IsTrue(queryResult.getLineNumberNamePairs() == expected);

      patternStorage.setPatternIfTable({});
      queryResult = patternStorage.getPatternIfUnderscore(firstArg);

      Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
      Assert::IsTrue(queryResult.getLineNumberNamePairs().empty());
    }

    TEST_METHOD(TestGetPatternWhileUnderscore) {
      spa::PatternStorage patternStorage;
      patternStorage.setPatternWhileTable(patternWhileTable);
      std::vector<std::pair<int, std::string>> expected = { {7, "a"}, {7, "b"}, {8, "c"} };

      spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
      spa::QueryResult queryResult = patternStorage.getPatternWhileUnderscore(firstArg);

      Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
      Assert::IsTrue(queryResult.getLineNumberNamePairs() == expected);

      patternStorage.setPatternWhileTable({});
      queryResult = patternStorage.getPatternIfUnderscore(firstArg);

      Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
      Assert::IsTrue(queryResult.getLineNumberNamePairs().empty());
    }

    TEST_METHOD(TestGetPatternWhileVar) {
      spa::PatternStorage patternStorage;
      patternStorage.setPatternWhileTable(patternWhileTable);
      std::vector<std::pair<int, std::string>> expected = { {7, "a"}, {7, "b"}, {8, "c"} };

      spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "v",
                                                                    { spa::DesignEntityType::VARIABLE }));
      spa::QueryResult queryResult = patternStorage.getPatternWhileUnderscore(firstArg);

      Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
      Assert::IsTrue(queryResult.getLineNumberNamePairs() == expected);

      patternStorage.setPatternWhileTable({});
      queryResult = patternStorage.getPatternIfUnderscore(firstArg);

      Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
      Assert::IsTrue(queryResult.getLineNumberNamePairs().empty());
    }

    TEST_METHOD(TestGetPatternWhileVarName) {
      spa::PatternStorage patternStorage;
      patternStorage.setPatternWhileTable(patternWhileTable);
      std::vector<std::pair<int, std::string>> expected = { {8, "c"} };

      spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LITERAL_STRING, "c", {}));
      spa::QueryResult queryResult = patternStorage.getPatternWhileVarName(firstArg);

      Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
      Assert::IsTrue(queryResult.getLineNumberNamePairs() == expected);

      patternStorage.setPatternWhileTable({});
      queryResult = patternStorage.getPatternIfUnderscore(firstArg);

      Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
      Assert::IsTrue(queryResult.getLineNumberNamePairs().empty());
    }
  };
}  // namespace UnitTesting
