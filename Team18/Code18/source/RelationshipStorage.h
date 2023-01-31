#pragma once

#include <unordered_map>
#include <string>
#include <optional>

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
  std::unordered_map<int, std::unordered_set<std::string>> usesTable;
  std::unordered_map<int, std::string> usesProcTable;
  std::unordered_map<int, StatementType> statementTypeTable;
  std::unordered_map<int, std::string> statementProcTable;

 public:
  bool addStatementType(std::string lineNo, StatementType statementType);
  bool addStatementProc(std::string lineNo, std::string procName);
  QueryResult getStatements(std::optional<StatementType> statementType);

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

  bool addFollows(std::string firstLineNo, std::string secondLineNo);
  QueryResult getFollowsLineLine(PkbQueryArg firstArg, PkbQueryArg secondArg);
  QueryResult getFollowsLineStatement(PkbQueryArg firstArg, PkbQueryArg secondArg);
  QueryResult getFollowsStatementLine(PkbQueryArg firstArg, PkbQueryArg secondArg);
  QueryResult getFollowsLineUnderscore(PkbQueryArg firstArg, PkbQueryArg secondArg);
  QueryResult getFollowsUnderscoreLine(PkbQueryArg firstArg, PkbQueryArg secondArg);
  QueryResult getFollowsStatementStatement(PkbQueryArg firstArg, PkbQueryArg secondArg);
  QueryResult getFollowsStatementUnderscore(PkbQueryArg firstArg, PkbQueryArg secondArg);
  QueryResult getFollowsUnderscoreStatement(PkbQueryArg firstArg, PkbQueryArg secondArg);
  QueryResult getFollowsUnderscoreUnderscore(PkbQueryArg firstArg, PkbQueryArg secondArg);

  bool addFollowsStar(std::string firstLineNo, std::string secondLineNo);
  QueryResult getFollowsStarLineLine(PkbQueryArg firstArg, PkbQueryArg secondArg);
  QueryResult getFollowsStarLineStatement(PkbQueryArg firstArg, PkbQueryArg secondArg);
  QueryResult getFollowsStarStatementLine(PkbQueryArg firstArg, PkbQueryArg secondArg);
  QueryResult getFollowsStarLineUnderscore(PkbQueryArg firstArg, PkbQueryArg secondArg);
  QueryResult getFollowsStarUnderscoreLine(PkbQueryArg firstArg, PkbQueryArg secondArg);
  QueryResult getFollowsStarStatementStatement(PkbQueryArg firstArg, PkbQueryArg secondArg);
  QueryResult getFollowsStarStatementUnderscore(PkbQueryArg firstArg, PkbQueryArg secondArg);
  QueryResult getFollowsStarUnderscoreStatement(PkbQueryArg firstArg, PkbQueryArg secondArg);
  QueryResult getFollowsStarUnderscoreUnderscore(PkbQueryArg firstArg, PkbQueryArg secondArg);

  bool addParent(std::string firstLineNo, std::string secondLineNo);
  QueryResult getParentLineLine(PkbQueryArg firstArg, PkbQueryArg secondArg);
  QueryResult getParentLineStatement(PkbQueryArg firstArg, PkbQueryArg secondArg);
  QueryResult getParentStatementLine(PkbQueryArg firstArg, PkbQueryArg secondArg);
  QueryResult getParentLineUnderscore(PkbQueryArg firstArg, PkbQueryArg secondArg);
  QueryResult getParentUnderscoreLine(PkbQueryArg firstArg, PkbQueryArg secondArg);
  QueryResult getParentStatementStatement(PkbQueryArg firstArg, PkbQueryArg secondArg);
  QueryResult getParentStatementUnderscore(PkbQueryArg firstArg, PkbQueryArg secondArg);
  QueryResult getParentUnderscoreStatement(PkbQueryArg firstArg, PkbQueryArg secondArg);
  QueryResult getParentUnderscoreUnderscore(PkbQueryArg firstArg, PkbQueryArg secondArg);

  bool addParentStar(std::string firstLineNo, std::string secondLineNo);
  QueryResult getParentStarLineLine(PkbQueryArg firstArg, PkbQueryArg secondArg);
  QueryResult getParentStarLineStatement(PkbQueryArg firstArg, PkbQueryArg secondArg);
  QueryResult getParentStarStatementLine(PkbQueryArg firstArg, PkbQueryArg secondArg);
  QueryResult getParentStarLineUnderscore(PkbQueryArg firstArg, PkbQueryArg secondArg);
  QueryResult getParentStarUnderscoreLine(PkbQueryArg firstArg, PkbQueryArg secondArg);
  QueryResult getParentStarStatementStatement(PkbQueryArg firstArg, PkbQueryArg secondArg);
  QueryResult getParentStarStatementUnderscore(PkbQueryArg firstArg, PkbQueryArg secondArg);
  QueryResult getParentStarUnderscoreStatement(PkbQueryArg firstArg, PkbQueryArg secondArg);
  QueryResult getParentStarUnderscoreUnderscore(PkbQueryArg firstArg, PkbQueryArg secondArg);
};
}  // namespace spa
