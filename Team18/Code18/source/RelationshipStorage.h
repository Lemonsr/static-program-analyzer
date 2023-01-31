#pragma once

#include <unordered_map>
#include <string>
#include <optional>

#include "QueryResult.h"
#include "PKBQueryArg.h"

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
  std::unordered_map<int, StatementType> statementTypeTable;
  std::unordered_map<int, std::string> statementProcTable;

 public:
  bool addStatementType(std::string lineNo, StatementType statementType);
  bool addStatementProc(std::string lineNo, std::string procName);
  QueryResult getStatements(std::optional<StatementType> statementType);

  bool addModifies(std::string lineNo, std::string varName);
  QueryResult getModifiesLineVarName(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getModifiesLineUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getModifiesLineVar(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getModifiesStmtVarName(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getModifiesStmtUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getModifiesStmtVar(PKBQueryArg firstArg, PKBQueryArg secondArg);

  bool addUses(std::string lineNo, std::string varName);
  QueryResult getUsesLineVarName(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getUsesLineUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getUsesLineVar(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getUsesStmtVarName(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getUsesStmtUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getUsesStmtVar(PKBQueryArg firstArg, PKBQueryArg secondArg);

  bool addFollows(std::string firstLineNo, std::string secondLineNo);
  QueryResult getFollowsLineLine(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getFollowsLineStatement(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getFollowsStatementLine(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getFollowsLineUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getFollowsUnderscoreLine(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getFollowsStatementStatement(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getFollowsStatementUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getFollowsUnderscoreStatement(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getFollowsUnderscoreUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);

  bool addFollowsStar(std::string firstLineNo, std::string secondLineNo);
  QueryResult getFollowsStarLineLine(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getFollowsStarLineStatement(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getFollowsStarStatementLine(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getFollowsStarLineUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getFollowsStarUnderscoreLine(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getFollowsStarStatementStatement(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getFollowsStarStatementUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getFollowsStarUnderscoreStatement(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getFollowsStarUnderscoreUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);

  bool addParent(std::string firstLineNo, std::string secondLineNo);
  QueryResult getParentLineLine(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getParentLineStatement(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getParentStatementLine(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getParentLineUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getParentUnderscoreLine(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getParentStatementStatement(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getParentStatementUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getParentUnderscoreStatement(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getParentUnderscoreUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);

  bool addParentStar(std::string firstLineNo, std::string secondLineNo);
  QueryResult getParentStarLineLine(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getParentStarLineStatement(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getParentStarStatementLine(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getParentStarLineUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getParentStarUnderscoreLine(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getParentStarStatementStatement(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getParentStarStatementUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getParentStarUnderscoreStatement(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getParentStarUnderscoreUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);
};
}  // namespace spa
