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

  // static void printOptionalVector(const std::optional<std::vector<int>>& optVec) {
  //   if (optVec.has_value()) {
  //     const std::vector<int>& vec = optVec.value();
  //     std::cout << "Vector: [";
  //     for (int i = 0; i < vec.size(); i++) {
  //       std::cout << vec[i];
  //       if (i < vec.size() - 1) {
  //         std::cout << ", ";
  //       }
  //     }
  //     std::cout << "]" << std::endl;
  //   } else {
  //     std::cout << "Vector is empty." << std::endl;
  //   }
  //   std::cout << std::flush;
  // }
};
}  // namespace spa
