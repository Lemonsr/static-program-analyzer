#include "Tokenizer.h"
#include "Token.h"
#include "Stream.h"
#include "Literal.h"

#include <iostream>
#include <istream>
#include <string>
#include <stdexcept>
#include <cctype>
#include <unordered_map>

const std::string INVALID_NAME_ERROR = "Invalid name in SIMPLE code";
const std::string LEADING_ZERO_ERROR = "Leading Zero in Integer";
const std::string UNKNOWN_SYMBOL_ERROR = "Unknown Symbol: ";

const std::unordered_map<std::string, spa::TokenType> tokenTypes {
  {OPEN_BRACE_LITERAL, spa::TOKEN_OPEN_BRACE},
  {CLOSE_BRACE_LITERAL, spa::TOKEN_CLOSE_BRACE},
  {OPEN_BRACKET_LITERAL, spa::TOKEN_OPEN_BRACKET},
  {CLOSE_BRACKET_LITERAL, spa::TOKEN_CLOSE_BRACKET},
  {SEMICOLON_LITERAL, spa::TOKEN_SEMICOLON},
  {PLUS_LITERAL, spa::TOKEN_PLUS},
  {MINUS_LITERAL, spa::TOKEN_MINUS},
  {DIVIDE_LITERAL, spa::TOKEN_DIVIDE},
  {MULTIPLY_LITERAL, spa::TOKEN_MULTIPLY},
  {MODULO_LITERAL, spa::TOKEN_MODULO},
  {EQUAL_LITERAL, spa::TOKEN_EQUAL},
  {COND_EQ_LITERAL, spa::TOKEN_COND_EQ},
  {COND_NEQ_LITERAL, spa::TOKEN_COND_NEQ},
  {COND_LT_LITERAL, spa::TOKEN_COND_LT},
  {COND_LTE_LITERAL, spa::TOKEN_COND_LTE},
  {COND_GT_LITERAL, spa::TOKEN_COND_GT},
  {COND_GTE_LITERAL, spa::TOKEN_COND_GTE},
  {BOOL_NOT_LITERAL, spa::TOKEN_BOOL_NOT},
  {BOOL_AND_LITERAL, spa::TOKEN_BOOL_AND},
  {BOOL_OR_LITERAL, spa::TOKEN_BOOL_OR},
  {COMMA_LITERAL, spa::TOKEN_COMMA},
  {UNDERSCORE_LITERAL, spa::TOKEN_UNDERSCORE},
  {DOUBLE_QUOTES_LITERAL, spa::TOKEN_DOUBLE_QUOTES},
  {FULL_STOP_LITERAL, spa::TOKEN_FULL_STOP},
  {HASH_LITERAL, spa::TOKEN_HASH}
};

void spa::Tokenizer::pushWordToken(spa::Stream<spa::Token>& tokens,
                                   std::string& word, int index) {
  if (word.empty()) {
    return;
  }
  if (std::isalpha(word[0])) {
    tokens.pushBack({spa::TOKEN_NAME, word, index - static_cast<int>(word.size())});
  } else {
    for (char c : word) {
      if (!std::isdigit(c)) {
        throw std::runtime_error(INVALID_NAME_ERROR);
      }
    }
    if (word.size() > 1 && word[0] == ZERO_LITERAL) {
      throw std::runtime_error(LEADING_ZERO_ERROR);
    }
    tokens.pushBack({spa::TOKEN_INTEGER, word, index - static_cast<int>(word.size())});
  }
  word.clear();
}

void spa::Tokenizer::pushSymbolToken(std::istream& srcStream,
                                     spa::Stream<spa::Token>& tokens,
                                     char c, int& index) {
  std::string s(1, c);
  int next = srcStream.peek();
  if (next != EOF) {
    s.push_back(static_cast<char>(next));
    auto it = tokenTypes.find(s);
    if (it != tokenTypes.end()) {
      tokens.pushBack({it->second, it->first, index});
      srcStream.get();
      ++index;
      return;
    }
    s.pop_back();
  }
  auto it = tokenTypes.find(s);
  if (it == tokenTypes.end()) {
    throw std::runtime_error(std::string(UNKNOWN_SYMBOL_ERROR).append(s));
  }
  tokens.pushBack({it->second, it->first, index});
}

spa::Stream<spa::Token> spa::Tokenizer::tokenize(std::istream& srcStream) {
  std::string word;
  Stream<Token> tokens;
  char c;
  int index = 0;
  while (!srcStream.get(c).eof()) {
    if (std::isspace(c)) {
      pushWordToken(tokens, word, index);
    } else if (std::isalnum(c)) {
      word.push_back(c);
    } else {
      pushWordToken(tokens, word, index);
      pushSymbolToken(srcStream, tokens, c, index);
    }
    ++index;
  }
  pushWordToken(tokens, word, index);
  return tokens;
}
