#include "Statement.h"

int spa::Statement::getStatementLineNum() {
	return statementLineNum;
}

const std::string spa::Statement::getParentProcedureVal() {
	return parentProcedureVal;
}
