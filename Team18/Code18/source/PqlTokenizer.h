#pragma once

#include <string>
#include <vector>

enum TokenType {
  STMT,
  READ,
  PRINT,
  WHILE,
  IF,
  ASSIGN,
  VARIABLE,
  CONSTANT,
  PROCEDURE,

  FOLLOWS,
  FOLLOWS_T,
  PARENT,
  PARENT_T,
  USES,
  MODIFIES,

  SUCH,
  THAT,
  AND,
  PATTERN,
  WITH,

  SYNONYM,
  NUMBER,
  STRING,
  UNDERSCORE,

  SEMICOLON,
  COMMA,
  DOT,
  OPEN_ROUND_BRACKET,
  CLOSED_ROUND_BRACKET,

  PROCNAME,
  VARNAME,
  VALUE,
  STATEMENT_NUM,
};

struct PqlToken {
  TokenType type;
  std::string value;

  PqlToken() : type{ TokenType::STRING }, value{ "" } {}
};

class PqlTokenizer {
 public:
  explicit PqlTokenizer(std::string query);
  std::vector<PqlToken> tokenize();

 private:
  std::string query;
};
