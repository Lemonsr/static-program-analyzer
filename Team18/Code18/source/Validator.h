#pragma once
#include "Tokenizer.h"
#include "Token.h"
#include "Stream.h"
#include "PKB.h"

namespace spa {
class Validator {
 public:
        spa::Stream<spa::Token> tokens;
        int offset = 0;
        explicit Validator(spa::Stream<spa::Token> tokens);
        bool validateProgram();
        bool setStream(spa::Stream<spa::Token> stream);
        bool parseStream(spa::Stream<spa::Token> stream);
        spa::Token& get();
        spa::Token& next(int offset);
        spa::Token& next();

 private:
        static bool isValidRelExprToken(spa::Token token);
        static bool isValidStmtToken(spa::Token token);
        static bool isValidCondExprToken(spa::Token token);
        static bool isValidTermToken(spa::Token token);
        static bool isValidProgram(spa::Stream<spa::Token> tokens);
        static bool isValidGrammar();
        static bool isValidProcedure();
        static bool isValidCondExpr();
        static bool isValidAssign();
        static bool isValidStmt();
        static bool isValidStmtList();
        static bool isValidReadPrint();
        static bool isValidWhile();
        static bool isValidif();
};
}  // namespace spa
