#include "NonContainerStatement.h"

std::string OneVarNonContainerStatement::getVariableName()
{
	return variableName;
}

//Constructor for ReadStatement
ReadStatement::ReadStatement(std::string parentProcedureVal, std::string variableName, std::unordered_set<int>whileStmtAncestors, std::unordered_set<int> ifStmtAncestors, int statementLineNum) {
	this->parentProcedureVal = parentProcedureVal;
	this->variableName = variableName;
	this->whileStmtAncestors = whileStmtAncestors;
	this->ifStmtAncestors = ifStmtAncestors;
	this->statementLineNum = statementLineNum;
};

//Constructor for PrintStatement
PrintStatement::PrintStatement(std::string parentProcedureVal, std::string variableName, std::unordered_set<int>whileStmtAncestors, std::unordered_set<int> ifStmtAncestors, int statementLineNum) {
	this->parentProcedureVal = parentProcedureVal;
	this->variableName = variableName;
	this->whileStmtAncestors = whileStmtAncestors;
	this->ifStmtAncestors = ifStmtAncestors;
	this->statementLineNum = statementLineNum;
};

//Constructor for CallStatement
CallStatement::CallStatement(std::string parentProcedureVal, std::string variableName, std::unordered_set<int>whileStmtAncestors, std::unordered_set<int> ifStmtAncestors, int statementLineNum) {
	this->parentProcedureVal = parentProcedureVal;
	this->variableName = variableName;
	this->whileStmtAncestors = whileStmtAncestors;
	this->ifStmtAncestors = ifStmtAncestors;
	this->statementLineNum = statementLineNum;
};

//Constructor for AssignStatement
AssignStatement::AssignStatement(std::string parentProcedureVal, Expr* expr, std::unordered_set<int>whileStmtAncestors, std::unordered_set<int> ifStmtAncestors, int statementLineNum) {
	this->parentProcedureVal = parentProcedureVal;
	this->expr = expr;
	this->whileStmtAncestors = whileStmtAncestors;
	this->ifStmtAncestors = ifStmtAncestors;
	this->statementLineNum = statementLineNum;
};

//Constructor for ifConditionStatement
ifConditionStatement::ifConditionStatement(std::string parentProcedureVal, Expr* expr, std::unordered_set<int>whileStmtAncestors, std::unordered_set<int> ifStmtAncestors, int statementLineNum) {
	this->parentProcedureVal = parentProcedureVal;
	this->expr = expr;
	this->whileStmtAncestors = whileStmtAncestors;
	this->ifStmtAncestors = ifStmtAncestors;
	this->statementLineNum = statementLineNum;
};

//Constructor for whileConditionStatement
whileConditionStatement::whileConditionStatement(std::string parentProcedureVal, Expr* expr, std::unordered_set<int>whileStmtAncestors, std::unordered_set<int> ifStmtAncestors, int statementLineNum) {
	this->parentProcedureVal = parentProcedureVal;
	this->expr = expr;
	this->whileStmtAncestors = whileStmtAncestors;
	this->ifStmtAncestors = ifStmtAncestors;
	this->statementLineNum = statementLineNum;
};