#include "RelationshipStorage.h"

#include <string>
#include <vector>
#include <utility>

#include "PKBQueryTypes.h"

bool spa::RelationshipStorage::addStatementType(std::string lineNo, StatementType statementType) {
  int lineNumber = std::stoi(lineNo);
  if (statementTypeTable.find(lineNumber) != statementTypeTable.end()) {
    return false;
  }

  statementTypeTable.insert({ lineNumber, statementType });
  return true;
}

bool spa::RelationshipStorage::addStatementProc(std::string lineNo, std::string procName) {
  int lineNumber = stoi(lineNo);
  if (statementProcTable.find(lineNumber) != statementProcTable.end()) {
    return false;
  }

  statementProcTable.insert({ lineNumber, procName });
  return true;
}

bool spa::RelationshipStorage::addModifies(std::string lineNo, std::string varName) {
  int lineNumber = std::stoi(lineNo);
  if (modifiesTable.find(lineNumber) != modifiesTable.end()) {
    return false;
  }

  modifiesTable.insert({ lineNumber, varName });
  return true;
}

spa::QueryResult spa::RelationshipStorage::getModifiesLineVarName(PkbQueryArg firstArg, PkbQueryArg secondArg) {
  int lineNumber = firstArg.getLineNumber().lineNo;
  std::string varName = secondArg.getName().name;
  QueryResult queryResult;
  queryResult.setQueryResultType(BOOL);

  if (modifiesTable.find(lineNumber) == modifiesTable.end() || modifiesTable[lineNumber] != varName) {
    queryResult.setIsTrue(false);
    return queryResult;
  }

  queryResult.setIsTrue(true);
  return queryResult;
}

spa::QueryResult spa::RelationshipStorage::getModifiesLineUnderscore(PkbQueryArg firstArg, PkbQueryArg secondArg) {
  int lineNumber = firstArg.getLineNumber().lineNo;
  QueryResult queryResult;
  queryResult.setQueryResultType(BOOL);

  if (modifiesTable.find(lineNumber) == modifiesTable.end()) {
    queryResult.setIsTrue(false);
    return queryResult;
  }

  queryResult.setIsTrue(true);
  return queryResult;
}

spa::QueryResult spa::RelationshipStorage::getModifiesLineVar(PkbQueryArg firstArg, PkbQueryArg secondArg) {
  int lineNumber = firstArg.getLineNumber().lineNo;
  QueryResult queryResult;
  queryResult.setQueryResultType(TUPLE);

  std::vector<std::pair<int, std::string>> lineNumberVariablePairs;
  if (modifiesTable.find(lineNumber) != modifiesTable.end()) {
    lineNumberVariablePairs.push_back({ lineNumber, modifiesTable[lineNumber] });
  }

  queryResult.setLineNumberVariablePairs(lineNumberVariablePairs);
  return queryResult;
}

spa::QueryResult spa::RelationshipStorage::getModifiesStmtVarName(PkbQueryArg firstArg, PkbQueryArg secondArg) {
  Statement stmt = firstArg.getStatement();
  std::string varName = secondArg.getName().name;
  QueryResult queryResult;
  queryResult.setQueryResultType(TUPLE);

  std::vector<std::pair<int, std::string>> lineNumberVariablePairs;
  for (auto itr = modifiesTable.begin(); itr != modifiesTable.end(); itr++) {
    if (itr->second != varName || (stmt.statementType && statementTypeTable[itr->first] != stmt.statementType)) {
      continue;
    }
    lineNumberVariablePairs.push_back({ itr->first, itr->second });
  }

  queryResult.setLineNumberVariablePairs(lineNumberVariablePairs);
  return queryResult;
}

spa::QueryResult spa::RelationshipStorage::getModifiesStmtUnderscore(PkbQueryArg firstArg, PkbQueryArg secondArg) {
  Statement stmt = firstArg.getStatement();
  QueryResult queryResult;
  queryResult.setQueryResultType(TUPLE);

  std::vector<std::pair<int, std::string>> lineNumberVariablePairs;
  for (auto itr = modifiesTable.begin(); itr != modifiesTable.end(); itr++) {
    if (stmt.statementType && statementTypeTable[itr->first] != stmt.statementType) {
      continue;
    }
    lineNumberVariablePairs.push_back({ itr->first, itr->second });
  }

  queryResult.setLineNumberVariablePairs(lineNumberVariablePairs);
  return queryResult;
}

spa::QueryResult spa::RelationshipStorage::getModifiesStmtVar(PkbQueryArg firstArg, PkbQueryArg secondArg) {
  Statement stmt = firstArg.getStatement();
  QueryResult queryResult;
  queryResult.setQueryResultType(TUPLE);

  std::vector<std::pair<int, std::string>> lineNumberVariablePairs;
  for (auto itr = modifiesTable.begin(); itr != modifiesTable.end(); itr++) {
    if (stmt.statementType && statementTypeTable[itr->first] != stmt.statementType) {
      continue;
    }
    lineNumberVariablePairs.push_back({ itr->first, itr->second });
  }
  
  queryResult.setLineNumberVariablePairs(lineNumberVariablePairs);
  return queryResult;
}

bool spa::RelationshipStorage::addUses(std::string lineNo, std::string varName) {
  return false;
}

spa::QueryResult spa::RelationshipStorage::getUsesLineVarName(PkbQueryArg firstArg, PkbQueryArg secondArg) {
  return spa::QueryResult();
}

spa::QueryResult spa::RelationshipStorage::getUsesLineUnderscore(PkbQueryArg firstArg, PkbQueryArg secondArg) {
  return spa::QueryResult();
}

spa::QueryResult spa::RelationshipStorage::getUsesLineVar(PkbQueryArg firstArg, PkbQueryArg secondArg) {
  return spa::QueryResult();
}

spa::QueryResult spa::RelationshipStorage::getUsesStmtVarName(PkbQueryArg firstArg, PkbQueryArg secondArg) {
  return spa::QueryResult();
}

spa::QueryResult spa::RelationshipStorage::getUsesStmtUnderscore(PkbQueryArg firstArg, PkbQueryArg secondArg) {
  return spa::QueryResult();
}

spa::QueryResult spa::RelationshipStorage::getUsesStmtVar(PkbQueryArg firstArg, PkbQueryArg secondArg) {
  return spa::QueryResult();
}
