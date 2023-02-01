#include "ProcedureStatement.h"

//Constructor
spa::ProcedureStatement::ProcedureStatement(spa::Token procedureVarToken,
                          std::vector<ProgramStatement*> statementLst): procedureVarToken(procedureVarToken), statementLst(statementLst) {}

spa::Token spa::ProcedureStatement::getProcedureVarToken() {
	return procedureVarToken;
}

std::vector<spa::ProgramStatement*> spa::ProcedureStatement::getStatementLst() {
	return statementLst;
}
