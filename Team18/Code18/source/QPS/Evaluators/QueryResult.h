#pragma once

#include "CFGNode.h"

#include <optional>
#include <string>
#include <vector>
#include <utility>
#include <unordered_set>
#include <unordered_map>

namespace spa {
enum QueryResultType {
  BOOL,
  TUPLE
};

class QueryResult {
 private:
  QueryResultType queryResultType;
  std::optional<std::vector<std::string>> names;
  std::optional<std::vector<int>> lineNumbers;
  std::optional<std::vector<std::pair<int, std::string>>> lineNumberNamePairs;
  std::optional<std::vector<std::pair<int, int>>> lineNumberLineNumberPairs;
  std::optional<std::vector<std::pair<std::string, std::string>>> nameNamePairs;
  std::optional<bool> isTrue;
  std::optional<std::vector<CFGNode*>> cfgNodes;
  std::optional<std::vector<int>> cfgEndNodes;
  std::optional<std::unordered_map<int, std::unordered_set<int>>*> affectsTable;

 public:
  const QueryResultType& getQueryResultType();
  const std::vector<std::string>& getNames();
  const std::vector<int>& getLineNumbers();
  const std::vector<std::pair<int, std::string>>& getLineNumberNamePairs();
  const std::vector<std::pair<int, int>>& getLineNumberLineNumberPairs();
  const std::vector<std::pair<std::string, std::string>>& getNameNamePairs();
  const std::vector<spa::CFGNode*>& getCfgNodes();
  const std::vector<int>& getCfgEndNodes();
  const bool& getIsTrue();
  const std::unordered_map<int, std::unordered_set<int>>* getAffectsTable();

  void setQueryResultType(QueryResultType queryResultType);
  void setNames(std::vector<std::string> names);
  void setLineNumbers(std::vector<int> lineNumbers);
  void setLineNumberNamePairs(std::vector<std::pair<int, std::string>> lineNumberNamePairs);
  void setLineNumberLineNumberPairs(std::vector<std::pair<int, int>> lineNumberLineNumberPairs);
  void setIsTrue(bool isTrue);
  void setNameNamePairs(std::vector<std::pair<std::string, std::string>> nameNamePairs);
  void setCfgNodes(std::vector<spa::CFGNode*> cfgNodes);
  void setCfgEndNodes(std::vector<int> cfgEndNodes);
  void setAffectsTable(std::unordered_map<int, std::unordered_set<int>>* affectsTable);
};
}  // namespace spa
