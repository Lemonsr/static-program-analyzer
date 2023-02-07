#pragma once
#include "Token.h"
#include "Stream.h"
#include <unordered_set>
#include <string>
#include <vector>

namespace spa {
class SpValidator {
 public:
        explicit SpValidator(Stream<Token> tokens);
        bool validateGrammar();
 private:
        std::unordered_set<TokenType> condExprToken = {TOKEN_BOOL_AND, TOKEN_BOOL_OR};
        std::unordered_set<TokenType> relExprToken = {
            TOKEN_COND_EQ, TOKEN_COND_NEQ, TOKEN_COND_LT, TOKEN_COND_LTE, TOKEN_COND_GT,
            TOKEN_COND_GTE
        };
        std::unordered_set<TokenType> exprToken = {TOKEN_PLUS, TOKEN_MINUS};
        std::unordered_set<TokenType> termToken = {TOKEN_DIVIDE, TOKEN_MULTIPLY, TOKEN_MODULO};
        std::unordered_set<TokenType> nameToken = {
            TOKEN_NAME, TOKEN_IF, TOKEN_WHILE,
            TOKEN_PRINT, TOKEN_READ
        };
        std::unordered_set<TokenType> relFactorToken = {TOKEN_NAME, TOKEN_INTEGER};

        int64_t idx = 0;  // Track the current token position
        Stream<Token> tokens;
        std::unordered_set<std::string> procNames;
        bool hasRemaining();
        bool hasRemaining(int64_t);
        void next(int64_t offset = 1);
        Token getToken();
        Token peekNextToken(int64_t offset = 0);

        bool isExpr(std::vector<Token>);
        bool isTerm(std::vector<Token>);
        bool isFactor(std::vector<Token>);
        bool isCondExpr(std::vector<Token>);
        bool isRelExpr(std::vector<Token>);
        bool isRelFactor(std::vector<Token>);
        static bool isValidOpenBrace(Token token);
        static bool isValidCloseBrace(Token token);
        static bool isValidOpenBracket(Token token);
        static bool isValidCloseBracket(Token token);

        bool isValidCondExprToken(Token) const;
        bool isValidRelExprToken(Token) const;
        bool isValidTermToken(Token) const;
        bool isValidRelFactorToken(Token) const;
        bool isValidExprToken(Token) const;

        void validateProcedure();
        void validateStmtLst();
        void validateStmt();

        void validateEqual();
        void validateReadPrint();
        void validateWhileIf();
        void validateWhile();
        void validateIf();
        void validateBracketCondExpr();
        void validateBraceCondExpr();
        void validateBraceStmtLst();
        void validateCondExpr();
        void validateRelExpr();
        void validateBracketExpr();
        void validateFactor();
};
}  // namespace spa
