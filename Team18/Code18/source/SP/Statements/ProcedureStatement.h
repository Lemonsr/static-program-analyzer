#pragma once
#include <vector>
#include <string>
#include <unordered_set>

#include "ProgramStatement.h"
#include "Token.h"

namespace spa {
class ProcedureStatement {
 private:
  Token procedureVarToken;
  std::vector<ProgramStatement*> statementLst;
  std::unordered_set<std::string> calledVars;

 public:
  ProcedureStatement(Token procedureVarToken, std::vector<ProgramStatement*> statementLst);
  Token getProcedureVarToken();
  std::vector<ProgramStatement*> getStatementLst();
  void addCalledVars(std::string);
  std::unordered_set<std::string>& getCalledVars();
};
}  // namespace spa
