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
  std::unordered_map<int, spa::CFGNode> cfgNodeTable;
  std::unordered_map<std::string, std::unordered_set<int>> cfgEndNodeTable;

 public:
  bool addCfgNode(int lineNumber, spa::CFGNode cfgNode);
  bool updateCfgNode(int lineNumber, spa::CFGNode newCfgNode);
  bool deleteCfgNode(int lineNumber);
  QueryResult getCfgNode(int lineNumber);
  
  void setCfgNodeTable(std::unordered_map<int, spa::CFGNode> cfgNodeTable);
};
}  // namespace spa
