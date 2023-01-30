#include "ContainerStatement.h"

std::vector<Statement*> ContainerStatement::getStatementList() {
	return statementList;
}

//Constructor for IfContainerStatement
IfContainerStatement::IfContainerStatement(std::string parentProcedureVal, int statementLineNum,
                                           std::vector<Statement*> statementList) {
	this->parentProcedureVal = parentProcedureVal;
	this->statementLineNum = statementLineNum;
	this->statementList = statementList;
}

//Constructor for IfContainerStatement
WhileContainerStatement::WhileContainerStatement(std::string parentProcedureVal, int statementLineNum,
                                                 std::vector<Statement*> statementList) {
	this->parentProcedureVal = parentProcedureVal;
	this->statementLineNum = statementLineNum;
	this->statementList = statementList;
}

//Constructor for IfContainerStatement
InnerBlockStatement::InnerBlockStatement(std::string parentProcedureVal,
                                         std::vector<Statement*> statementList) {
	this->parentProcedureVal = parentProcedureVal;
	this->statementList = statementList;
}
