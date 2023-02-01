#pragma once
#include <vector>

#include "ProgramStatement.h"
#include "Token.h"

namespace spa {
  class ProcedureStatement {
  private:
    Token token;
    std::vector<ProgramStatement*> statementLst;

  public:
    ProcedureStatement(Token token, std::vector<ProgramStatement*> statementLst);
    Token getToken();
    std::vector<ProgramStatement*> getStatementLst();
  };
}  // namespace spa
