#pragma once

#include <unordered_set>
#include <unordered_map>
#include <string>

#include "PKBTypes.h"
#include "QueryResult.h"

namespace spa {
class EntityStorage {
 private:
  std::unordered_set<std::string> varTable;
  std::unordered_set<std::string> constTable;
  std::unordered_set<std::string> procTable;
 public:
  bool addVar(std::string varName);
  bool addConst(std::string constName);
  bool addProc(std::string procName);

  QueryResult getVars();
  QueryResult getConsts();
  QueryResult getProcs();

  void setVarTable(std::unordered_set<std::string> varTable);
  void setConstTable(std::unordered_set<std::string> constTable);
  void setProcTable(std::unordered_set<std::string> procTable);
};
}  // namespace spa
