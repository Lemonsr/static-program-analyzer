#pragma once
#include "Tokenizer.h"
#include "Token.h"
#include "Stream.h"
#include "PKB.h"
#include <string>
#include <unordered_map>

namespace spa {
class SP {
 public:
    explicit SP(std::string);
    void processSource();
 private:
    std::string sourceCode;
    Stream<Token> convertToken();
    const std::unordered_map<std::string, TokenType> stmtTokensMap{
    {"procedure", spa::TOKEN_PROCEDURE},
    {"read", spa::TOKEN_READ},
    {"call", spa::TOKEN_CALL},
    {"print", spa::TOKEN_PRINT},
    {"while", spa::TOKEN_WHILE},
    {"if", spa::TOKEN_IF},
    {"then", spa::TOKEN_THEN},
    {"else", spa::TOKEN_ELSE},
    };
};
}  // namespace spa
