#pragma once
#include <string>
#include <vector>

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
  };
}  // namespace spa
