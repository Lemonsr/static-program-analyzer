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
