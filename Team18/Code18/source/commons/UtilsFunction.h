#pragma once
#include <string>
#include <utility>
#include <vector>
#include <optional>
#include <unordered_set>

#include "Token.h"

namespace spa {
class UtilsFunction {
 private:
  static int getPrecedence(std::string op);

 public:
  static void ltrim(std::string& s);
  static void rtrim(std::string& s);
  static void trimString(std::string& s);
  static std::string infixToPostfix(std::vector<spa::Token> tokens);
  static std::vector<std::string> splitStringByDelimiter(std::string str, char delimiter);

  static std::unordered_set<TokenType> condExprToken;
  static std::unordered_set<TokenType> relExprToken;
  static std::unordered_set<TokenType> exprToken;
  static std::unordered_set<TokenType> termToken;
  static std::unordered_set<TokenType> relFactorToken;

  static bool isValidCondExprToken(Token token);
  static bool isValidRelExprToken(Token token);
  static bool isValidTermToken(Token token);
  static bool isValidRelFactorToken(Token token);
  static bool isValidExprToken(Token token);
  static bool isValidOpenBracket(Token token);
  static bool isValidCloseBracket(Token token);
  static bool isValidOpenBrace(Token token);
  static bool isValidCloseBrace(Token token);

  static bool isValidFactor(std::vector<Token> tokens);
  static bool isValidTerm(std::vector<Token> tokens);
  static bool isValidExpr(std::vector<Token> tokens);

  template <typename T>
  static bool isOptionalVectorEqual(std::optional<std::vector<T>>& expected,
                                    std::optional<std::vector<T>>& actual) {
    std::unordered_set<T> expectedSet{};
    std::unordered_set<T> actualSet{};

    if (!expected && !actual) {
      return true;
    }
    if (expected && actual) {
      const auto& vectorExpected = *expected;
      const auto& vectorActual = *actual;
      if (vectorExpected.size() != vectorActual.size()) {
        return false;
      }
      for (std::size_t i = 0; i < vectorExpected.size(); ++i) {
        expectedSet.insert(vectorExpected[i]);
        actualSet.insert(vectorActual[i]);
      }
      return expectedSet == actualSet;
    }
    return false;
  }

  static bool isOptionalVectorEqual(
    std::optional<std::vector<std::pair<int, std::string>>>& expected,
    std::optional<std::vector<std::pair<int, std::string>>>& actual) {
    std::vector<std::pair<int, std::string>> expectedSet{};
    std::vector<std::pair<int, std::string>> actualSet{};

    if (!expected && !actual) {
      return true;
    }
    if (expected && actual) {
      const auto& vectorExpected = *expected;
      const auto& vectorActual = *actual;
      if (vectorExpected.size() != vectorActual.size()) {
        return false;
      }
      for (std::size_t i = 0; i < vectorExpected.size(); ++i) {
        expectedSet.push_back(vectorExpected[i]);
        actualSet.push_back(vectorActual[i]);
      }
      return expectedSet == actualSet;
    }
    return false;
  }
};
}  // namespace spa
