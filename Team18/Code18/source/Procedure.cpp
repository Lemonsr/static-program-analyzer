#include "Procedure.h"

//Constructor
Procedure::Procedure(spa::Token token,
                     std::vector<Statement*> statementLst): token(token), statementLst(statementLst) {}

spa::Token Procedure::getToken() {
	return token;
}

std::vector<Statement*> Procedure::getStatementLst() {
	return statementLst;
}
