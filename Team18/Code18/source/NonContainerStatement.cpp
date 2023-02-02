#include "NonContainerStatement.h"

#include <algorithm>

std::string spa::OneVarNonContainerStatement::getVariableName() {
	return variableName;
}

// Constructor for ReadStatement
spa::ReadStatement::ReadStatement(std::string parentProcedureVal, std::string variableName,
                                  std::unordered_set<int> whileStmtParents, std::unordered_set<int> ifStmtParents,
                                  int statementLineNum) {
	this->parentProcedureVal = parentProcedureVal;
	this->variableName = variableName;
	this->whileStmtParents = whileStmtParents;
	this->ifStmtParents = ifStmtParents;
	this->statementLineNum = statementLineNum;
};

// Constructor for PrintStatement
spa::PrintStatement::PrintStatement(std::string parentProcedureVal, std::string variableName,
                                    std::unordered_set<int> whileStmtParents, std::unordered_set<int> ifStmtParents,
                                    int statementLineNum) {
	this->parentProcedureVal = parentProcedureVal;
	this->variableName = variableName;
	this->whileStmtParents = whileStmtParents;
	this->ifStmtParents = ifStmtParents;
	this->statementLineNum = statementLineNum;
};

// Constructor for CallStatement
spa::CallStatement::CallStatement(std::string parentProcedureVal, std::string variableName,
                                  std::unordered_set<int> whileStmtParents, std::unordered_set<int> ifStmtParents,
                                  int statementLineNum) {
	this->parentProcedureVal = parentProcedureVal;
	this->variableName = variableName;
	this->whileStmtParents = whileStmtParents;
	this->ifStmtParents = ifStmtParents;
	this->statementLineNum = statementLineNum;
};

// Constructor for ifConditionStatement
spa::AssignStatement::AssignStatement(std::string parentProcedureVal, std::string assignVar, std::string postfixExpr,
                                      std::unordered_set<int> whileStmtParents,
                                      std::unordered_set<int> ifStmtParents, int statementLineNum) {
	this->parentProcedureVal = parentProcedureVal;
	this->assignVar = assignVar;
	this->postfixExpr = postfixExpr;
	this->whileStmtParents = whileStmtParents;
	this->ifStmtParents = ifStmtParents;
	this->statementLineNum = statementLineNum;
};

// Constructor for AssignStatement
spa::IfConditionStatement::IfConditionStatement(std::string parentProcedureVal, std::string postfixExpr,
                                                std::unordered_set<int> whileStmtParents,
                                                std::unordered_set<int> ifStmtParents, int statementLineNum) {
	this->parentProcedureVal = parentProcedureVal;
	this->postfixExpr = postfixExpr;
	this->whileStmtParents = whileStmtParents;
	this->ifStmtParents = ifStmtParents;
	this->statementLineNum = statementLineNum;
};


// Constructor for whileConditionStatement
spa::WhileConditionStatement::WhileConditionStatement(std::string parentProcedureVal, std::string postfixExpr,
                                                      std::unordered_set<int> whileStmtParents,
                                                      std::unordered_set<int> ifStmtParents, int statementLineNum) {
	this->parentProcedureVal = parentProcedureVal;
	this->postfixExpr = postfixExpr;
	this->whileStmtParents = whileStmtParents;
	this->ifStmtParents = ifStmtParents;
	this->statementLineNum = statementLineNum;
};

void spa::ReadStatement::processStatement(spa::PKB& pkb) {
	std::string stringStmtLineNum = std::to_string(statementLineNum);
	pkb.addEntity(VARIABLE, variableName);
	pkb.addStatementProc(stringStmtLineNum, parentProcedureVal);
  pkb.addStatementType(stringStmtLineNum, StatementType::READ);
	pkb.addRelationship(MODIFIES, stringStmtLineNum, variableName);
	addParentModifies(pkb, variableName, whileStmtParents, ifStmtParents);
}

void spa::PrintStatement::processStatement(spa::PKB& pkb) {
	std::string stringStmtLineNum = std::to_string(statementLineNum);
	pkb.addEntity(VARIABLE, variableName);
	pkb.addStatementProc(stringStmtLineNum, parentProcedureVal);
	pkb.addStatementType(stringStmtLineNum, StatementType::PRINT);
	pkb.addRelationship(USES, stringStmtLineNum, variableName);
	addParentUses(pkb, variableName, whileStmtParents, ifStmtParents);
}

void spa::CallStatement::processStatement(spa::PKB& pkb) {
	std::string stringStmtLineNum = std::to_string(statementLineNum);
	pkb.addEntity(VARIABLE, variableName);
	pkb.addStatementProc(stringStmtLineNum, parentProcedureVal);
	pkb.addStatementType(stringStmtLineNum, StatementType::CALL);
	pkb.addRelationship(MODIFIES, stringStmtLineNum, variableName);
}

void spa::AssignStatement::processStatement(spa::PKB& pkb) {
	std::string stringStmtLineNum = std::to_string(statementLineNum);
	pkb.addEntity(VARIABLE, assignVar);
	pkb.addStatementType(stringStmtLineNum, StatementType::ASSIGN);
	pkb.addStatementProc(stringStmtLineNum, parentProcedureVal);
	addParentModifies(pkb, assignVar, whileStmtParents, ifStmtParents);
	extractUsesFromPostfix(pkb, postfixExpr);
	pkb.addPattern(stringStmtLineNum, assignVar, postfixExpr);
}

void spa::IfConditionStatement::processStatement(PKB& pkb) {
	std::string stringStmtLineNum = std::to_string(statementLineNum);
	pkb.addStatementProc(stringStmtLineNum, parentProcedureVal);
	pkb.addStatementType(stringStmtLineNum, StatementType::IF);
	extractUsesFromPostfix(pkb, postfixExpr);
}

void spa::WhileConditionStatement::processStatement(PKB& pkb) {
	std::string stringStmtLineNum = std::to_string(statementLineNum);
	pkb.addStatementProc(stringStmtLineNum, parentProcedureVal);
	pkb.addStatementType(stringStmtLineNum, StatementType::WHILE);
	extractUsesFromPostfix(pkb, postfixExpr);
}

void spa::MultiVarNonContainerStatement::extractUsesFromPostfix(PKB& pkb, std::string postfix) {
	std::string stringStmtLineNum = std::to_string(statementLineNum);
	postfix += " ";
	std::string expr = "";
	for (auto& ch : postfix) {
		if (!isspace(ch)) {
			expr += ch;
			continue;
		}
		if (std::all_of(expr.begin(), expr.end(), ::isdigit)) {
			pkb.addEntity(CONSTANT, expr);
		} else if (std::all_of(expr.begin(), expr.end(), ::isalnum)) {
			pkb.addEntity(VARIABLE, expr);
			pkb.addRelationship(USES, stringStmtLineNum, expr);
			addParentUses(pkb, expr, whileStmtParents, ifStmtParents);
		}
		expr = "";
	}
}

void spa::NonContainerStatement::addParentUses(PKB& pkb, std::string variableName, std::unordered_set<int> whileStmtParents, std::unordered_set<int> ifStmtParents) {
  for (int parent : whileStmtParents) {
		std::string stringParentStmtNum = std::to_string(parent);
		pkb.addRelationship(USES, stringParentStmtNum, variableName);
	}
  for (int parent : ifStmtParents) {
		std::string stringParentStmtNum = std::to_string(parent);
		pkb.addRelationship(USES, stringParentStmtNum, variableName);
	}
}

void spa::NonContainerStatement::addParentModifies(PKB& pkb, std::string variableName, std::unordered_set<int> whileStmtParents, std::unordered_set<int> ifStmtParents) {
	for (int parent : whileStmtParents) {
		std::string stringParentStmtNum = std::to_string(parent);
		pkb.addRelationship(MODIFIES, stringParentStmtNum, variableName);
	}
	for (int parent : ifStmtParents) {
		std::string stringParentStmtNum = std::to_string(parent);
		pkb.addRelationship(MODIFIES, stringParentStmtNum, variableName);
	}
}
