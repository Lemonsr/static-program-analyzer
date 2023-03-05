#include <unordered_set>

#include "ContainerStatement.h"
#include "NonContainerStatement.h"

std::vector<spa::ProgramStatement*> spa::ContainerStatement::getStatementList() {
  return statementList;
}

std::unordered_set<std::string> spa::ContainerStatement::getProceduresCalled() {
    if (proceduresCalled.empty()) {
        for (auto& statement : statementList) {
            if (dynamic_cast<spa::CallStatement*>(statement)) {
                auto callStatement = dynamic_cast<spa::CallStatement*>(statement);
                proceduresCalled.emplace(callStatement->getVariableName());
            } else if (dynamic_cast<spa::ContainerStatement*>(statement)) {
                auto containerStatement = dynamic_cast<spa::ContainerStatement*>(statement);
                std::unordered_set<std::string> calledSet = containerStatement->getProceduresCalled();
                for (auto& called : calledSet) {
                    proceduresCalled.emplace(called);
                }
            }
        }
    }
    return proceduresCalled;
}

// Constructor for IfContainerStatement
spa::IfContainerStatement::IfContainerStatement(std::string parentProcedureVal,
                                                int statementLineNum,
                                                std::vector<ProgramStatement*> statementList) {
  this->parentProcedureVal = parentProcedureVal;
  this->statementLineNum = statementLineNum;
  this->statementList = statementList;
}

// Constructor for WhileContainerStatement
spa::WhileContainerStatement::WhileContainerStatement(std::string parentProcedureVal,
                                                      int statementLineNum,
                                                      std::vector<ProgramStatement*>
                                                      statementList) {
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

std::pair<spa::CFGNode*, spa::CFGNode*> spa::IfContainerStatement::processStatement(PKBManager& pkbManager) {
  ProgramStatement* ifConditionStatement = statementList[0];
  ProgramStatement* thenStatement = statementList[1];
  ProgramStatement* elseStatement = statementList[2];
  std::pair<CFGNode*, CFGNode*> cfgIfConditionNode = ifConditionStatement->processStatement(pkbManager);
  std::pair<CFGNode*, CFGNode*> cfgThenInnerBlockNode = thenStatement->processStatement(pkbManager);
  std::pair<CFGNode*, CFGNode*> cfgElseInnerBlockNode = elseStatement->processStatement(pkbManager);
  CFGNode* dummyNode = CFGNode::createDummyNode();
  CFGNode::linkCFGNodes(cfgIfConditionNode.second, cfgThenInnerBlockNode.first);
  CFGNode::linkCFGNodes(cfgIfConditionNode.second, cfgElseInnerBlockNode.first);
  CFGNode::linkCFGNodes(cfgThenInnerBlockNode.second, dummyNode);
  CFGNode::linkCFGNodes(cfgElseInnerBlockNode.second, dummyNode);
  return std::make_pair(cfgIfConditionNode.first, dummyNode);
}

std::pair<spa::CFGNode*, spa::CFGNode*> spa::WhileContainerStatement::processStatement(PKBManager& pkbManager) {
  ProgramStatement* whileConditionStatement = statementList[0];
  ProgramStatement* innerWhileBlockStatements = statementList[1];
  std::pair<CFGNode*, CFGNode*> cfgWhileConditionNode = whileConditionStatement->processStatement(pkbManager);
  std::pair<CFGNode*, CFGNode*> cfgWhileInnerBlockNode = innerWhileBlockStatements->processStatement(pkbManager);
  CFGNode::linkCFGNodes(cfgWhileConditionNode.second, cfgWhileInnerBlockNode.first);
  CFGNode::linkCFGNodes(cfgWhileInnerBlockNode.second, cfgWhileConditionNode.first);
  return std::make_pair(cfgWhileConditionNode.first, cfgWhileConditionNode.first);
}

std::pair<spa::CFGNode*, spa::CFGNode*> spa::InnerBlockStatement::processStatement(PKBManager& pkbManager) {
  CFGNode* blockStmtHeadNode = nullptr;
  CFGNode* prevStmtEndNode = nullptr;
  for (auto statement : statementList) {
    std::pair<CFGNode*, CFGNode*> cfgStmtNode = statement->processStatement(pkbManager);
    if (!blockStmtHeadNode) {
      blockStmtHeadNode = cfgStmtNode.first;
      prevStmtEndNode = cfgStmtNode.second;
      continue;
    }
    CFGNode::linkCFGNodes(prevStmtEndNode, cfgStmtNode.first);
    prevStmtEndNode = cfgStmtNode.second;
  }
  return std::make_pair(blockStmtHeadNode, prevStmtEndNode);
}
