#pragma once
#include <vector>

#include "ProgramStatement.h"
#include "Token.h"

namespace spa {
class ProcedureStatement {
 private:
  spa::Token token;
  std::vector<ProgramStatement*> statementLst;

 public:
  ProcedureStatement(spa::Token token, std::vector<ProgramStatement*> statementLst);
  spa::Token getToken();
  std::vector<ProgramStatement*> getStatementLst();
};
}  // namespace spa
