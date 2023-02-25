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
  std::optional<std::vector<std::pair<int, std::string>>> lineNumberNamePairs;
  std::optional<std::vector<std::pair<int, int>>> lineNumberLineNumberPairs;
  std::optional<std::vector<std::pair<std::string, std::string>>> procNameProcNamePairs;
  std::optional<bool> isTrue;

 public:
  const QueryResultType& getQueryResultType();
  const std::vector<std::string>& getNames();
  const std::vector<int>& getLineNumbers();
  const std::vector<std::pair<int, std::string>>& getLineNumberNamePairs();
  const std::vector<std::pair<int, int>>& getLineNumberLineNumberPairs();
  const std::vector<std::pair<std::string, std::string>>& getProcNameProcNamePairs();
  const bool& getIsTrue();

  void setQueryResultType(QueryResultType queryResultType);
  void setNames(std::vector<std::string> names);
  void setLineNumbers(std::vector<int> lineNumbers);
  void setLineNumberNamePairs(std::vector<std::pair<int, std::string>> lineNumberNamePairs);
  void setLineNumberLineNumberPairs(std::vector<std::pair<int, int>> lineNumberLineNumberPairs);
  void setIsTrue(bool isTrue);
  void setProcNameProcNamePairs(std::vector<std::pair<std::string, std::string>> procNameProcNamePairs);
};
}  // namespace spa
