#include "ProcedureStatement.h"

#include <memory>
#include <string>
#include <unordered_set>

// Constructor
spa::ProcedureStatement::ProcedureStatement(spa::Token procedureVarToken,
  std::vector<std::shared_ptr<ProgramStatement>> statementLst)
  : procedureVarToken(procedureVarToken), statementLst(statementLst) {}

spa::Token spa::ProcedureStatement::getProcedureVarToken() {
  return procedureVarToken;
}

std::vector<std::shared_ptr<spa::ProgramStatement>>& spa::ProcedureStatement::getStatementLst() {
  return statementLst;
}

void spa::ProcedureStatement::addCalledVars(std::string var) {
    calledVars.emplace(var);
}

std::unordered_set<std::string>& spa::ProcedureStatement::getCalledVars() {
    return calledVars;
}
