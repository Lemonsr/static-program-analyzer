#pragma once

#include "QueryResult.h"
#include "PKBQueryArg.h"
#include "CFGNode.h"

#include <unordered_map>
#include <string>
#include <optional>
#include <unordered_set>

namespace spa {
class CFGStorage {
 private:
  std::unordered_map<int, std::unordered_set<int>> nextTable;
  std::unordered_map<int, spa::CFGNode> cfgTable;

 public:
  bool addCfgNode(int lineNumber, spa::CFGNode cfgNode);
  bool updateCfgNode(int lineNumber, spa::CFGNode newCfgNode);
  bool deleteCfgNode(int lineNumber);
  QueryResult getCfgNode(int lineNumber);

  void setCfgTable(std::unordered_map<int, spa::CFGNode> cfgTable);
};
}  // namespace spa
