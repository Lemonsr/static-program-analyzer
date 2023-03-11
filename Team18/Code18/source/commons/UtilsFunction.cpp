#include "UtilsFunction.h"

#include <algorithm>
#include <stack>
#include <unordered_set>
#include <sstream>

std::unordered_set<spa::TokenType> spa::UtilsFunction::condExprToken = {
  TOKEN_BOOL_AND, TOKEN_BOOL_OR
};
std::unordered_set<spa::TokenType> spa::UtilsFunction::relExprToken = {
  TOKEN_COND_EQ, TOKEN_COND_NEQ, TOKEN_COND_LT, TOKEN_COND_LTE, TOKEN_COND_GT,
  TOKEN_COND_GTE
};
std::unordered_set<spa::TokenType> spa::UtilsFunction::exprToken = {TOKEN_PLUS, TOKEN_MINUS};
std::unordered_set<spa::TokenType> spa::UtilsFunction::termToken = {
  TOKEN_DIVIDE, TOKEN_MULTIPLY, TOKEN_MODULO
};
std::unordered_set<spa::TokenType> spa::UtilsFunction::relFactorToken = {TOKEN_NAME, TOKEN_INTEGER};

bool spa::UtilsFunction::isValidCondExprToken(Token token) {
  return condExprToken.count(token.getType());
}

bool spa::UtilsFunction::isValidRelExprToken(Token token) {
  return relExprToken.count(token.getType());
}

bool spa::UtilsFunction::isValidTermToken(Token token) {
  return termToken.count(token.getType());
}

bool spa::UtilsFunction::isValidRelFactorToken(Token token) {
  return relFactorToken.count(token.getType());
}

bool spa::UtilsFunction::isValidExprToken(Token token) {
  return exprToken.count(token.getType());
}

bool spa::UtilsFunction::isValidOpenBracket(Token token) {
  return token.getType() == TOKEN_OPEN_BRACKET;
}

bool spa::UtilsFunction::isValidCloseBracket(Token token) {
  return token.getType() == TOKEN_CLOSE_BRACKET;
}

bool spa::UtilsFunction::isValidOpenBrace(Token token) {
  return token.getType() == TOKEN_OPEN_BRACE;
}

bool spa::UtilsFunction::isValidCloseBrace(Token token) {
  return token.getType() == TOKEN_CLOSE_BRACE;
}

// factor : var_name | const_value | '(' tokens ')'
bool spa::UtilsFunction::isValidFactor(std::vector<Token> tokens) {
  if (tokens.empty()) {
    return false;
  }

  Token firstToken = tokens[0];
  if (tokens.size() == 1 && isValidRelFactorToken(firstToken)) {
    return true;
  }

  if (tokens.size() >= 3 && isValidOpenBracket(tokens.front()) && isValidCloseBracket(
    tokens.back())) {
    return isValidExpr(std::vector<Token>(tokens.begin() + 1, tokens.end() - 1));
  }

  return false;
}

// term: term '*' factor | term '/' factor | term '%' factor | factor
bool spa::UtilsFunction::isValidTerm(std::vector<Token> tokens) {
  // Find outer most ( * | / | % ) from the right
  int unclosedBracketCount = 0;
  int index = -1;
  for (int i = tokens.size() - 1; i >= 0; i--) {
    const TokenType tokenType = tokens[i].getType();
    if (tokenType == TOKEN_OPEN_BRACKET) {
      unclosedBracketCount += 1;
    } else if (tokenType == TOKEN_CLOSE_BRACKET) {
      unclosedBracketCount -= 1;
    }

    if (isValidTermToken(tokens[i]) && unclosedBracketCount == 0) {
      index = i;
      break;
    }
  }

  // ( * | / | % ) not found
  if (index == -1) {
    return isValidFactor(tokens);
  }

  std::vector<Token> left(tokens.begin(), tokens.begin() + index);
  std::vector<Token> right(tokens.begin() + index + 1, tokens.end());

  return isValidTerm(left) && isValidFactor(right);
}

// tokens: tokens '+' term | tokens '-' term | term
bool spa::UtilsFunction::isValidExpr(std::vector<spa::Token> tokens) {
  if (tokens.empty()) {
    return false;
  }

  const Token firstToken = tokens[0];
  if (tokens.size() == 1 && isValidRelFactorToken(firstToken)) {
    return true;
  }

  // Find outer most +/- from the right
  int unclosedBracketCount = 0;
  int index = -1;
  for (int i = tokens.size() - 1; i >= 0; i--) {
    const TokenType tokenType = tokens[i].getType();
    if (tokenType == TOKEN_OPEN_BRACKET) {
      unclosedBracketCount += 1;
    } else if (tokenType == TOKEN_CLOSE_BRACKET) {
      unclosedBracketCount -= 1;
    }

    if (isValidExprToken(tokens[i]) && unclosedBracketCount == 0) {
      index = i;
      break;
    }
  }

  // +/- not found
  if (index == -1) {
    return isValidTerm(tokens);
  }

  std::vector<Token> left(tokens.begin(), tokens.begin() + index);
  std::vector<Token> right(tokens.begin() + index + 1, tokens.end());

  return (isValidExpr(left) && isValidTerm(right)) || isValidTerm(tokens);
  // This means that the operator is surrounded by Parenthesis
}

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

std::vector<std::string>
spa::UtilsFunction::splitStringByDelimiter(std::string str, char delimiter) {
  std::vector<std::string> tokens;
  std::string token;
  std::stringstream ss(str);

  while (std::getline(ss, token, delimiter)) {
    tokens.push_back(token);
  }

  return tokens;
}
