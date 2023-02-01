#include "ContainerStatement.h"

std::vector<spa::ProgramStatement*> spa::ContainerStatement::getStatementList() {
	return statementList;
}

// Constructor for IfContainerStatement
spa::IfContainerStatement::IfContainerStatement(std::string parentProcedureVal, int statementLineNum,
                                                std::vector<ProgramStatement*> statementList) {
	this->parentProcedureVal = parentProcedureVal;
	this->statementLineNum = statementLineNum;
	this->statementList = statementList;
}

// Constructor for IfContainerStatement
spa::WhileContainerStatement::WhileContainerStatement(std::string parentProcedureVal, int statementLineNum,
                                                      std::vector<ProgramStatement*> statementList) {
	this->parentProcedureVal = parentProcedureVal;
	this->statementLineNum = statementLineNum;
	this->statementList = statementList;
}

// Constructor for IfContainerStatement
spa::InnerBlockStatement::InnerBlockStatement(std::string parentProcedureVal,
                                              std::vector<ProgramStatement*> statementList) {
	this->parentProcedureVal = parentProcedureVal;
	this->statementList = statementList;
}

void spa::IfContainerStatement::processStatement(PKB& pkb) {
  ProgramStatement* ifConditionStatement = statementList[0];
	ProgramStatement* thenStatement = statementList[1];
	ProgramStatement* elseStatement = statementList[2];
	ifConditionStatement->processStatement(pkb);
	thenStatement->processStatement(pkb);
	elseStatement->processStatement(pkb);
}

void spa::WhileContainerStatement::processStatement(PKB& pkb) {
	ProgramStatement* whileConditionStatement = statementList[0];
	ProgramStatement* innerWhileBlockStatements = statementList[1];
	whileConditionStatement->processStatement(pkb);
	innerWhileBlockStatements->processStatement(pkb);
}

void spa::InnerBlockStatement::processStatement(PKB& pkb) {
  for (auto statement: statementList) {
    statement->processStatement(pkb);
  }
}



