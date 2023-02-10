#pragma once

#include <optional>
#include <string>
#include <vector>
#include <utility>
#include <unordered_set>

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
  std::optional<std::vector<std::pair<int, std::string>>> lineNumberVariablePairs;
  std::optional<std::vector<std::pair<int, int>>> lineNumberLineNumberPairs;
  std::optional<bool> isTrue;

 public:
  const QueryResultType& getQueryResultType();
  const std::vector<std::string>& getNames();
  const std::vector<int>& getLineNumbers();
  const std::vector<std::pair<int, std::string>>& getLineNumberVariablePairs();
  const std::vector<std::pair<int, int>>& getLineNumberLineNumberPairs();
  const bool& getIsTrue();

  void setQueryResultType(QueryResultType queryResultType);
  void setNames(std::vector<std::string> names);
  void setLineNumbers(std::vector<int> lineNumbers);
  void setLineNumberVariablePairs(std::vector<std::pair<int, std::string>> lineNumberVariablePairs);
  void setLineNumberLineNumberPairs(std::vector<std::pair<int, int>> lineNumberLineNumberPairs);
  void setIsTrue(bool isTrue);
};
}  // namespace spa
