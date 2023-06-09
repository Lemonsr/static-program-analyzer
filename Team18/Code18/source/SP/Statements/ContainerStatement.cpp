#include "ContainerStatement.h"

#include <memory>
#include <unordered_set>
#include <utility>

#include "NonContainerStatement.h"
#include "Literal.h"

std::vector<std::shared_ptr<spa::ProgramStatement>>& spa::ContainerStatement::getStatementList() {
  return statementList;
}

std::unordered_set<std::string> spa::ContainerStatement::getProceduresCalled() {
  if (proceduresCalled.empty()) {
    for (auto& statement : statementList) {
      if (std::dynamic_pointer_cast<spa::CallStatement>(statement)) {
        auto callStatement = std::dynamic_pointer_cast<spa::CallStatement>(statement);
        proceduresCalled.emplace(callStatement->getVariableName());
      } else if (std::dynamic_pointer_cast<spa::ContainerStatement>(statement)) {
        auto containerStatement = std::dynamic_pointer_cast<spa::ContainerStatement>(statement);
        std::unordered_set<std::string> calledSet = containerStatement->getProceduresCalled();
        for (auto& called : calledSet) {
          proceduresCalled.emplace(called);
        }
      }
    }
  }
  return proceduresCalled;
}

spa::IfContainerStatement::IfContainerStatement(std::string parentProcedureVal,
                                                int statementLineNum,
                                                std::vector<std::shared_ptr<ProgramStatement>>&
                                                statementList) {
  this->parentProcedureVal = parentProcedureVal;
  this->statementLineNum = statementLineNum;
  this->statementList = statementList;
}

spa::WhileContainerStatement::WhileContainerStatement(std::string parentProcedureVal,
                                                      int statementLineNum,
                                                      std::vector<std::shared_ptr<ProgramStatement>>
                                                      statementList) {
  this->parentProcedureVal = parentProcedureVal;
  this->statementLineNum = statementLineNum;
  this->statementList = statementList;
}

spa::InnerBlockStatement::InnerBlockStatement(std::string parentProcedureVal,
                                              std::vector<std::shared_ptr<ProgramStatement>>
                                              statementList) {
  this->parentProcedureVal = parentProcedureVal;
  this->statementList = statementList;
}

std::pair<spa::CFGNode, spa::CFGNode> spa::IfContainerStatement::processStatement(
  PKBManager& pkbManager) {
  std::shared_ptr<ProgramStatement> ifConditionStatement = statementList[0];
  std::shared_ptr<ProgramStatement> thenStatement = statementList[1];
  std::shared_ptr<ProgramStatement> elseStatement = statementList[2];
  std::pair<CFGNode, CFGNode> cfgIfConditionNode = ifConditionStatement->
    processStatement(pkbManager);
  std::pair<CFGNode, CFGNode> cfgThenInnerBlockNode = thenStatement->processStatement(pkbManager);
  std::pair<CFGNode, CFGNode> cfgElseInnerBlockNode = elseStatement->processStatement(pkbManager);
  CFGNode dummyNode = CFGNode();
  int const cfgIfConditionNodeLineNum = cfgIfConditionNode.second.getLineNumber();
  int const cfgThenInnerBlkNodeStart = cfgThenInnerBlockNode.first.getLineNumber();
  int const cfgElseInnerBlkNodeStart = cfgElseInnerBlockNode.first.getLineNumber();
  pkbManager.addEdge(cfgIfConditionNodeLineNum, cfgThenInnerBlkNodeStart);
  pkbManager.addEdge(cfgIfConditionNodeLineNum, cfgElseInnerBlkNodeStart);
  QueryResult res = pkbManager.getCfgNode(cfgIfConditionNodeLineNum);
  relinkDummyNode(dummyNode, pkbManager, cfgThenInnerBlockNode.second);
  relinkDummyNode(dummyNode, pkbManager, cfgElseInnerBlockNode.second);
  return std::make_pair(*res.getCfgNodes()[0], dummyNode);
}

void spa::IfContainerStatement::relinkDummyNode(CFGNode& dummyNode, PKBManager& pkbManager,
                                                CFGNode& innerBlockDummyNode) {
  if (innerBlockDummyNode.getLineNumber() == DUMMY_NODE_VAL) {
    for (auto& incomingNode : innerBlockDummyNode.getIncomingEdges()) {
      QueryResult e = pkbManager.getCfgNode(incomingNode->getLineNumber());
      dummyNode.addIncomingEdge(e.getCfgNodes()[0]);
    }
  } else {
    QueryResult e = pkbManager.getCfgNode(innerBlockDummyNode.getLineNumber());
    dummyNode.addIncomingEdge(e.getCfgNodes()[0]);
  }
}

std::pair<spa::CFGNode, spa::CFGNode> spa::WhileContainerStatement::processStatement(
  PKBManager& pkbManager) {
  std::shared_ptr<ProgramStatement> whileConditionStatement = statementList[0];
  std::shared_ptr<ProgramStatement> innerWhileBlockStatements = statementList[1];
  std::pair<CFGNode, CFGNode> cfgWhileConditionNode = whileConditionStatement->
    processStatement(pkbManager);
  std::pair<CFGNode, CFGNode> cfgWhileInnerBlockNode = innerWhileBlockStatements->
    processStatement(pkbManager);
  int const cfgWhileConditionNodeLineNum = cfgWhileConditionNode.second.getLineNumber();
  int const cfgWhileInnerBlkNodeStart = cfgWhileInnerBlockNode.first.getLineNumber();
  int const cfgWhileInnerBlkNodeEnd = cfgWhileInnerBlockNode.second.getLineNumber();
  pkbManager.addEdge(cfgWhileConditionNodeLineNum, cfgWhileInnerBlkNodeStart);
  if (cfgWhileInnerBlkNodeEnd == DUMMY_NODE_VAL) {
    for (auto& incomingNode : cfgWhileInnerBlockNode.second.getIncomingEdges()) {
      pkbManager.addEdge(incomingNode->getLineNumber(), cfgWhileConditionNodeLineNum);
    }
  } else {
    pkbManager.addEdge(cfgWhileInnerBlkNodeEnd, cfgWhileConditionNodeLineNum);
  }
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
    if (prevStmtEndNode.isDummyNode()) {
      for (auto& incomingNode : prevStmtEndNode.getIncomingEdges()) {
        pkbManager.addEdge(incomingNode->getLineNumber(), cfgStmtNode.first.getLineNumber());
      }
    } else {
      pkbManager.addEdge(prevStmtEndNodeLineNum, cfgStmtNodeStart);
    }
    prevStmtEndNode = cfgStmtNode.second;
  }
  if (blockStmtHeadNode.getLineNumber() == DUMMY_NODE_VAL && prevStmtEndNode.getLineNumber() ==
    DUMMY_NODE_VAL) {
    return std::make_pair(CFGNode(), CFGNode());
  }
  return std::make_pair(blockStmtHeadNode, prevStmtEndNode);
}
