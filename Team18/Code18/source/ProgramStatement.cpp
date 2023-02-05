#include "ProgramStatement.h"

int spa::ProgramStatement::getStatementLineNum() {
  return statementLineNum;
}

const std::string spa::ProgramStatement::getParentProcedureVal() {
  return parentProcedureVal;
}
