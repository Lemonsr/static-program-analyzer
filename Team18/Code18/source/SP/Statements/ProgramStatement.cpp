#include "ProgramStatement.h"

int spa::ProgramStatement::getStatementLineNum() {
  return statementLineNum;
}

const std::string spa::ProgramStatement::getParentProcedureVal() {
  return parentProcedureVal;
}

std::pair<spa::CFGNode*, spa::CFGNode*> spa::ProgramStatement::processStatement(PKBManager& pkbManager) {
  return {new CFGNode(-1), new CFGNode(-1)};
}
