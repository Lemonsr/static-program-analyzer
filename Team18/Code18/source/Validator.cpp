#include "Tokenizer.h"
#include "Validator.h";
#include "Stream.h";
#include "PKB.h";
#include <unordered_set>;

std::unordered_set<spa::TokenType> stmtToken = {
    spa::TokenType::TOKEN_READ, spa::TokenType::TOKEN_CALL, spa::TokenType::TOKEN_PRINT,
    spa::TokenType::TOKEN_WHILE, spa::TokenType::TOKEN_IF,
    spa::TokenType::TOKEN_THEN, spa::TokenType::TOKEN_ELSE
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

bool spa::Validator::isValidStmtToken(spa::Token token) {
    if (stmtToken.count(token.getType())) {
        return true;
    }
    return false;
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


spa::Validator::Validator(spa::Stream<spa::Token> t) : tokens(t) {}

bool spa::Validator::validateProgram() {
    // Checks the program
    for (int64_t i = 0; i < tokens.remaining(); i++) {
        // tokens[i].getType() == TokenType[TOKEN_MULTIPLY]
        std::cout << "Type: " << tokens[i].getType() << ", Value: " << tokens[i].getValue() <<
            std::endl;
    }
    return true;
}
