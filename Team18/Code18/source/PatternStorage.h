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
  QueryResult getAssign(PKBQueryArg lhs, Pattern rhs);
  QueryResult getAssign(PKBQueryArg lhs, Pattern rhs);
  QueryResult getAssign(PKBQueryArg lhs, Pattern rhs);
};
}  // namespace spa
