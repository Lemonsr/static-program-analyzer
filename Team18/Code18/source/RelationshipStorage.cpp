#include "RelationshipStorage.h"

#include <string>
#include <vector>
#include <utility>
#include <unordered_set>

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
  int lineNumber = std::stoi(lineNo);
  if (statementProcTable.find(lineNumber) != statementProcTable.end()) {
    return false;
  }

  statementProcTable.insert({ lineNumber, procName });
  return true;
}

spa::QueryResult spa::RelationshipStorage::getStatements(
  std::optional<StatementType> statementType) {
  QueryResult queryResult;
  queryResult.setQueryResultType(TUPLE);

  std::vector<int> lineNumbers;
  if (!statementType) {
    for (auto& itr = statementTypeTable.begin(); itr != statementTypeTable.end(); itr++) {
      lineNumbers.push_back(itr->first);
    }
    queryResult.setLineNumbers(lineNumbers);
    return queryResult;
  }

  StatementType stmtType = statementType.value();
  for (auto& itr = statementTypeTable.begin(); itr != statementTypeTable.end(); itr++) {
    if (itr->second == stmtType) {
      lineNumbers.push_back(itr->first);
    }
  }
  queryResult.setLineNumbers(lineNumbers);
  return queryResult;
}

bool spa::RelationshipStorage::addModifies(std::string lineNo, std::string varName) {
  int lineNumber = std::stoi(lineNo);
  if (modifiesTable.find(lineNumber) != modifiesTable.end()) {
    return false;
  }

  modifiesTable.insert({ lineNumber, varName });
  return true;
}

spa::QueryResult spa::RelationshipStorage::getModifiesLineVarName(PKBQueryArg firstArg,
                                                                  PKBQueryArg secondArg) {
  int lineNumber = firstArg.getLineNumber().lineNo;
  std::string varName = secondArg.getName().name;
  QueryResult queryResult;
  queryResult.setQueryResultType(BOOL);

  if (modifiesTable.find(lineNumber) == modifiesTable.end() ||
    modifiesTable[lineNumber] != varName) {
    queryResult.setIsTrue(false);
    return queryResult;
  }

  queryResult.setIsTrue(true);
  return queryResult;
}

spa::QueryResult spa::RelationshipStorage::getModifiesLineUnderscore(PKBQueryArg firstArg,
                                                                     PKBQueryArg secondArg) {
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

spa::QueryResult spa::RelationshipStorage::getModifiesLineVar(PKBQueryArg firstArg,
                                                              PKBQueryArg secondArg) {
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

spa::QueryResult spa::RelationshipStorage::getModifiesStmtVarName(PKBQueryArg firstArg,
                                                                  PKBQueryArg secondArg) {
  Statement stmt = firstArg.getStatement();
  std::string varName = secondArg.getName().name;
  QueryResult queryResult;
  queryResult.setQueryResultType(TUPLE);

  std::vector<std::pair<int, std::string>> lineNumberVariablePairs;
  for (auto itr = modifiesTable.begin(); itr != modifiesTable.end(); itr++) {
    if (itr->second != varName ||
      (stmt.statementType && statementTypeTable[itr->first] != stmt.statementType)) {
      continue;
    }
    lineNumberVariablePairs.push_back({ itr->first, itr->second });
  }

  queryResult.setLineNumberVariablePairs(lineNumberVariablePairs);
  return queryResult;
}

spa::QueryResult spa::RelationshipStorage::getModifiesStmtUnderscore(PKBQueryArg firstArg,
                                                                     PKBQueryArg secondArg) {
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

spa::QueryResult spa::RelationshipStorage::getModifiesStmtVar(PKBQueryArg firstArg,
                                                              PKBQueryArg secondArg) {
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
  int lineNumber = std::stoi(lineNo);
  if (usesTable.find(lineNumber) == usesTable.end()) {
    usesTable[lineNumber] = {};
  }

  usesTable[lineNumber].insert(varName);
  return true;
}

spa::QueryResult spa::RelationshipStorage::getUsesLineVarName(PKBQueryArg firstArg,
                                                              PKBQueryArg secondArg) {
  int lineNumber = firstArg.getLineNumber().lineNo;
  std::string varName = secondArg.getName().name;
  QueryResult queryResult;
  queryResult.setQueryResultType(BOOL);

  if (usesTable.find(lineNumber) == usesTable.end() ||
    usesTable[lineNumber].find(varName) == usesTable[lineNumber].end()) {
    queryResult.setIsTrue(false);
    return queryResult;
  }

  queryResult.setIsTrue(true);
  return queryResult;
}

spa::QueryResult spa::RelationshipStorage::getUsesLineUnderscore(PKBQueryArg firstArg,
                                                                 PKBQueryArg secondArg) {
  int lineNumber = firstArg.getLineNumber().lineNo;
  QueryResult queryResult;
  queryResult.setQueryResultType(BOOL);

  if (usesTable.find(lineNumber) == usesTable.end()) {
    queryResult.setIsTrue(false);
    return queryResult;
  }

  queryResult.setIsTrue(true);
  return queryResult;
}

spa::QueryResult spa::RelationshipStorage::getUsesLineVar(PKBQueryArg firstArg,
                                                          PKBQueryArg secondArg) {
  int lineNumber = firstArg.getLineNumber().lineNo;
  QueryResult queryResult;
  queryResult.setQueryResultType(TUPLE);

  std::vector<std::pair<int, std::string>> lineNumberVariablePairs;
  if (usesTable.find(lineNumber) != usesTable.end()) {
    for (auto& varName : usesTable[lineNumber]) {
      lineNumberVariablePairs.push_back({ lineNumber, varName });
    }
  }

  queryResult.setLineNumberVariablePairs(lineNumberVariablePairs);
  return queryResult;
}

spa::QueryResult spa::RelationshipStorage::getUsesStmtVarName(PKBQueryArg firstArg,
                                                              PKBQueryArg secondArg) {
  Statement stmt = firstArg.getStatement();
  std::string varName = secondArg.getName().name;
  QueryResult queryResult;
  queryResult.setQueryResultType(TUPLE);

  std::vector<std::pair<int, std::string>> lineNumberVariablePairs;
  for (auto itr = usesTable.begin(); itr != usesTable.end(); itr++) {
    if (itr->second.find(varName) != itr->second.end() ||
      (stmt.statementType && statementTypeTable[itr->first] != stmt.statementType)) {
      continue;
    }

    for (auto& varName : itr->second) {
      lineNumberVariablePairs.push_back({ itr->first, varName });
    }
  }

  queryResult.setLineNumberVariablePairs(lineNumberVariablePairs);
  return queryResult;
}

spa::QueryResult spa::RelationshipStorage::getUsesStmtUnderscore(PKBQueryArg firstArg,
                                                                 PKBQueryArg secondArg) {
  Statement stmt = firstArg.getStatement();
  std::string varName = secondArg.getName().name;
  QueryResult queryResult;
  queryResult.setQueryResultType(TUPLE);

  std::vector<std::pair<int, std::string>> lineNumberVariablePairs;
  for (auto itr = usesTable.begin(); itr != usesTable.end(); itr++) {
    if (stmt.statementType && statementTypeTable[itr->first] != stmt.statementType) {
      continue;
    }

    for (auto& varName : itr->second) {
      lineNumberVariablePairs.push_back({ itr->first, varName });
    }
  }

  queryResult.setLineNumberVariablePairs(lineNumberVariablePairs);
  return queryResult;
}

spa::QueryResult spa::RelationshipStorage::getUsesStmtVar(PKBQueryArg firstArg,
                                                          PKBQueryArg secondArg) {
  Statement stmt = firstArg.getStatement();
  QueryResult queryResult;
  queryResult.setQueryResultType(TUPLE);

  std::vector<std::pair<int, std::string>> lineNumberVariablePairs;
  for (auto itr = usesTable.begin(); itr != usesTable.end(); itr++) {
    if (stmt.statementType && statementTypeTable[itr->first] != stmt.statementType) {
      continue;
    }

    for (auto& varName : itr->second) {
      lineNumberVariablePairs.push_back({ itr->first, varName });
    }
  }

  queryResult.setLineNumberVariablePairs(lineNumberVariablePairs);
  return queryResult;
}

void spa::RelationshipStorage::setModifiesTable(std::unordered_map<int, std::string> modifiesTable) {
  this->modifiesTable = modifiesTable;
}

void spa::RelationshipStorage::setUsesTable(std::unordered_map<int, std::unordered_set<std::string>> usesTable) {
  this->usesTable = usesTable;
}

void spa::RelationshipStorage::setStatementTypeTable(std::unordered_map<int, StatementType> statementTypeTable) {
  this->statementTypeTable = statementTypeTable;
}
