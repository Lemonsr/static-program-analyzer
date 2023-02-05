#pragma once
#include <vector>

#include "ProgramStatement.h"
#include "Token.h"

namespace spa {
class ProcedureStatement {
 private:
  Token procedureVarToken;
  std::vector<ProgramStatement*> statementLst;

 public:
  ProcedureStatement(Token procedureVarToken, std::vector<ProgramStatement*> statementLst);
  Token getProcedureVarToken();
  std::vector<ProgramStatement*> getStatementLst();
};
}  // namespace spa
