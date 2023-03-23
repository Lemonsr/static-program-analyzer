#include "NonContainerStatement.h"

#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <utility>

std::string spa::OneVarNonContainerStatement::getVariableName() {
  return variableName;
}

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

std::pair<spa::CFGNode, spa::CFGNode> spa::ReadStatement::processStatement(
  spa::PKBManager& pkbManager) {
  std::string stringStmtLineNum = std::to_string(statementLineNum);
  pkbManager.addEntity(VARIABLE, variableName);
  pkbManager.addStatementType(stringStmtLineNum, StatementType::READ);
  pkbManager.addRelationship(MODIFIES, stringStmtLineNum, variableName);
  pkbManager.addRelationship(MODIFIES_P, parentProcedureVal, variableName);
  addParentModifies(pkbManager, variableName);
  CFGNode cfgReadStmtNode = CFGNode(statementLineNum, variableName, StatementType::READ);
  pkbManager.addCfgNode(statementLineNum, cfgReadStmtNode);
  return std::make_pair(cfgReadStmtNode, cfgReadStmtNode);
}

std::pair<spa::CFGNode, spa::CFGNode> spa::PrintStatement::processStatement(
  spa::PKBManager& pkbManager) {
  std::string stringStmtLineNum = std::to_string(statementLineNum);
  pkbManager.addEntity(VARIABLE, variableName);
  pkbManager.addStatementType(stringStmtLineNum, StatementType::PRINT);
  pkbManager.addRelationship(USES, stringStmtLineNum, variableName);
  pkbManager.addRelationship(USES_P, parentProcedureVal, variableName);
  addParentUses(pkbManager, variableName);
  CFGNode cfgPrintStmtNode = CFGNode(statementLineNum, StatementType::PRINT);
  cfgPrintStmtNode.addUsesVariable(variableName);
  pkbManager.addCfgNode(statementLineNum, cfgPrintStmtNode);
  return std::make_pair(cfgPrintStmtNode, cfgPrintStmtNode);
}

std::pair<spa::CFGNode, spa::CFGNode> spa::CallStatement::processStatement(
  spa::PKBManager& pkbManager) {
  std::string stringStmtLineNum = std::to_string(statementLineNum);
  pkbManager.addStatementType(stringStmtLineNum, StatementType::CALL);
  pkbManager.addCallsProc(statementLineNum, variableName);
  pkbManager.addRelationship(CALLS, parentProcedureVal, variableName);
  addCallIfWhileParent(pkbManager, variableName);
  CFGNode cfgCallStmtNode = CFGNode(statementLineNum, StatementType::CALL);
  pkbManager.addCfgNode(statementLineNum, cfgCallStmtNode);
  return std::make_pair(cfgCallStmtNode, cfgCallStmtNode);
}

std::pair<spa::CFGNode, spa::CFGNode> spa::AssignStatement::processStatement(
  spa::PKBManager& pkbManager) {
  std::string stringStmtLineNum = std::to_string(statementLineNum);
  CFGNode cfgAssignStmtNode = CFGNode(statementLineNum, assignVar, StatementType::ASSIGN);
  pkbManager.addEntity(VARIABLE, assignVar);
  pkbManager.addStatementType(stringStmtLineNum, StatementType::ASSIGN);
  pkbManager.addRelationship(MODIFIES, stringStmtLineNum, assignVar);
  pkbManager.addRelationship(MODIFIES_P, parentProcedureVal, assignVar);
  addParentModifies(pkbManager, assignVar);
  extractUsesFromPostfix(pkbManager, postfixExpr, cfgAssignStmtNode);
  pkbManager.addPattern(stringStmtLineNum, assignVar, postfixExpr);
  pkbManager.addCfgNode(statementLineNum, cfgAssignStmtNode);
  return std::make_pair(cfgAssignStmtNode, cfgAssignStmtNode);
}

std::pair<spa::CFGNode, spa::CFGNode> spa::IfConditionStatement::processStatement(
  PKBManager& pkbManager) {
  std::string stringStmtLineNum = std::to_string(statementLineNum);
  CFGNode cfgIfConditionalStmtNode = CFGNode(statementLineNum, StatementType::IF);
  pkbManager.addStatementType(stringStmtLineNum, StatementType::IF);
  extractUsesFromPostfix(pkbManager, postfixExpr, cfgIfConditionalStmtNode);
  pkbManager.addCfgNode(statementLineNum, cfgIfConditionalStmtNode);
  extractPatternFromPostfix(pkbManager, stringStmtLineNum, postfixExpr, IF);
  return std::make_pair(cfgIfConditionalStmtNode, cfgIfConditionalStmtNode);
}

std::pair<spa::CFGNode, spa::CFGNode> spa::WhileConditionStatement::processStatement(
  PKBManager& pkbManager) {
  std::string stringStmtLineNum = std::to_string(statementLineNum);
  CFGNode cfgWhileConditionalStmtNode = CFGNode(statementLineNum, StatementType::WHILE);
  pkbManager.addStatementType(stringStmtLineNum, StatementType::WHILE);
  extractUsesFromPostfix(pkbManager, postfixExpr, cfgWhileConditionalStmtNode);
  pkbManager.addCfgNode(statementLineNum, cfgWhileConditionalStmtNode);
  extractPatternFromPostfix(pkbManager, stringStmtLineNum, postfixExpr, WHILE);
  return std::make_pair(cfgWhileConditionalStmtNode, cfgWhileConditionalStmtNode);
}

void spa::MultiVarNonContainerStatement::extractUsesFromPostfix(
  PKBManager& pkbManager, std::string postfix, CFGNode& cfgNode) {
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
      cfgNode.addUsesVariable(expr);
      pkbManager.addEntity(VARIABLE, expr);
      pkbManager.addRelationship(USES, stringStmtLineNum, expr);
      pkbManager.addRelationship(USES_P, parentProcedureVal, expr);
      addParentUses(pkbManager, expr);
    }
    expr = "";
  }
}

void spa::MultiVarNonContainerStatement::extractPatternFromPostfix(
  PKBManager& pkbManager, std::string lineNum, std::string postfix, spa::DesignEntityType type) {
  std::unordered_set<std::string> controlVariables;
  std::string operand;
  for (auto& ch : postfix) {
    if (!isspace(ch)) {
      operand += ch;
    } else if (!operand.empty()) {
      if (std::isalpha(operand[0])) {
        controlVariables.insert(operand);
      }
      operand.clear();
    }
  }

  for (auto& var : controlVariables) {
    pkbManager.addContainerPattern(type, lineNum, var);
  }
}

void spa::NonContainerStatement::addParentUses(PKBManager& pkbManager, std::string variableName) {
  for (int parent : whileStmtParents) {
    std::string stringParentStmtNum = std::to_string(parent);
    QueryResult parentNode = pkbManager.getCfgNode(parent);
    parentNode.getCfgNodes()[0]->addUsesVariable(variableName);
    pkbManager.addRelationship(USES, stringParentStmtNum, variableName);
  }
  for (int parent : ifStmtParents) {
    std::string stringParentStmtNum = std::to_string(parent);
    QueryResult parentNode = pkbManager.getCfgNode(parent);
    parentNode.getCfgNodes()[0]->addUsesVariable(variableName);
    pkbManager.addRelationship(USES, stringParentStmtNum, variableName);
  }
}

void spa::NonContainerStatement::addParentModifies(PKBManager& pkbManager,
                                                   std::string variableName) {
  for (int parent : whileStmtParents) {
    std::string stringParentStmtNum = std::to_string(parent);
    QueryResult parentNode = pkbManager.getCfgNode(parent);
    parentNode.getCfgNodes()[0]->addModifiedVariable(variableName);
    pkbManager.addRelationship(MODIFIES, stringParentStmtNum, variableName);
  }
  for (int parent : ifStmtParents) {
    std::string stringParentStmtNum = std::to_string(parent);
    QueryResult parentNode = pkbManager.getCfgNode(parent);
    parentNode.getCfgNodes()[0]->addModifiedVariable(variableName);
    pkbManager.addRelationship(MODIFIES, stringParentStmtNum, variableName);
  }
}

void spa::CallStatement::addCallIfWhileParent(PKBManager& pkbManager, std::string childProcName) {
  for (int parent : whileStmtParents) {
    pkbManager.addCallsContainerParent(childProcName, std::to_string(parent));
  }

  for (int parent : ifStmtParents) {
    pkbManager.addCallsContainerParent(childProcName, std::to_string(parent));
  }
}
