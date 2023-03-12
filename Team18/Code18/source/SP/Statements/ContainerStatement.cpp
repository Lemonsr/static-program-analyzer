#include <unordered_set>
#include <utility>

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

//  Constructor for IfContainerStatement
spa::IfContainerStatement::IfContainerStatement(std::string parentProcedureVal,
                                                int statementLineNum,
                                                std::vector<ProgramStatement*> statementList) {
  this->parentProcedureVal = parentProcedureVal;
  this->statementLineNum = statementLineNum;
  this->statementList = statementList;
}

//  Constructor for WhileContainerStatement
spa::WhileContainerStatement::WhileContainerStatement(std::string parentProcedureVal,
                                                      int statementLineNum,
                                                      std::vector<ProgramStatement*>
                                                      statementList) {
  this->parentProcedureVal = parentProcedureVal;
  this->statementLineNum = statementLineNum;
  this->statementList = statementList;
}

//  Constructor for IfContainerStatement
spa::InnerBlockStatement::InnerBlockStatement(std::string parentProcedureVal,
                                              std::vector<ProgramStatement*> statementList) {
  this->parentProcedureVal = parentProcedureVal;
  this->statementList = statementList;
}

std::pair<spa::CFGNode, spa::CFGNode> spa::IfContainerStatement::processStatement(
  PKBManager& pkbManager) {
  ProgramStatement* ifConditionStatement = statementList[0];
  ProgramStatement* thenStatement = statementList[1];
  ProgramStatement* elseStatement = statementList[2];
  std::pair<CFGNode, CFGNode> cfgIfConditionNode = ifConditionStatement->
    processStatement(pkbManager);
  std::pair<CFGNode, CFGNode> cfgThenInnerBlockNode = thenStatement->processStatement(pkbManager);
  std::pair<CFGNode, CFGNode> cfgElseInnerBlockNode = elseStatement->processStatement(pkbManager);
  CFGNode dummyNode = CFGNode();
  int const dummyNodeLineNum = dummyNode.getLineNumber();
  int const cfgIfConditionNodeLineNum = cfgIfConditionNode.second.getLineNumber();
  int const cfgThenInnerBlkNodeStart = cfgThenInnerBlockNode.first.getLineNumber();
  int const cfgElseInnerBlkNodeStart = cfgElseInnerBlockNode.first.getLineNumber();
  int const cfgThenInnerBlkNodeEnd = cfgThenInnerBlockNode.second.getLineNumber();
  int const cfgElseInnerBlkNodeEnd = cfgElseInnerBlockNode.second.getLineNumber();
  pkbManager.addEdge(cfgIfConditionNodeLineNum, cfgThenInnerBlkNodeStart);
  pkbManager.addEdge(cfgIfConditionNodeLineNum, cfgElseInnerBlkNodeStart);
  pkbManager.addEdge(cfgThenInnerBlkNodeEnd, dummyNodeLineNum);
  pkbManager.addEdge(cfgElseInnerBlkNodeEnd, dummyNodeLineNum);
  QueryResult cfgIfCondQueryResult = pkbManager.getCfgNode(cfgIfConditionNodeLineNum);
  QueryResult cfgDummyQueryResult = pkbManager.getCfgNode(-1);
  return std::make_pair(*cfgIfCondQueryResult.getCfgNodes()[0], *cfgDummyQueryResult.getCfgNodes()[0]);
}

std::pair<spa::CFGNode, spa::CFGNode> spa::WhileContainerStatement::processStatement(
  PKBManager& pkbManager) {
  ProgramStatement* whileConditionStatement = statementList[0];
  ProgramStatement* innerWhileBlockStatements = statementList[1];
  std::pair<CFGNode, CFGNode> cfgWhileConditionNode = whileConditionStatement->
    processStatement(pkbManager);
  std::pair<CFGNode, CFGNode> cfgWhileInnerBlockNode = innerWhileBlockStatements->
    processStatement(pkbManager);
  int const cfgWhileConditionNodeLineNum = cfgWhileConditionNode.second.getLineNumber();
  int const cfgWhileInnerBlkNodeStart = cfgWhileInnerBlockNode.first.getLineNumber();
  int const cfgWhileInnerBlkNodeEnd = cfgWhileInnerBlockNode.second.getLineNumber();
  pkbManager.addEdge(cfgWhileConditionNodeLineNum, cfgWhileInnerBlkNodeStart);
  pkbManager.addEdge(cfgWhileInnerBlkNodeEnd, cfgWhileConditionNodeLineNum);
  return std::make_pair(cfgWhileConditionNode.first, cfgWhileConditionNode.first);
}

std::pair<spa::CFGNode, spa::CFGNode> spa::InnerBlockStatement::processStatement(
  PKBManager& pkbManager) {
  bool start = true;
  CFGNode blockStmtHeadNode;
  CFGNode prevStmtEndNode;
  for (auto statement : statementList) {
    std::pair<CFGNode, CFGNode> cfgStmtNode = statement->processStatement(pkbManager);
    int const prevStmtEndNodeLineNum = prevStmtEndNode.getLineNumber();
    int const cfgStmtNodeStart = cfgStmtNode.first.getLineNumber();
    if (start) {
      blockStmtHeadNode = cfgStmtNode.first;
      prevStmtEndNode = cfgStmtNode.second;
      start = false;
      continue;
    }
    pkbManager.addEdge(prevStmtEndNodeLineNum, cfgStmtNodeStart);
    prevStmtEndNode = cfgStmtNode.second;
  }
  if (blockStmtHeadNode.getLineNumber() == -1 && prevStmtEndNode.getLineNumber() == -1) {
    return std::make_pair(CFGNode(), CFGNode());
  }
  return std::make_pair(blockStmtHeadNode, prevStmtEndNode);
}
