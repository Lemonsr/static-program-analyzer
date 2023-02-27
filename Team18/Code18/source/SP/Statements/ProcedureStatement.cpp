#include <string>
#include <unordered_set>

#include "ProcedureStatement.h"

// Constructor
spa::ProcedureStatement::ProcedureStatement(spa::Token procedureVarToken,
                                            std::vector<ProgramStatement*> statementLst) :
  procedureVarToken(procedureVarToken), statementLst(statementLst) {}

spa::Token spa::ProcedureStatement::getProcedureVarToken() {
  return procedureVarToken;
}

std::vector<spa::ProgramStatement*> spa::ProcedureStatement::getStatementLst() {
  return statementLst;
}

void spa::ProcedureStatement::addCalledVars(std::string var) {
    calledVars.emplace(var);
}

std::unordered_set<std::string>& spa::ProcedureStatement::getCalledVars() {
    return calledVars;
}
