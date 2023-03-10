#pragma once

#include "QueryResult.h"
#include "PKBQueryArg.h"
#include "CFGNode.h"

#include <unordered_map>
#include <string>
#include <optional>
#include <unordered_set>

namespace spa {

class RelationshipStorage;

class CFGStorage {
 private:
  std::unordered_map<int, std::unordered_set<int>> nextTable;
  std::unordered_map<int, spa::CFGNode> cfgNodeTable;
  std::unordered_map<std::string, std::unordered_set<int>> cfgEndNodeTable;
  RelationshipStorage& relationshipStorage;
 public:
  CFGStorage(RelationshipStorage& relationshipStorage);
  bool addNext(std::string firstLineNo, std::string secondLineNo);
  QueryResult getNextLineLine(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getNextLineStatement(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getNextStatementLine(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getNextLineUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getNextUnderscoreLine(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getNextStatementStatement(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getNextStatementUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getNextUnderscoreStatement(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getNextUnderscoreUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);

  bool addCfgNode(int lineNumber, spa::CFGNode cfgNode);
  bool updateCfgNode(int lineNumber, spa::CFGNode newCfgNode);
  bool deleteCfgNode(int lineNumber);
  QueryResult getCfgNode(int lineNumber);
  
  void setNextTable(std::unordered_map<int, std::unordered_set<int>> nextTable);
  void setCfgNodeTable(std::unordered_map<int, spa::CFGNode> cfgNodeTable);
};
}  // namespace spa
