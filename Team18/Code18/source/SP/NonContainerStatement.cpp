#include "NonContainerStatement.h"

#include <algorithm>
#include <iostream>
#include <set>

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

void spa::ReadStatement::processStatement(spa::PKBManager& pkbManager) {
    std::string stringStmtLineNum = std::to_string(statementLineNum);
    pkbManager.addEntity(VARIABLE, variableName);
    pkbManager.addStatementProc(stringStmtLineNum, parentProcedureVal);
    pkbManager.addStatementType(stringStmtLineNum, StatementType::READ);
    pkbManager.addRelationship(MODIFIES, stringStmtLineNum, variableName);
    addParentModifies(pkbManager, variableName);
}

void spa::PrintStatement::processStatement(spa::PKBManager& pkbManager) {
    std::string stringStmtLineNum = std::to_string(statementLineNum);
    pkbManager.addEntity(VARIABLE, variableName);
    pkbManager.addStatementProc(stringStmtLineNum, parentProcedureVal);
    pkbManager.addStatementType(stringStmtLineNum, StatementType::PRINT);
    pkbManager.addRelationship(USES, stringStmtLineNum, variableName);
    addParentUses(pkbManager, variableName);
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
    addParentModifies(pkbManager, assignVar);
    extractUsesFromPostfix(pkbManager, postfixExpr);
    pkbManager.addPattern(stringStmtLineNum, assignVar, postfixExpr);
}

void spa::IfConditionStatement::processStatement(PKBManager& pkbManager) {
    std::string stringStmtLineNum = std::to_string(statementLineNum);
    pkbManager.addStatementProc(stringStmtLineNum, parentProcedureVal);
    pkbManager.addStatementType(stringStmtLineNum, StatementType::IF);
    extractUsesFromPostfix(pkbManager, postfixExpr);
    extractPatternFromPostfix(pkbManager, stringStmtLineNum, postfixExpr, IF);
}

void spa::WhileConditionStatement::processStatement(PKBManager& pkbManager) {
    std::string stringStmtLineNum = std::to_string(statementLineNum);
    pkbManager.addStatementProc(stringStmtLineNum, parentProcedureVal);
    pkbManager.addStatementType(stringStmtLineNum, StatementType::WHILE);
    extractUsesFromPostfix(pkbManager, postfixExpr);
    extractPatternFromPostfix(pkbManager, stringStmtLineNum, postfixExpr, WHILE);
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
            addParentUses(pkbManager, expr);
        }
        expr = "";
    }
}

void spa::MultiVarNonContainerStatement::extractPatternFromPostfix(
    PKBManager& pkbManager, std::string lineNum, std::string postfix, spa::DesignEntityType type) {
    std::set<std::string> controlVariables;
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
        pkbManager.addRelationship(USES, stringParentStmtNum, variableName);
    }
    for (int parent : ifStmtParents) {
        std::string stringParentStmtNum = std::to_string(parent);
        pkbManager.addRelationship(USES, stringParentStmtNum, variableName);
    }
}

void spa::NonContainerStatement::addParentModifies(PKBManager& pkbManager,
                                                   std::string variableName) {
    for (int parent : whileStmtParents) {
        std::string stringParentStmtNum = std::to_string(parent);
        pkbManager.addRelationship(MODIFIES, stringParentStmtNum, variableName);
    }
    for (int parent : ifStmtParents) {
        std::string stringParentStmtNum = std::to_string(parent);
        pkbManager.addRelationship(MODIFIES, stringParentStmtNum, variableName);
    }
}
