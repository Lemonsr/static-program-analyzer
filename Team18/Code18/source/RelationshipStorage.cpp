#include "RelationshipStorage.h"
#include "PKBQueryTypes.h"

#include <string>
#include <vector>
#include <utility>
#include <unordered_set>

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

spa::QueryResult spa::RelationshipStorage::getStatements(std::optional<StatementType> statementType) {
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

bool spa::RelationshipStorage::addFollows(std::string firstLineNo, std::string secondLineNo) {
  int firstLineNumber = std::stoi(firstLineNo);
  int secondLineNumber = std::stoi(secondLineNo);
  if (followsTable.find(firstLineNumber) != followsTable.end()) {
    return false;
  }

  followsTable.insert({ firstLineNumber, secondLineNumber });
  return true;
}

spa::QueryResult spa::RelationshipStorage::getFollowsLineLine(PKBQueryArg firstArg, PKBQueryArg secondArg) {
  int firstLineNumber = firstArg.getLineNumber().lineNo;
  int secondLineNumber = secondArg.getLineNumber().lineNo;
  QueryResult queryResult;
  queryResult.setQueryResultType(BOOL);

  if (followsTable.find(firstLineNumber) == followsTable.end() || followsTable[firstLineNumber] != secondLineNumber) {
    queryResult.setIsTrue(false);
    return queryResult;
  }

  queryResult.setIsTrue(true);
  return queryResult;
}

spa::QueryResult spa::RelationshipStorage::getFollowsLineStatement(PKBQueryArg firstArg, PKBQueryArg secondArg) {
  int lineNumber = firstArg.getLineNumber().lineNo;
  Statement stmt = secondArg.getStatement();
  QueryResult queryResult;
  queryResult.setQueryResultType(TUPLE);

  std::vector<std::pair<int, int>> lineNumberLineNumberPairs;
  for (auto itr = followsTable.begin(); itr != followsTable.end(); itr++) {
    if (itr->first != lineNumber ||
      (stmt.statementType && statementTypeTable[itr->second] != stmt.statementType)) {
      continue;
    }
    lineNumberLineNumberPairs.push_back({ lineNumber, itr->second });
  }

  queryResult.setLineNumberLineNumberPairs(lineNumberLineNumberPairs);
  return queryResult;
}

spa::QueryResult spa::RelationshipStorage::getFollowsStatementLine(PKBQueryArg firstArg, PKBQueryArg secondArg) {
  Statement stmt = firstArg.getStatement();
  int lineNumber = secondArg.getLineNumber().lineNo;
  QueryResult queryResult;
  queryResult.setQueryResultType(TUPLE);

  std::vector<std::pair<int, int>> lineNumberLineNumberPairs;
  for (auto itr = followsTable.begin(); itr != followsTable.end(); itr++) {
    if (itr->second != lineNumber ||
       (stmt.statementType && statementTypeTable[itr->first] != stmt.statementType)) {
      continue;
    }
    lineNumberLineNumberPairs.push_back({ itr->first, lineNumber });
  }
  
  queryResult.setLineNumberLineNumberPairs(lineNumberLineNumberPairs);
  return queryResult;
}

spa::QueryResult spa::RelationshipStorage::getFollowsLineUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg) {
  int lineNumber = firstArg.getLineNumber().lineNo;
  QueryResult queryResult;
  queryResult.setQueryResultType(TUPLE);

  std::vector<std::pair<int, int>> lineNumberLineNumberPairs;
  if (followsTable.find(lineNumber) != followsTable.end()) {
    lineNumberLineNumberPairs.push_back({ lineNumber, followsTable[lineNumber] });
  }

  queryResult.setLineNumberLineNumberPairs(lineNumberLineNumberPairs);
  return queryResult;
}

spa::QueryResult spa::RelationshipStorage::getFollowsUnderscoreLine(PKBQueryArg firstArg, PKBQueryArg secondArg) {
  int lineNumber = secondArg.getLineNumber().lineNo;
  QueryResult queryResult;
  queryResult.setQueryResultType(TUPLE);

  std::vector<std::pair<int, int>> lineNumberLineNumberPairs;
  for (auto itr = followsTable.begin(); itr != followsTable.end(); itr++) {
    if (itr->second != lineNumber) {
      continue;
    }
    lineNumberLineNumberPairs.push_back({ itr->first, lineNumber });
  }

  queryResult.setLineNumberLineNumberPairs(lineNumberLineNumberPairs);
  return queryResult;
}

spa::QueryResult spa::RelationshipStorage::getFollowsStatementStatement(PKBQueryArg firstArg, PKBQueryArg secondArg) {
  Statement firstStmt = firstArg.getStatement();
  Statement secondStmt = secondArg.getStatement();
  QueryResult queryResult;
  queryResult.setQueryResultType(TUPLE);

  std::vector<std::pair<int, int>> lineNumberLineNumberPairs;
  for (auto itr = followsTable.begin(); itr != followsTable.end(); itr++) {
    if ((firstStmt.statementType && statementTypeTable[itr->first] != firstStmt.statementType) ||
        (secondStmt.statementType && statementTypeTable[itr->second] != secondStmt.statementType)) {
      continue;
    }
    lineNumberLineNumberPairs.push_back({ itr->first, itr->second });
  }

  queryResult.setLineNumberLineNumberPairs(lineNumberLineNumberPairs);
  return queryResult;
}

spa::QueryResult spa::RelationshipStorage::getFollowsStatementUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg) {
  Statement firstStmt = firstArg.getStatement();
  QueryResult queryResult;
  queryResult.setQueryResultType(TUPLE);

  std::vector<std::pair<int, int>> lineNumberLineNumberPairs;
  for (auto itr = followsTable.begin(); itr != followsTable.end(); itr++) {
    if (firstStmt.statementType && statementTypeTable[itr->first] != firstStmt.statementType) {
      continue;
    }
    lineNumberLineNumberPairs.push_back({ itr->first, itr->second });
  }

  queryResult.setLineNumberLineNumberPairs(lineNumberLineNumberPairs);
  return queryResult;
}

spa::QueryResult spa::RelationshipStorage::getFollowsUnderscoreStatement(PKBQueryArg firstArg, PKBQueryArg secondArg) {
  Statement secondStmt = secondArg.getStatement();
  QueryResult queryResult;
  queryResult.setQueryResultType(TUPLE);

  std::vector<std::pair<int, int>> lineNumberLineNumberPairs;
  for (auto itr = followsTable.begin(); itr != followsTable.end(); itr++) {
    if (secondStmt.statementType && statementTypeTable[itr->second] != secondStmt.statementType) {
      continue;
    }
    lineNumberLineNumberPairs.push_back({ itr->first, itr->second });
  }

  queryResult.setLineNumberLineNumberPairs(lineNumberLineNumberPairs);
  return queryResult;
}

spa::QueryResult spa::RelationshipStorage::getFollowsUnderscoreUnderscore(PKBQueryArg firstArg,
                                                                          PKBQueryArg secondArg) {
  QueryResult queryResult;
  queryResult.setQueryResultType(TUPLE);

  std::vector<std::pair<int, int>> lineNumberLineNumberPairs;
  for (auto itr = followsTable.begin(); itr != followsTable.end(); itr++) {
    lineNumberLineNumberPairs.push_back({ itr->first, itr->second });
  }

  queryResult.setLineNumberLineNumberPairs(lineNumberLineNumberPairs);
  return queryResult;
}

bool spa::RelationshipStorage::addFollowsStar(std::string firstLineNo, std::string secondLineNo) {
  return false;
}

spa::QueryResult spa::RelationshipStorage::getFollowsStarLineLine(PKBQueryArg firstArg, PKBQueryArg secondArg) {
  return QueryResult();
}

spa::QueryResult spa::RelationshipStorage::getFollowsStarLineStatement(PKBQueryArg firstArg, PKBQueryArg secondArg) {
  return QueryResult();
}

spa::QueryResult spa::RelationshipStorage::getFollowsStarStatementLine(PKBQueryArg firstArg, PKBQueryArg secondArg) {
  return QueryResult();
}

spa::QueryResult spa::RelationshipStorage::getFollowsStarLineUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg) {
  return QueryResult();
}

spa::QueryResult spa::RelationshipStorage::getFollowsStarUnderscoreLine(PKBQueryArg firstArg, PKBQueryArg secondArg) {
  return QueryResult();
}

spa::QueryResult spa::RelationshipStorage::getFollowsStarStatementStatement(PKBQueryArg firstArg,
                                                                            PKBQueryArg secondArg) {
  return QueryResult();
}

spa::QueryResult spa::RelationshipStorage::getFollowsStarStatementUnderscore(PKBQueryArg firstArg,
                                                                             PKBQueryArg secondArg) {
  return QueryResult();
}

spa::QueryResult spa::RelationshipStorage::getFollowsStarUnderscoreStatement(PKBQueryArg firstArg,
                                                                             PKBQueryArg secondArg) {
  return QueryResult();
}

spa::QueryResult spa::RelationshipStorage::getFollowsStarUnderscoreUnderscore(PKBQueryArg firstArg,
                                                                              PKBQueryArg secondArg) {
  return QueryResult();
}

bool spa::RelationshipStorage::addParent(std::string firstLineNo, std::string secondLineNo) {
  return false;
}

spa::QueryResult spa::RelationshipStorage::getParentLineLine(PKBQueryArg firstArg, PKBQueryArg secondArg) {
  return QueryResult();
}

spa::QueryResult spa::RelationshipStorage::getParentLineStatement(PKBQueryArg firstArg, PKBQueryArg secondArg) {
  return QueryResult();
}

spa::QueryResult spa::RelationshipStorage::getParentStatementLine(PKBQueryArg firstArg, PKBQueryArg secondArg) {
  return QueryResult();
}

spa::QueryResult spa::RelationshipStorage::getParentLineUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg) {
  return QueryResult();
}

spa::QueryResult spa::RelationshipStorage::getParentUnderscoreLine(PKBQueryArg firstArg, PKBQueryArg secondArg) {
  return QueryResult();
}

spa::QueryResult spa::RelationshipStorage::getParentStatementStatement(PKBQueryArg firstArg, PKBQueryArg secondArg) {
  return QueryResult();
}

spa::QueryResult spa::RelationshipStorage::getParentStatementUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg) {
  return QueryResult();
}

spa::QueryResult spa::RelationshipStorage::getParentUnderscoreStatement(PKBQueryArg firstArg, PKBQueryArg secondArg) {
  return QueryResult();
}

spa::QueryResult spa::RelationshipStorage::getParentUnderscoreUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg) {
  return QueryResult();
}

bool spa::RelationshipStorage::addParentStar(std::string firstLineNo, std::string secondLineNo) {
  return false;
}

spa::QueryResult spa::RelationshipStorage::getParentStarLineLine(PKBQueryArg firstArg, PKBQueryArg secondArg) {
  return QueryResult();
}

spa::QueryResult spa::RelationshipStorage::getParentStarLineStatement(PKBQueryArg firstArg, PKBQueryArg secondArg) {
  return QueryResult();
}

spa::QueryResult spa::RelationshipStorage::getParentStarStatementLine(PKBQueryArg firstArg, PKBQueryArg secondArg) {
  return QueryResult();
}

spa::QueryResult spa::RelationshipStorage::getParentStarLineUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg) {
  return QueryResult();
}

spa::QueryResult spa::RelationshipStorage::getParentStarUnderscoreLine(PKBQueryArg firstArg, PKBQueryArg secondArg) {
  return QueryResult();
}

spa::QueryResult spa::RelationshipStorage::getParentStarStatementStatement(PKBQueryArg firstArg,
                                                                           PKBQueryArg secondArg) {
  return QueryResult();
}

spa::QueryResult spa::RelationshipStorage::getParentStarStatementUnderscore(PKBQueryArg firstArg,
                                                                            PKBQueryArg secondArg) {
  return QueryResult();
}

spa::QueryResult spa::RelationshipStorage::getParentStarUnderscoreStatement(PKBQueryArg firstArg,
                                                                            PKBQueryArg secondArg) {
  return QueryResult();
}

spa::QueryResult spa::RelationshipStorage::getParentStarUnderscoreUnderscore(PKBQueryArg firstArg,
                                                                             PKBQueryArg secondArg) {
  return QueryResult();
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
  if (usesTable.find(lineNumber) != usesTable.end() &&
    usesTable[lineNumber].find(varName) != usesTable[lineNumber].end()) {
    return false;
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
    if (itr->second.find(varName) == itr->second.end() ||
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
