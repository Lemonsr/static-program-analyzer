#include "Tokenizer.h"
#include "Validator.h";

#include <iostream>

#include "Stream.h";
#include "PKB.h";
#include <string>
#include <unordered_map>;
#include <unordered_set>;

const std::unordered_map<std::string, spa::TokenType> stmtToken{
    {"procedure", spa::TOKEN_PROCEDURE},
    {"read", spa::TOKEN_READ},
    {"call", spa::TOKEN_CALL},
    {"print", spa::TOKEN_PRINT},
    {"while", spa::TOKEN_WHILE},
    {"if", spa::TOKEN_IF},
    {"then", spa::TOKEN_THEN},
    {"else", spa::TOKEN_ELSE},
};

std::unordered_set<spa::TokenType> condExprToken = {
    spa::TokenType::TOKEN_BOOL_NOT, spa::TokenType::TOKEN_BOOL_AND, spa::TokenType::TOKEN_BOOL_OR
};
std::unordered_set<spa::TokenType> relExprToken = {
    spa::TokenType::TOKEN_COND_EQ, spa::TokenType::TOKEN_COND_NEQ, spa::TokenType::TOKEN_COND_LT,
    spa::TokenType::TOKEN_COND_LTE, spa::TokenType::TOKEN_COND_GT, spa::TokenType::TOKEN_COND_GTE
};
std::unordered_set<spa::TokenType> exprToken = {
    spa::TokenType::TOKEN_PLUS, spa::TokenType::TOKEN_MINUS
};
std::unordered_set<spa::TokenType> termToken = {
    spa::TokenType::TOKEN_DIVIDE, spa::TokenType::TOKEN_MULTIPLY, spa::TokenType::TOKEN_MODULO
};

spa::Validator::Validator(spa::Stream<spa::Token> t) : tokens(t) {}


spa::Stream<spa::Token> spa::Validator::convertToken(spa::Stream<spa::Token>& tokens) {
    for (int64_t i = 0; i < tokens.remaining(); i++) {
        if (tokens[i].getType() == spa::TOKEN_NAME &&
            stmtToken.count(tokens[i].getValue())) {
            tokens[i] =  Token(stmtToken.at(tokens[i].getValue()), tokens[i].getValue());
            }
        }
    return tokens;
    }


bool spa::Validator::isValidCondExprToken(spa::Token token) {
    if (condExprToken.count(token.getType())) {
        return true;
    }
    return false;
}

bool spa::Validator::isValidRelExprToken(spa::Token token) {
    if (relExprToken.count(token.getType())) {
        return true;
    }
    return false;
}

bool spa::Validator::isValidTermToken(spa::Token token) {
    if (termToken.count(token.getType())) {
        return true;
    }
    return false;
}


bool spa::Validator::validateProgram() {
    // Checks the program
    for (int64_t i = 0; i < tokens.remaining(); i++) {
        // tokens[i].getType() == TokenType[TOKEN_MULTIPLY]
        std::cout << "Type: " << tokens[i].getType() << ", Value: " << tokens[i].getValue() <<
            std::endl;
    }
    return true;
}