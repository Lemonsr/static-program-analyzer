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
  QueryResult getAssign(PkbQueryArg lhs, Pattern rhs);
  QueryResult getAssign(PkbQueryArg lhs, Pattern rhs);
  QueryResult getAssign(PkbQueryArg lhs, Pattern rhs);
};
}  // namespace spa
