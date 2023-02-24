#pragma once

#include "QueryResult.h"
#include "PKBQueryArg.h"

#include <unordered_map>
#include <string>
#include <optional>
#include <unordered_set>

namespace spa {
class RelationshipStorage {
 private:
  std::unordered_map<int, int> followsTable;
  std::unordered_map<int, std::unordered_set<int>> followsStarTable;
  std::unordered_map<int, std::unordered_set<int>> parentTable;
  std::unordered_map<int, std::unordered_set<int>> parentStarTable;
  std::unordered_map<int, std::unordered_set<std::string>> usesTable;
  std::unordered_map<std::string, std::unordered_set<std::string>> usesProcTable;
  std::unordered_map<int, std::unordered_set<std::string>> modifiesTable;
  std::unordered_map<std::string, std::unordered_set<std::string>> modifiesProcTable;
  std::unordered_map<std::string, std::unordered_set<std::string>> callsTable;
  std::unordered_map<std::string, std::unordered_set<std::string>> callsStarTable;
  std::unordered_map<std::string, std::unordered_set<int>> callsContainerParentsTable;
  std::unordered_map<int, std::string> callsProcTable;
  std::unordered_map<int, StatementType> statementTypeTable;
  std::unordered_map<int, std::string> statementProcTable;

 public:
  bool addStatementType(std::string lineNo, StatementType statementType);
  bool addStatementProc(std::string lineNo, std::string procName);
  QueryResult getStatements(std::optional<StatementType> statementType);

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

  bool addUses(std::string lineNo, std::string varName);
  QueryResult getUsesLineVarName(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getUsesLineUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getUsesLineVar(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getUsesStmtVarName(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getUsesStmtUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getUsesStmtVar(PKBQueryArg firstArg, PKBQueryArg secondArg);

  bool addModifies(std::string lineNo, std::string varName);
  QueryResult getModifiesLineVarName(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getModifiesLineUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getModifiesLineVar(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getModifiesStmtVarName(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getModifiesStmtUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getModifiesStmtVar(PKBQueryArg firstArg, PKBQueryArg secondArg);

  bool addCalls(std::string firstProc, std::string secondProc);
  QueryResult getCallsNameName(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getCallsNameUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getCallsNameProcedure(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getCallsUnderscoreName(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getCallsUnderscoreUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getCallsUnderscoreProcedure(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getCallsProcedureName(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getCallsProcedureUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getCallsProcedureProcedure(PKBQueryArg firstArg, PKBQueryArg secondArg);

  bool addCallsStar(std::string firstProc, std::string secondProc);
  QueryResult getCallsStarNameName(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getCallsStarNameUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getCallsStarNameProcedure(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getCallsStarUnderscoreName(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getCallsStarUnderscoreUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getCallsStarUnderscoreProcedure(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getCallsStarProcedureName(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getCallsStarProcedureUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getCallsStarProcedureProcedure(PKBQueryArg firstArg, PKBQueryArg secondArg);

  bool addCallsContainerParent(std::string procName, std::string lineNo);
  std::optional<std::unordered_set<int>> getCallsContainerParent(std::string procName);

  bool addCallsProc(int lineNumber, std::string procName);
  QueryResult getCallsProc();

  void setFollowsTable(std::unordered_map<int, int> followsTable);
  void setFollowsStarTable(std::unordered_map<int, std::unordered_set<int>> followsStarTable);
  void setParentTable(std::unordered_map<int, std::unordered_set<int>> parentTable);
  void setParentStarTable(std::unordered_map<int, std::unordered_set<int>> parentStarTable);
  void setUsesTable(std::unordered_map<int, std::unordered_set<std::string>> usesTable);
  void setModifiesTable(std::unordered_map<int, std::unordered_set<std::string>> modifiesTable);
  void setCallsTable(std::unordered_map<std::string, std::unordered_set<std::string>> callsTable);
  void setCallsStarTable(std::unordered_map<std::string, std::unordered_set<std::string>> callsStarTable);
  void setCallsContainerParentsTable(std::unordered_map<std::string,
                                                        std::unordered_set<int>> callsContainerParentsTable);
  void setCallsProcTable(std::unordered_map<int, std::string> statementTypeTable);
  void setStatementTypeTable(std::unordered_map<int, StatementType> statementTypeTable);
};
}  // namespace spa
