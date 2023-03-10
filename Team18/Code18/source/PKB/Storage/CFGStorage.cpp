#include "CFGStorage.h"
#include "PKBQueryTypes.h"

#include <string>
#include <vector>
#include <utility>
#include <unordered_set>

bool spa::CFGStorage::addCfgNode(int lineNumber, spa::CFGNode cfgNode) {
  if (cfgNodeTable.find(lineNumber) != cfgNodeTable.end()) {
    return false;
  }

  cfgNodeTable.insert({ lineNumber, cfgNode });
  return true;
}

bool spa::CFGStorage::updateCfgNode(int lineNumber, spa::CFGNode newCfgNode) {
  if (cfgNodeTable.find(lineNumber) == cfgNodeTable.end()) {
    return false;
  }

  cfgNodeTable[lineNumber] = newCfgNode;
  return true;
}

bool spa::CFGStorage::deleteCfgNode(int lineNumber) {
  if (cfgNodeTable.find(lineNumber) == cfgNodeTable.end()) {
    return false;
  }

  cfgNodeTable.erase(lineNumber);
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
