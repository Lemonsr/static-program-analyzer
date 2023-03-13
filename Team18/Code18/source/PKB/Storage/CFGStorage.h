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
  std::unordered_set<int> cfgEndNodes;

  bool popDummyNode(int lineNumber, RelationshipStorage& relationshipStorage);

 public:
  bool addCfgNode(int lineNumber, spa::CFGNode cfgNode);
  bool addCfgEndNode(int lineNumber);
  bool addEdge(int lineNumberOne, int lineNumberTwo, RelationshipStorage& relationshipStorage);
  bool addModifiedVariable(int lineNumber, std::string varName);
  bool removeDummyNode();

  QueryResult getCfgNode(int lineNumber);
  QueryResult getCfgEndNodes();

  void setCfgNodeTable(std::unordered_map<int, spa::CFGNode> cfgNodeTable);
};
}  // namespace spa
