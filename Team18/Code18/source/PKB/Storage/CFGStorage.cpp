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

bool spa::CFGStorage::addCfgEndNode(int lineNumber) {
  cfgEndNodes.insert(lineNumber);
}

bool spa::CFGStorage::addEdge(int lineNumber1, int lineNumber2, RelationshipStorage& relationshipStorage) {
  if (lineNumber1 == -1) {
    popNode(lineNumber2, relationshipStorage);
    return true;
  }
  CFGNode& edge1 = cfgNodeTable[lineNumber1];
  CFGNode& edge2 = cfgNodeTable[lineNumber2];
  edge1.addOutgoingEdge(&edge2);
  edge2.addIncomingEdge(&edge1);
  return relationshipStorage.addNext(std::to_string(lineNumber1), std::to_string(lineNumber2));
}

bool spa::CFGStorage::addModifiedVariable(int lineNumber, std::string varName) {
  CFGNode& edge = cfgNodeTable[lineNumber];
  edge.addModifiedVariable(varName);
}

bool spa::CFGStorage::removeDummyNode() {
  CFGNode& dummyEdge = cfgNodeTable[-1];
  for (auto& incomingEdge : dummyEdge.getIncomingEdges()) {
    incomingEdge->removeOutgoingEdge(&dummyEdge);
  }
  cfgNodeTable.erase(-1);
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
