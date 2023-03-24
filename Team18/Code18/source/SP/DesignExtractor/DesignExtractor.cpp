#include "DesignExtractor.h"

#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <utility>
#include <memory>
#include <queue>

#include "ContainerStatement.h"
#include "NonContainerStatement.h"
#include "ProcedureStatement.h"
#include "SpCyclicValidator.h"

spa::DesignExtractor::DesignExtractor(PKBManager& pkbManager,
                                      std::vector<std::shared_ptr<ProcedureStatement>>& procedureList) :
  pkbManager(pkbManager), procedureList(procedureList) {
  for (auto& procedure : procedureList) {
    auto& statements = procedure->getStatementLst();
    for (auto& statement : statements) {
      if (std::dynamic_pointer_cast<spa::CallStatement>(statement)) {
        auto callStatement = std::dynamic_pointer_cast<spa::CallStatement>(statement);
        procedure->addCalledVars(callStatement->getVariableName());
      } else if (std::dynamic_pointer_cast<spa::ContainerStatement>(statement)) {
        auto containerStatement = std::dynamic_pointer_cast<spa::ContainerStatement>(statement);
        std::unordered_set<std::string> calledSet = containerStatement->getProceduresCalled();
        for (auto& called : calledSet) {
          procedure->addCalledVars(called);
        }
      }
    }
    procCallMap.emplace(procedure->getProcedureVarToken().getValue(),
      procedure->getCalledVars());
  }
}

void spa::DesignExtractor::extractRelationship() {
  spa::SpCyclicValidator cyclicValidator(procCallMap);
  if (cyclicValidator.validateCyclic()) {
    std::cerr << "Cyclic calls detected" << std::endl;
    exit(1);
  }
  for (auto& procedure : procedureList) {
    pkbManager.addEntity(PROCEDURE, procedure->getProcedureVarToken().getValue());
    auto& statementList = procedure->getStatementLst();
    extractDesignAbstraction(statementList);
  }
  extractCallsStar();
  extractUsesAndModifiesProc();
  extractNestedProcUsesAndModifies();
  extractCallsModifiesAndUses();
}

void spa::DesignExtractor::extractDesignAbstraction(std::vector<std::shared_ptr<ProgramStatement>>& statementList) {
  extractFollows(statementList);
  extractFollowsStar(statementList);
  extractParentAbstraction(statementList);
  extractUsesAndModifies(statementList);
}

void spa::DesignExtractor::extractParentAbstraction(std::vector<std::shared_ptr<ProgramStatement>>& statementList) {
  for (auto& statement : statementList) {
    if (std::dynamic_pointer_cast<ContainerStatement>(statement)) {
      auto containerStatement = std::dynamic_pointer_cast<ContainerStatement>(statement);
      extractParent(containerStatement);
      extractParentStar(containerStatement,
        std::to_string(containerStatement->getStatementLineNum()));
    }
  }
}

void spa::DesignExtractor::extractFollows(std::vector<std::shared_ptr<spa::ProgramStatement>>& statementList) {
  for (unsigned i = 0; i < statementList.size(); i++) {
    if (i + 1 != statementList.size()) {
      std::string followStmtOne = std::to_string(statementList[i]->getStatementLineNum());
      std::string followStmtTwo = std::to_string(statementList[i + 1]->getStatementLineNum());

      if (followStmtOne != "-1" && followStmtTwo != "-1") {
        pkbManager.addRelationship(FOLLOWS, followStmtOne, followStmtTwo);
      }
    }
    if (std::dynamic_pointer_cast<ContainerStatement>(statementList[i])) {
      auto containerStatement = std::dynamic_pointer_cast<ContainerStatement>(statementList[i]);
      extractFollows(containerStatement->getStatementList());
    }
  }
}

void spa::DesignExtractor::extractFollowsStar(std::vector<std::shared_ptr<spa::ProgramStatement>>& statementList) {
  for (unsigned i = 0; i < statementList.size(); i++) {
    if (i + 1 != statementList.size()) {
      for (unsigned j = i + 1; j < statementList.size(); j++) {
        std::string followStarStmtOne = std::to_string(statementList[i]->getStatementLineNum());
        std::string followStarStmtTwo = std::to_string(statementList[j]->getStatementLineNum());

        if (followStarStmtOne != "-1" && followStarStmtTwo != "-1") {
          pkbManager.addRelationship(spa::FOLLOWS_STAR, followStarStmtOne, followStarStmtTwo);
        }
      }
    }
    if (std::dynamic_pointer_cast<spa::ContainerStatement>(statementList[i])) {
      auto containerStatement = std::dynamic_pointer_cast<spa::ContainerStatement>(statementList[i]);
      extractFollowsStar(containerStatement->getStatementList());
    }
  }
}

void spa::DesignExtractor::extractParent(std::shared_ptr<spa::ContainerStatement>& containerStatement) {
  std::vector<std::shared_ptr<ProgramStatement>> statementList = containerStatement->getStatementList();
  for (auto& statement : containerStatement->getStatementList()) {
    std::string parentStmtOne = std::to_string(containerStatement->getStatementLineNum());
    if (statement->getStatementLineNum() == containerStatement->getStatementLineNum()) {
      continue;
    }

    if (statement->getStatementLineNum() != -1) {
      std::string parentStmtTwo = std::to_string(statement->getStatementLineNum());
      pkbManager.addRelationship(spa::RelationshipType::PARENT, parentStmtOne, parentStmtTwo);
      continue;
    }
    auto innerBlockStatements = std::dynamic_pointer_cast<ContainerStatement>(statement);
    std::vector<std::shared_ptr<ProgramStatement>> innerBlockStmtList = innerBlockStatements->getStatementList();
    for (auto& childStatement : innerBlockStmtList) {
      std::string parentStmtTwo = std::to_string(childStatement->getStatementLineNum());
      pkbManager.addRelationship(PARENT, parentStmtOne, parentStmtTwo);
      if (std::dynamic_pointer_cast<ContainerStatement>(childStatement)) {
        auto childContainerStmt = std::dynamic_pointer_cast<ContainerStatement>(childStatement);
        extractParent(childContainerStmt);
      }
    }
  }
}

void spa::DesignExtractor::extractParentStar(std::shared_ptr<ContainerStatement>& containerStatement,
                                             std::string ancestorLineNum) {
  std::vector<std::shared_ptr<ProgramStatement>>& statementList = containerStatement->getStatementList();
  for (auto& statement : statementList) {
    std::string parentStmtOne = std::to_string(containerStatement->getStatementLineNum());
    std::string parentStmtTwo = std::to_string(statement->getStatementLineNum());

    if (parentStmtOne == parentStmtTwo) {
      continue;
    }
    if (parentStmtTwo != "-1") {
      pkbManager.addRelationship(PARENT_STAR, ancestorLineNum, parentStmtTwo);
      continue;
    }

    auto innerBlockStatements = std::dynamic_pointer_cast<ContainerStatement>(statement);
    std::vector<std::shared_ptr<ProgramStatement>> innerBlockStmtList = innerBlockStatements->getStatementList();

    for (auto& childStatement : innerBlockStmtList) {
      parentStmtTwo = std::to_string(childStatement->getStatementLineNum());
      pkbManager.addRelationship(PARENT_STAR, ancestorLineNum, parentStmtTwo);

      if (std::dynamic_pointer_cast<ContainerStatement>(childStatement)) {
        auto childContainerStmt = std::dynamic_pointer_cast<ContainerStatement>(childStatement);
        extractParentStar(childContainerStmt, ancestorLineNum);
        extractParentStar(childContainerStmt,
          std::to_string(childStatement->getStatementLineNum()));
      }
    }
  }
}


void spa::DesignExtractor::extractUsesAndModifies(std::vector<std::shared_ptr<spa::ProgramStatement>>& statementList) {
  CFGNode dummyStartNode = CFGNode();
  CFGNode tailNode = dummyStartNode;
  bool start = true;
  for (auto& statement : statementList) {
    std::pair<CFGNode, CFGNode> cfgNode = statement->processStatement(pkbManager);
    if (start && tailNode.isDummyNode()) {
      start = false;
      tailNode = cfgNode.second;
      continue;
    }
    if (tailNode.isDummyNode()) {
      for (auto& incomingNode : tailNode.getIncomingEdges()) {
        pkbManager.addEdge(incomingNode->getLineNumber(), cfgNode.first.getLineNumber());
      }
    } else {
      int const tailNodeLineNum = tailNode.getLineNumber();
      int const cfgNodeStart = cfgNode.first.getLineNumber();
      pkbManager.addEdge(tailNodeLineNum, cfgNodeStart);
    }
    tailNode = cfgNode.second;
  }
  if (tailNode.isDummyNode()) {
    for (auto edge : tailNode.getIncomingEdges()) {
      pkbManager.addCfgEndNode(edge->getLineNumber());
    }
    return;
  }
  pkbManager.addCfgEndNode(tailNode.getLineNumber());
}

void spa::DesignExtractor::dfsCallsStar(std::string parent, std::string child) {
  if (procCallMap.find(child) == procCallMap.end()) return;
  for (auto& childChild : procCallMap[child]) {
    pkbManager.addRelationship(CALLS_STAR, parent, childChild);
    dfsCallsStar(parent, childChild);
  }
}

void spa::DesignExtractor::extractCallsStar() {
  for (auto& procedure : procedureList) {
    std::string currentProcedure = procedure->getProcedureVarToken().getValue();
    for (auto& directCall : procedure->getCalledVars()) {
      pkbManager.addRelationship(CALLS_STAR, currentProcedure, directCall);
      dfsCallsStar(currentProcedure, directCall);
    }
  }
}

void spa::DesignExtractor::extractUsesAndModifiesProc() {
  for (auto& procedure : procedureList) {
    std::string procName = procedure->getProcedureVarToken().getValue();
    std::vector<std::pair<std::string, std::string>> childrenProc = getResFromPkbHelper(procName,
      "p",
      PROCEDURE, CALLS_STAR);
    for (auto& childProc : childrenProc) {
      std::vector<std::pair<std::string, std::string>> varUses = getResFromPkbHelper(
        childProc.second, "v",
        VARIABLE, USES);
      std::vector<std::pair<std::string, std::string>> varModifies = getResFromPkbHelper(
        childProc.second, "v",
        VARIABLE, MODIFIES);
      addUsesModifies(procName, varUses, varModifies);
    }
  }
}

void spa::DesignExtractor::extractNestedProcUsesAndModifies() {
  for (auto& procedure : procedureList) {
    std::string procName = procedure->getProcedureVarToken().getValue();
    QueryResult queryResult = pkbManager.getCallsContainerParent(procName);
    std::vector<int> ifWhileParents = queryResult.getLineNumbers();

    std::vector<std::pair<std::string, std::string>> varUses = getResFromPkbHelper(procName, "v",
      VARIABLE, USES);
    std::vector<std::pair<std::string, std::string>> varModifies = getResFromPkbHelper(procName,
      "v",
      VARIABLE, MODIFIES);
    for (auto& parent : ifWhileParents) {
      addUsesModifies(std::to_string(parent), varUses, varModifies, false);
    }
  }
}

void spa::DesignExtractor::extractCallsModifiesAndUses() {
  QueryResult queryResult = pkbManager.getCallsProc();
  std::vector<std::pair<int, std::string>> callLineNamePairs = queryResult.
    getLineNumberNamePairs();

  for (auto& pair : callLineNamePairs) {
    std::vector<std::pair<std::string, std::string>> varUses = getResFromPkbHelper(pair.second, "v",
      VARIABLE, USES);
    std::vector<std::pair<std::string, std::string>> varModifies = getResFromPkbHelper(pair.second,
      "v",
      VARIABLE, MODIFIES);
    addUsesModifies(std::to_string(pair.first), varUses, varModifies, true);
  }
}

std::vector<std::pair<std::string, std::string>> spa::DesignExtractor::getResFromPkbHelper(
  std::string procName, std::string synonym, DesignEntityType type, RelationshipType relType) {
  PqlArgument firstArg = PqlArgument(LITERAL_STRING, procName, {});
  PqlArgument secondArg = PqlArgument(SYNONYM, synonym,
    {type});
  QueryResult queryResult = pkbManager.getRelationship(relType, PKBQueryArg(firstArg),
    PKBQueryArg(secondArg));
  return queryResult.getNameNamePairs();
}

void spa::DesignExtractor::addUsesModifies(std::string relArg,
                                                  std::vector<std::pair<std::string, std::string>>
                                                  varUses,
                                                  std::vector<std::pair<std::string, std::string>>
                                                  varModifies) {
  for (auto& var : varUses) {
    pkbManager.addRelationship(USES_P, relArg, var.second);
  }
  for (auto& var : varModifies) {
    pkbManager.addRelationship(MODIFIES_P, relArg, var.second);
  }
}

void spa::DesignExtractor::addUsesModifies(std::string relArg,
                                                  std::vector<std::pair<std::string, std::string>>
                                                  varUses,
                                                  std::vector<std::pair<std::string, std::string>>
                                                  varModifies, bool isCallStmt) {
  for (auto& var : varUses) {
    if (isCallStmt) {
      QueryResult queryResult = pkbManager.getCfgNode(stoi(relArg));
      std::vector<CFGNode*> nodes = queryResult.getCfgNodes();
      if (!nodes.empty()) {
        CFGNode* node = nodes[0];
        node->addUsesVariable(var.second);
      }
    } else {
      QueryResult parentNode = pkbManager.getCfgNode(stoi(relArg));
      parentNode.getCfgNodes()[0]->addUsesVariable(var.second);
    }
    pkbManager.addRelationship(USES, relArg, var.second);
  }
  for (auto& var : varModifies) {
    if (isCallStmt) {
      QueryResult queryResult = pkbManager.getCfgNode(stoi(relArg));
      std::vector<CFGNode*> nodes = queryResult.getCfgNodes();
      if (!nodes.empty()) {
        CFGNode* node = nodes[0];
        node->addModifiedVariable(var.second);
      }
    } else {
      QueryResult parentNode = pkbManager.getCfgNode(stoi(relArg));
      parentNode.getCfgNodes()[0]->addModifiedVariable(var.second);
    }
    pkbManager.addRelationship(MODIFIES, relArg, var.second);
  }
}

bool spa::AffectsNodeEquality::operator()(const AffectsNode& first, const AffectsNode& second) const {
  return first.nodeP == second.nodeP && first.usedVariables == second.usedVariables;
}

size_t spa::AffectsNodeHash::operator()(const AffectsNode& node) const {
  return node.nodeP->getLineNumber();
}

void spa::DesignExtractor::populateAffectsForNode(CFGNode* initialNode) {
  std::unordered_set<AffectsNode, AffectsNodeHash, AffectsNodeEquality> seen;
  std::queue<AffectsNode> affectsNodes;
  for (auto edge : initialNode->getIncomingEdges()) {
    affectsNodes.push({ edge, initialNode->getUsesVariables() });
  }
  while (!affectsNodes.empty()) {
    auto affectsNode = affectsNodes.front();
    affectsNodes.pop();
    if (seen.find(affectsNode) != seen.end()) {
      continue;
    }
    seen.insert(affectsNode);
    auto node = affectsNode.nodeP;
    auto currSet = affectsNode.usedVariables;
    if (currSet.empty()) {
      continue;
    }
    auto statementType = node->getStatementType();
    auto modifiedVariables = node->getModifiedVariables();
    if (statementType == StatementType::ASSIGN) {
      auto& modifiedVariable = *(modifiedVariables.begin());
      if (currSet.find(modifiedVariable) != currSet.end()) {
        pkbManager.addRelationship(AFFECTS,
                                   std::to_string(node->getLineNumber()),
                                   std::to_string(initialNode->getLineNumber()));
      }
    }
    if (statementType != StatementType::WHILE && statementType != StatementType::IF) {
      for (auto& modifiedVariable : modifiedVariables) {
        currSet.erase(modifiedVariable);
      }
    }
    for (auto edge : node->getIncomingEdges()) {
      affectsNodes.push({ edge, currSet });
    }
  }
}

void spa::DesignExtractor::populateAffects() {
  QueryResult result = pkbManager.getCfgEndNodes();
  auto& endNodes = result.getCfgNodes();
  std::unordered_set<CFGNode*> seen;
  std::queue<CFGNode*> nodes;
  for (auto node : endNodes) {
    nodes.push(node);
  }
  while (!nodes.empty()) {
    auto node = nodes.front();
    nodes.pop();
    if (seen.find(node) != seen.end()) {
      continue;
    }
    seen.insert(node);
    if (node->getStatementType() == StatementType::ASSIGN) {
      populateAffectsForNode(node);
    }
    for (auto edge : node->getIncomingEdges()) {
      nodes.push(edge);
    }
  }
}

std::unordered_map<int, std::unordered_set<int>> spa::DesignExtractor::extractAffectsStar() {
  auto affectsResult = pkbManager.getAffectsTable();
  auto& affectsTable = *(affectsResult.getIntToSetIntTable());
  std::unordered_map<int, std::unordered_set<int>> result;
  for (auto& p : affectsTable) {
    std::unordered_set<int> seen;
    std::queue<int> lineNos;
    for (auto lineNo : p.second) {
      lineNos.push(lineNo);
    }
    while (!lineNos.empty()) {
      int currLineNo = lineNos.front();
      lineNos.pop();
      if (seen.find(currLineNo) != seen.end()) {
        continue;
      }
      seen.insert(currLineNo);
      result[p.first].insert(currLineNo);
      for (auto lineNo : affectsTable[currLineNo]) {
        lineNos.push(lineNo);
      }
    }
  }
  return result;
}

