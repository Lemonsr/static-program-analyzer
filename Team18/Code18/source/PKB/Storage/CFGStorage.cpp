#include "CFGStorage.h"
#include "PKBQueryTypes.h"

#include <string>
#include <vector>
#include <utility>
#include <unordered_set>

void spa::CFGStorage::popNode(int lineNumber, RelationshipStorage& relationshipStorage) {
  CFGNode& dummyEdge = cfgNodeTable[-1];
  for (auto& incomingEdge : dummyEdge.getIncomingEdges()) {
    addEdge(incomingEdge->getLineNumber(), lineNumber, relationshipStorage);
  }
}

bool spa::CFGStorage::addCfgNode(int lineNumber, spa::CFGNode cfgNode) {
  if (cfgNodeTable.find(lineNumber) != cfgNodeTable.end()) {
    return false;
  }
  cfgNodeTable.insert({ lineNumber, cfgNode });
  return true;
}

bool spa::CFGStorage::addCfgEndNode(int lineNumber) {
  if (cfgEndNodes.find(lineNumber) != cfgEndNodes.end()) {
    return false;
  }
  cfgEndNodes.insert(lineNumber);
  return true;
}

bool spa::CFGStorage::addEdge(int lineNumberOne, int lineNumberTwo, RelationshipStorage& relationshipStorage) {
  if (lineNumberOne == -1) {
    popNode(lineNumberTwo, relationshipStorage);
    return true;
  }
  CFGNode& nodeOne = cfgNodeTable[lineNumberOne];
  CFGNode& nodeTwo = cfgNodeTable[lineNumberTwo];
  nodeOne.addOutgoingEdge(&nodeTwo);
  nodeTwo.addIncomingEdge(&nodeOne);
  return relationshipStorage.addNext(std::to_string(lineNumberOne), std::to_string(lineNumberTwo));
}

bool spa::CFGStorage::addModifiedVariable(int lineNumber, std::string varName) {
  CFGNode& node = cfgNodeTable[lineNumber];
  node.addModifiedVariable(varName);
  return true;
}

bool spa::CFGStorage::removeDummyNode() {
  if (cfgNodeTable.find(-1) == cfgNodeTable.end()) {
    return false;
  }

  CFGNode& dummyNode = cfgNodeTable[-1];
  for (auto& node : dummyNode.getIncomingEdges()) {
    node->removeOutgoingEdge(&dummyNode);
  }
  cfgNodeTable.erase(-1);
  return true;
}

spa::QueryResult spa::CFGStorage::getCfgNode(int lineNumber) {
  QueryResult queryResult;
  queryResult.setQueryResultType(TUPLE);

  std::vector<spa::CFGNode> cfgNodes;
  queryResult.setCfgNodes(cfgNodes);
  if (cfgNodeTable.find(lineNumber) == cfgNodeTable.end()) {
    return queryResult;
  }

  cfgNodes.push_back(cfgNodeTable[lineNumber]);
  queryResult.setCfgNodes(cfgNodes);
  return queryResult;
}

void spa::CFGStorage::setCfgNodeTable(std::unordered_map<int, spa::CFGNode> cfgNodeTable) {
  this->cfgNodeTable = cfgNodeTable;
}

void spa::CFGStorage::setCfgEndNodeTable(std::unordered_map<std::string, std::unordered_set<int>> cfgEndNodeTable) {
  this->cfgEndNodeTable = cfgEndNodeTable;
}
