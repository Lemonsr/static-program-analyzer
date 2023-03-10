#pragma once

#include "QueryResult.h"
#include "RelationshipStorage.h"
#include "PKBQueryArg.h"
#include "CFGNode.h"

#include <unordered_map>
#include <string>
#include <optional>
#include <unordered_set>

namespace spa {
class CFGStorage {
 private:
  std::unordered_map<int, spa::CFGNode> cfgNodeTable;
  std::unordered_map<std::string, std::unordered_set<int>> cfgEndNodeTable;
  std::unordered_set<int> cfgEndNodes;

  void popNode(int lineNumber, RelationshipStorage& relationshipStorage);

 public:
  bool addCfgEndNode(int lineNumber);
  bool addEdge(int lineNumber1, int lineNumber2, RelationshipStorage& relationshipStorage);
  bool addModifiedVariable(int lineNumber, std::string varName);
  bool removeDummyNode();

  QueryResult getCfgNode(int lineNumber);

  void setCfgNodeTable(std::unordered_map<int, spa::CFGNode> cfgNodeTable);
  void setCfgEndNodeTable(std::unordered_map<std::string, std::unordered_set<int>> cfgEndNodeTable);
};
}  // namespace spa
