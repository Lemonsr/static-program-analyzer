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
      Assert::IsTrue(queryResult.getLineNumberVariablePairs() == expected);

      tokens = { spa::Token(spa::TokenType::TOKEN_NAME, "v") };
      pattern = spa::Pattern(spa::PatternType::EXACT, tokens);
      queryResult = patternStorage.getAssignUnderscore(lhs, pattern);

      Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
      Assert::IsTrue(queryResult.getLineNumberVariablePairs().empty());
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
      Assert::IsTrue(queryResult.getLineNumberVariablePairs() == expected);

      tokens = {
        spa::Token(spa::TokenType::TOKEN_NAME, "v"),
        spa::Token(spa::TokenType::TOKEN_PLUS, "+"),
        spa::Token(spa::TokenType::TOKEN_NAME, "x"),
      };
      pattern = spa::Pattern(spa::PatternType::PARTIAL, tokens);
      queryResult = patternStorage.getAssignUnderscore(lhs, pattern);

      Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
      Assert::IsTrue(queryResult.getLineNumberVariablePairs().empty());
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
      Assert::IsTrue(queryResult.getLineNumberVariablePairs() == expected);
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
      Assert::IsTrue(queryResult.getLineNumberVariablePairs() == expected);

      tokens = { spa::Token(spa::TokenType::TOKEN_NAME, "v") };
      pattern = spa::Pattern(spa::PatternType::EXACT, tokens);
      queryResult = patternStorage.getAssignVar(lhs, pattern);

      Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
      Assert::IsTrue(queryResult.getLineNumberVariablePairs().empty());
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
      Assert::IsTrue(queryResult.getLineNumberVariablePairs() == expected);

      tokens = {
        spa::Token(spa::TokenType::TOKEN_NAME, "v"),
        spa::Token(spa::TokenType::TOKEN_PLUS, "+"),
        spa::Token(spa::TokenType::TOKEN_NAME, "x"),
      };
      pattern = spa::Pattern(spa::PatternType::PARTIAL, tokens);
      queryResult = patternStorage.getAssignVar(lhs, pattern);

      Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
      Assert::IsTrue(queryResult.getLineNumberVariablePairs().empty());
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
      Assert::IsTrue(queryResult.getLineNumberVariablePairs() == expected);
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
      spa::PKBQueryArg lhs = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::VARIABLE_NAME, "a", {}));
      spa::QueryResult queryResult = patternStorage.getAssignVarName(lhs, pattern);

      Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
      Assert::IsTrue(queryResult.getLineNumberVariablePairs() == expected);

      lhs = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::VARIABLE_NAME, "b", {}));
      queryResult = patternStorage.getAssignVarName(lhs, pattern);

      Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
      Assert::IsTrue(queryResult.getLineNumberVariablePairs().empty());

      tokens = { spa::Token(spa::TokenType::TOKEN_NAME, "v") };
      pattern = spa::Pattern(spa::PatternType::EXACT, tokens);
      lhs = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::VARIABLE_NAME, "a", {}));
      queryResult = patternStorage.getAssignVarName(lhs, pattern);

      Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
      Assert::IsTrue(queryResult.getLineNumberVariablePairs().empty());
    }

    TEST_METHOD(TestGetAssignVarNamePartial) {
      spa::PatternStorage patternStorage;
      patternStorage.setAssignTable(assignTable);
      std::vector<spa::Token> tokens = { spa::Token(spa::TokenType::TOKEN_NAME, "v") };
      std::vector<std::pair<int, std::string>> expected = { {1, "a"} };

      spa::Pattern pattern(spa::PatternType::PARTIAL, tokens);
      spa::PKBQueryArg lhs = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::VARIABLE_NAME, "a", {}));
      spa::QueryResult queryResult = patternStorage.getAssignVarName(lhs, pattern);

      Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
      Assert::IsTrue(queryResult.getLineNumberVariablePairs() == expected);

      lhs = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::VARIABLE_NAME, "b", {}));
      queryResult = patternStorage.getAssignVarName(lhs, pattern);

      Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
      Assert::IsTrue(queryResult.getLineNumberVariablePairs().empty());

      tokens = {
        spa::Token(spa::TokenType::TOKEN_NAME, "v"),
        spa::Token(spa::TokenType::TOKEN_PLUS, "+"),
        spa::Token(spa::TokenType::TOKEN_NAME, "x"),
      };
      pattern = spa::Pattern(spa::PatternType::PARTIAL, tokens);
      lhs = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::VARIABLE_NAME, "a", {}));
      queryResult = patternStorage.getAssignVarName(lhs, pattern);

      Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
      Assert::IsTrue(queryResult.getLineNumberVariablePairs().empty());
    }

    TEST_METHOD(TestGetAssignVarNameAny) {
      spa::PatternStorage patternStorage;
      patternStorage.setAssignTable(assignTable);
      std::vector<spa::Token> tokens = { spa::Token(spa::TokenType::TOKEN_NAME, "v") };
      std::vector<std::pair<int, std::string>> expected = { {1, "a"} };

      spa::Pattern pattern(spa::PatternType::ANY);
      spa::PKBQueryArg lhs = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::VARIABLE_NAME, "a", {}));
      spa::QueryResult queryResult = patternStorage.getAssignVarName(lhs, pattern);

      Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
      Assert::IsTrue(queryResult.getLineNumberVariablePairs() == expected);

      lhs = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::VARIABLE_NAME, "b", {}));
      queryResult = patternStorage.getAssignVarName(lhs, pattern);

      Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
      Assert::IsTrue(queryResult.getLineNumberVariablePairs().empty());
    }
  };
}  // namespace UnitTesting
