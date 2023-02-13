#include "NonContainerStatement.h"

#include <algorithm>

std::string spa::OneVarNonContainerStatement::getVariableName() {
  return variableName;
}

// Constructor for ReadStatement
spa::ReadStatement::ReadStatement(std::string parentProcedureVal, std::string variableName,
                                  std::unordered_set<int> whileStmtParents,
                                  std::unordered_set<int> ifStmtParents,
                                  int statementLineNum) {
  this->parentProcedureVal = parentProcedureVal;
  this->variableName = variableName;
  this->whileStmtParents = whileStmtParents;
  this->ifStmtParents = ifStmtParents;
  this->statementLineNum = statementLineNum;
}

// Constructor for PrintStatement
spa::PrintStatement::PrintStatement(std::string parentProcedureVal, std::string variableName,
                                    std::unordered_set<int> whileStmtParents,
                                    std::unordered_set<int> ifStmtParents,
                                    int statementLineNum) {
  this->parentProcedureVal = parentProcedureVal;
  this->variableName = variableName;
  this->whileStmtParents = whileStmtParents;
  this->ifStmtParents = ifStmtParents;
  this->statementLineNum = statementLineNum;
}

// Constructor for CallStatement
spa::CallStatement::CallStatement(std::string parentProcedureVal, std::string variableName,
                                  std::unordered_set<int> whileStmtParents,
                                  std::unordered_set<int> ifStmtParents,
                                  int statementLineNum) {
  this->parentProcedureVal = parentProcedureVal;
  this->variableName = variableName;
  this->whileStmtParents = whileStmtParents;
  this->ifStmtParents = ifStmtParents;
  this->statementLineNum = statementLineNum;
}

// Constructor for ifConditionStatement
spa::AssignStatement::AssignStatement(std::string parentProcedureVal, std::string assignVar,
                                      std::string postfixExpr,
                                      std::unordered_set<int> whileStmtParents,
                                      std::unordered_set<int> ifStmtParents, int statementLineNum) {
  this->parentProcedureVal = parentProcedureVal;
  this->assignVar = assignVar;
  this->postfixExpr = postfixExpr;
  this->whileStmtParents = whileStmtParents;
  this->ifStmtParents = ifStmtParents;
  this->statementLineNum = statementLineNum;
}

// Constructor for AssignStatement
spa::IfConditionStatement::IfConditionStatement(std::string parentProcedureVal,
                                                std::string postfixExpr,
                                                std::unordered_set<int> whileStmtParents,
                                                std::unordered_set<int> ifStmtParents,
                                                int statementLineNum) {
  this->parentProcedureVal = parentProcedureVal;
  this->postfixExpr = postfixExpr;
  this->whileStmtParents = whileStmtParents;
  this->ifStmtParents = ifStmtParents;
  this->statementLineNum = statementLineNum;
}

// Constructor for whileConditionStatement
spa::WhileConditionStatement::WhileConditionStatement(std::string parentProcedureVal,
                                                      std::string postfixExpr,
                                                      std::unordered_set<int> whileStmtParents,
                                                      std::unordered_set<int> ifStmtParents,
                                                      int statementLineNum) {
  this->parentProcedureVal = parentProcedureVal;
  this->postfixExpr = postfixExpr;
  this->whileStmtParents = whileStmtParents;
  this->ifStmtParents = ifStmtParents;
  this->statementLineNum = statementLineNum;
}

void spa::ReadStatement::processStatement(spa::PKBManager& pkbManager) {
  std::string stringStmtLineNum = std::to_string(statementLineNum);
  pkbManager.addEntity(VARIABLE, variableName);
  pkbManager.addStatementProc(stringStmtLineNum, parentProcedureVal);
  pkbManager.addStatementType(stringStmtLineNum, StatementType::READ);
  pkbManager.addRelationship(MODIFIES, stringStmtLineNum, variableName);
  addParentModifies(pkbManager, variableName, whileStmtParents, ifStmtParents);
}

void spa::PrintStatement::processStatement(spa::PKBManager& pkbManager) {
  std::string stringStmtLineNum = std::to_string(statementLineNum);
  pkbManager.addEntity(VARIABLE, variableName);
  pkbManager.addStatementProc(stringStmtLineNum, parentProcedureVal);
  pkbManager.addStatementType(stringStmtLineNum, StatementType::PRINT);
  pkbManager.addRelationship(USES, stringStmtLineNum, variableName);
  addParentUses(pkbManager, variableName, whileStmtParents, ifStmtParents);
}

void spa::CallStatement::processStatement(spa::PKBManager& pkbManager) {
  std::string stringStmtLineNum = std::to_string(statementLineNum);
  pkbManager.addEntity(VARIABLE, variableName);
  pkbManager.addStatementProc(stringStmtLineNum, parentProcedureVal);
  pkbManager.addStatementType(stringStmtLineNum, StatementType::CALL);
  pkbManager.addRelationship(MODIFIES, stringStmtLineNum, variableName);
}

void spa::AssignStatement::processStatement(spa::PKBManager& pkbManager) {
  std::string stringStmtLineNum = std::to_string(statementLineNum);
  pkbManager.addEntity(VARIABLE, assignVar);
  pkbManager.addStatementProc(stringStmtLineNum, parentProcedureVal);
  pkbManager.addStatementType(stringStmtLineNum, StatementType::ASSIGN);
  pkbManager.addRelationship(MODIFIES, stringStmtLineNum, assignVar);
  addParentModifies(pkbManager, assignVar, whileStmtParents, ifStmtParents);
  extractUsesFromPostfix(pkbManager, postfixExpr);
  pkbManager.addPattern(stringStmtLineNum, assignVar, postfixExpr);
}

void spa::IfConditionStatement::processStatement(PKBManager& pkbManager) {
  std::string stringStmtLineNum = std::to_string(statementLineNum);
  pkbManager.addStatementProc(stringStmtLineNum, parentProcedureVal);
  pkbManager.addStatementType(stringStmtLineNum, StatementType::IF);
  extractUsesFromPostfix(pkbManager, postfixExpr);
}

void spa::WhileConditionStatement::processStatement(PKBManager& pkbManager) {
  std::string stringStmtLineNum = std::to_string(statementLineNum);
  pkbManager.addStatementProc(stringStmtLineNum, parentProcedureVal);
  pkbManager.addStatementType(stringStmtLineNum, StatementType::WHILE);
  extractUsesFromPostfix(pkbManager, postfixExpr);
}

void spa::MultiVarNonContainerStatement::extractUsesFromPostfix(
  PKBManager& pkbManager, std::string postfix) {
  std::string stringStmtLineNum = std::to_string(statementLineNum);
  postfix += " ";
  std::string expr = "";
  for (auto& ch : postfix) {
    if (!isspace(ch)) {
      expr += ch;
      continue;
    }
    if (std::all_of(expr.begin(), expr.end(), ::isdigit)) {
      pkbManager.addEntity(CONSTANT, expr);
    } else if (std::all_of(expr.begin(), expr.end(), ::isalnum)) {
      pkbManager.addEntity(VARIABLE, expr);
      pkbManager.addRelationship(USES, stringStmtLineNum, expr);
      addParentUses(pkbManager, expr, whileStmtParents, ifStmtParents);
    }
    expr = "";
  }
}

void spa::NonContainerStatement::addParentUses(PKBManager& pkbManager, std::string variableName,
                                               std::unordered_set<int> whileStmtParents,
                                               std::unordered_set<int> ifStmtParents) {
  for (int parent : whileStmtParents) {
    std::string stringParentStmtNum = std::to_string(parent);
    pkbManager.addRelationship(USES, stringParentStmtNum, variableName);
  }
  for (int parent : ifStmtParents) {
    std::string stringParentStmtNum = std::to_string(parent);
    pkbManager.addRelationship(USES, stringParentStmtNum, variableName);
  }
}

void spa::NonContainerStatement::addParentModifies(PKBManager& pkbManager, std::string variableName,
                                                   std::unordered_set<int> whileStmtParents,
                                                   std::unordered_set<int> ifStmtParents) {
  for (int parent : whileStmtParents) {
    std::string stringParentStmtNum = std::to_string(parent);
    pkbManager.addRelationship(MODIFIES, stringParentStmtNum, variableName);
  }
  for (int parent : ifStmtParents) {
    std::string stringParentStmtNum = std::to_string(parent);
    pkbManager.addRelationship(MODIFIES, stringParentStmtNum, variableName);
  }
}
