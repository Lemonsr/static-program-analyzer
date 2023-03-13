#include "ProgramStatement.h"
#include <utility>

int spa::ProgramStatement::getStatementLineNum() {
  return statementLineNum;
}

const std::string spa::ProgramStatement::getParentProcedureVal() {
  return parentProcedureVal;
}

std::pair<spa::CFGNode, spa::CFGNode> spa::ProgramStatement::processStatement(PKBManager& pkbManager) {
  return {CFGNode(), CFGNode()};
}
