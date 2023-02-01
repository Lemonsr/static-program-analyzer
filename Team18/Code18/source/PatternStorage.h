#pragma once

#include <unordered_map>
#include <utility>
#include <string>

#include "QueryResult.h"
#include "PKBQueryArg.h"

namespace spa {
class PatternStorage {
 private:
  std::unordered_map<int, std::pair<std::string, std::string>> assignTable;

 public:
  bool addAssign(std::string lineNo, std::string varName, std::string postfixString);
  QueryResult getAssignUnderscore(PKBQueryArg lhs, Pattern rhs);
  QueryResult getAssignVar(PKBQueryArg lhs, Pattern rhs);
  QueryResult getAssignVarName(PKBQueryArg lhs, Pattern rhs);

  void setAssignTable(std::unordered_map<int, std::pair<std::string, std::string>> assignTable);
};
}  // namespace spa
