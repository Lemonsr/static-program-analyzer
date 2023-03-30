#include "UtilsFunction.h"

#include <algorithm>
#include <stack>
#include <unordered_set>
#include <sstream>

#include "Literal.h"

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

bool spa::UtilsFunction::isSameSynonym(PqlArgument& first, PqlArgument& second) {
  return first.getType() == SYNONYM && second.getType() == SYNONYM && first.getValue() == second.getValue();
}

int spa::UtilsFunction::getPrecedence(std::string op) {
  if (op == BOOL_NOT_LITERAL) return 6;
  if (op == MULTIPLY_LITERAL || op == DIVIDE_LITERAL|| op == MODULO_LITERAL) return 5;
  if (op == PLUS_LITERAL || op == MINUS_LITERAL) return 4;
  if (op == COND_EQ_LITERAL || op == COND_NEQ_LITERAL) return 3;
  if (op == COND_GT_LITERAL || op == COND_LT_LITERAL || op == COND_GTE_LITERAL || op == COND_LTE_LITERAL) return 2;
  if (op == BOOL_AND_LITERAL) return 1;
  if (op == BOOL_OR_LITERAL) return 0;
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
    BOOL_NOT_LITERAL, BOOL_AND_LITERAL, BOOL_OR_LITERAL, MULTIPLY_LITERAL, DIVIDE_LITERAL, MODULO_LITERAL,
    PLUS_LITERAL, MINUS_LITERAL, COND_LT_LITERAL, COND_GT_LITERAL, COND_LTE_LITERAL, COND_GTE_LITERAL,
    COND_EQ_LITERAL, COND_NEQ_LITERAL
  };
  std::stack<std::string> operatorStack;
  std::string postfix = EMPTY_LITERAL;

  for (spa::Token token : tokens) {
    std::string tokenValue = token.getValue();
    if (std::all_of(tokenValue.begin(), tokenValue.end(), ::isalnum)) {
      postfix += tokenValue + SPACE_LITERAL;
    } else if (operators.count(tokenValue)) {
      while (!operatorStack.empty() && operatorStack.top() != OPEN_BRACKET_LITERAL &&
        getPrecedence(operatorStack.top()) >= getPrecedence(tokenValue)) {
        postfix += operatorStack.top() + SPACE_LITERAL;
        operatorStack.pop();
      }
      operatorStack.push(tokenValue);
    } else if (tokenValue == OPEN_BRACKET_LITERAL) {
      operatorStack.push(tokenValue);
    } else if (tokenValue == CLOSE_BRACKET_LITERAL) {
      while (!operatorStack.empty() && operatorStack.top() != OPEN_BRACKET_LITERAL) {
        postfix += operatorStack.top() + SPACE_LITERAL;
        operatorStack.pop();
      }
      operatorStack.pop();
    }
  }

  while (!operatorStack.empty()) {
    postfix += operatorStack.top() + SPACE_LITERAL;
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
