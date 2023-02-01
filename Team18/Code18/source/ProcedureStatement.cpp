#include "ProcedureStatement.h"

//Constructor
spa::ProcedureStatement::ProcedureStatement(spa::Token token,
                          std::vector<ProgramStatement*> statementLst): token(token), statementLst(statementLst) {}

spa::Token spa::ProcedureStatement::getToken() {
	return token;
}

std::vector<spa::ProgramStatement*> spa::ProcedureStatement::getStatementLst() {
	return statementLst;
}
