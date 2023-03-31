#pragma once
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include "ProgramStatement.h"
#include "Token.h"

namespace spa {
class ProcedureStatement {
 private:
  Token procedureVarToken;
  std::vector<std::shared_ptr<ProgramStatement>> statementLst;
  std::unordered_set<std::string> calledVars;

 public:
  ProcedureStatement(Token procedureVarToken, std::vector<std::shared_ptr<ProgramStatement>> statementLst);
  Token getProcedureVarToken();
  std::vector<std::shared_ptr<ProgramStatement>>& getStatementLst();
  void addCalledVars(std::string);
  std::unordered_set<std::string>& getCalledVars();
  std::string getProcTokenValue();
};
}  // namespace spa
