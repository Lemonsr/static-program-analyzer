#pragma once

#include <unordered_map>
#include <utility>
#include <string>

#include "QueryResult.h"
#include "PkbQueryArg.h"

namespace spa {
class PatternStorage {
 private:
  std::unordered_map<int, std::pair<std::string, std::string>> assignTable;

 public:
  bool addAssign(std::string lineNo, std::string varName, std::string postfixString);
  QueryResult getAssignUnderscore(PkbQueryArg lhs, Pattern rhs);
  QueryResult getAssignVar(PkbQueryArg lhs, Pattern rhs);
  QueryResult getAssignVarName(PkbQueryArg lhs, Pattern rhs);
};
}  // namespace spa
