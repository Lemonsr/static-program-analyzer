#pragma once

#include <unordered_map>
#include <string>

#include "QueryResult.h"
#include "PkbQueryArg.h"

namespace spa {
class RelationshipStorage {
 private:
  std::unordered_map<int, int> followsTable;
  std::unordered_map<int, int> followsStarTable;
  std::unordered_map<int, int> parentTable;
  std::unordered_map<int, int> parentStarTable;
  std::unordered_map<int, std::string> modifiesTable;
  std::unordered_map<int, std::string> modifiesProcTable;
  std::unordered_map<int, std::string> usesTable;
  std::unordered_map<int, std::string> usesProcTable;
 public:
  bool addModifies(std::string lineNo, std::string varName);
  QueryResult getModifiesLineVarName(PkbQueryArg firstArg, PkbQueryArg secondArg);
  QueryResult getModifiesLineUnderscore(PkbQueryArg firstArg, PkbQueryArg secondArg);
  QueryResult getModifiesLineVar(PkbQueryArg firstArg, PkbQueryArg secondArg);
  QueryResult getModifiesStmtVarName(PkbQueryArg firstArg, PkbQueryArg secondArg);
  QueryResult getModifiesStmtUnderscore(PkbQueryArg firstArg, PkbQueryArg secondArg);
  QueryResult getModifiesStmtVar(PkbQueryArg firstArg, PkbQueryArg secondArg);

  bool addUses(std::string lineNo, std::string varName);
  QueryResult getUsesLineVarName(PkbQueryArg firstArg, PkbQueryArg secondArg);
  QueryResult getUsesLineUnderscore(PkbQueryArg firstArg, PkbQueryArg secondArg);
  QueryResult getUsesLineVar(PkbQueryArg firstArg, PkbQueryArg secondArg);
  QueryResult getUsesStmtVarName(PkbQueryArg firstArg, PkbQueryArg secondArg);
  QueryResult getUsesStmtUnderscore(PkbQueryArg firstArg, PkbQueryArg secondArg);
  QueryResult getUsesStmtVar(PkbQueryArg firstArg, PkbQueryArg secondArg);
};
}