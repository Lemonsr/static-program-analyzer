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
        spa::Stream<spa::Token> getUpdatedStream();

 private:
        int64_t idx = 0;  // Track the current token position
        Stream<Token> tokens;
        std::unordered_set<std::string> procNames;
        bool hasRemaining();
        bool hasRemaining(int64_t);
        void next(int64_t offset = 1);
        Token getToken();
        Token peekNextToken(int64_t offset = 0);

        bool isCondExpr(std::vector<Token>);
        bool isRelExpr(std::vector<Token>);
        bool isRelFactor(std::vector<Token>);

        void validateProcedure();
        void validateStmtLst();
        void validateStmt();

        void validateEqual();
        void validateReadPrint();
        void validateWhileIf();
        void validateWhile();
        void validateIf();
        void validateCondExpr();
};
}  // namespace spa
