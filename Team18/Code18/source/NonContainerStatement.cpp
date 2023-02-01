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
	//pkb.addVariable(variableName);
	//pkb.addStatementProc(statementLineNum, parentProcedureVal);
  pkb.addStatementType(statementLineNum, StatementType::READ);
	//pkb.addModifies(statementLineNum, variableName);
	addParentModifies(pkb, variableName, whileStmtParents, ifStmtParents);
}

void spa::PrintStatement::processStatement(spa::PKB& pkb) {
	//pkb.addVariable(variableName);
	//pkb.addStatementProc(statementLineNum, parentProcedureVal);
  pkb.addStatementType(statementLineNum, StatementType::PRINT);
	//pkb.addUses(statementLineNum, variableName);
	addParentUses(pkb, variableName, whileStmtParents, ifStmtParents);
}

void spa::CallStatement::processStatement(spa::PKB& pkb) {
	//pkb.addVariable(variableName);
	//pkb.addStatementProc(statementLineNum, parentProcedureVal);
	pkb.addStatementType(statementLineNum, StatementType::CALL);
	//pkb.addModifies(statementLineNum, variableName);
}

void spa::AssignStatement::processStatement(spa::PKB& pkb) {
	//pkb.addVariable(assignVar);
	pkb.addStatementType(statementLineNum, StatementType::ASSIGN);
	//pkb.addStatementProc(statementLineNum, parentProcedureVal);
	addParentModifies(pkb, assignVar, whileStmtParents, ifStmtParents);
	extractUsesFromPostfix(pkb, postfixExpr);
	//pkb.addPattern(statementLineNum, assignVar, postfixExpr);
}

void spa::IfConditionStatement::processStatement(PKB& pkb) {
	//pkb.addStatementProc(statementLineNum, parentProcedureVal);
	pkb.addStatementType(statementLineNum, StatementType::IF);
	extractUsesFromPostfix(pkb, postfixExpr);
}

void spa::WhileConditionStatement::processStatement(PKB& pkb) {
	//pkb.addStatementProc(statementLineNum, parentProcedureVal);
	pkb.addStatementType(statementLineNum, StatementType::WHILE);
	extractUsesFromPostfix(pkb, postfixExpr);
}

void spa::MultiVarNonContainerStatement::extractUsesFromPostfix(PKB& pkb, std::string postfix) {
	postfix += " ";
	std::string expr = "";
	for (auto& ch : postfix) {
		if (!isspace(ch)) {
			expr += ch;
			continue;
		}
		if (std::all_of(expr.begin(), expr.end(), ::isdigit)) {
			//pkb.addConstant(stoi(expr));
		} else if (std::all_of(expr.begin(), expr.end(), ::isalnum)) {
			//pkb.addVariable(expr);
			//pkb.addUses(statementLineNum, expr);
			addParentUses(pkb, expr, whileStmtParents, ifStmtParents);
		}
		expr = "";
	}
}

void spa::NonContainerStatement::addParentUses(PKB& pkb, std::string variableName, std::unordered_set<int> whileStmtParents, std::unordered_set<int> ifStmtParents) {
  for (int parent : whileStmtParents) {
		//pkb.addUses(parent, variableName);
	}
  for (int parent : ifStmtParents) {
		//pkb.addUses(parent, variableName);
	}
}

void spa::NonContainerStatement::addParentModifies(PKB& pkb, std::string variableName, std::unordered_set<int> whileStmtParents, std::unordered_set<int> ifStmtParents) {
	for (int parent : whileStmtParents) {
		//pkb.addModifies(parent, variableName);
	}
	for (int parent : ifStmtParents) {
		//pkb.addModifies(parent, variableName);
	}
}
