#include "UtilsFunction.h"

#include <algorithm>
#include <stack>
#include <unordered_set>

int spa::UtilsFunction::getPrecedence(std::string op) {
  if (op == "!") return 6;
  if (op == "*" || op == "/" || op == "%") return 5;
  if (op == "+" || op == "-") return 4;
  if (op == "==" || op == "!=") return 3;
  if (op == ">" || op == "<" || op == ">=" || op == "<=") return 2;
  if (op == "&&") return 1;
  if (op == "||") return 0;
  return -1;
}

void spa::UtilsFunction::ltrim(std::string& s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(),
    [](unsigned char ch) {
      return !std::isspace(ch);
    }));
}

void spa::UtilsFunction::rtrim(std::string& s) {
  s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
    return !std::isspace(ch);
  }).base(), s.end());
}

void spa::UtilsFunction::trimString(std::string& s) {
  rtrim(s);
  ltrim(s);
}

std::string spa::UtilsFunction::infixToPostfix(std::vector<spa::Token> tokens) {
  std::unordered_set<std::string> operators = {
    "!", "&&", "||", "*", "/", "%", "+", "-", "<", ">", "<=", ">=", "==", "!="
  };
  std::stack<std::string> operatorStack;
  std::string postfix = "";

  for (spa::Token token : tokens) {
    std::string tokenValue = token.getValue();
    if (std::all_of(tokenValue.begin(), tokenValue.end(), ::isalnum)) {
      postfix += tokenValue + " ";
    } else if (operators.count(tokenValue)) {
      while (!operatorStack.empty() && operatorStack.top() != "(" &&
        getPrecedence(operatorStack.top()) >= getPrecedence(tokenValue)) {
        postfix += operatorStack.top() + " ";
        operatorStack.pop();
      }
      operatorStack.push(tokenValue);
    } else if (tokenValue == "(") {
      operatorStack.push(tokenValue);
    } else if (tokenValue == ")") {
      while (!operatorStack.empty() && operatorStack.top() != "(") {
        postfix += operatorStack.top() + " ";
        operatorStack.pop();
      }
      operatorStack.pop();
    }
  }

  while (!operatorStack.empty()) {
    postfix += operatorStack.top() + " ";
    operatorStack.pop();
  }
  trimString(postfix);
  return postfix;
}
