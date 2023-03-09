#include "CFGStorage.h"
#include "PKBQueryTypes.h"

#include <string>
#include <vector>
#include <utility>
#include <unordered_set>

bool spa::CFGStorage::addNext(std::string firstLineNo, std::string secondLineNo) {
  int firstLineNumber = std::stoi(firstLineNo);
  int secondLineNumber = std::stoi(secondLineNo);
  if (nextTable.find(firstLineNumber) != nextTable.end() &&
    nextTable[firstLineNumber].find(secondLineNumber) != nextTable[firstLineNumber].end()) {
    return false;
  }

  nextTable[firstLineNumber].insert(secondLineNumber);
  return true;
}

spa::QueryResult spa::CFGStorage::getNextLineLine(PKBQueryArg firstArg, PKBQueryArg secondArg) {
  int firstLineNumber = firstArg.getLineNumber().lineNo;
  int secondLineNumber = secondArg.getLineNumber().lineNo;
  QueryResult queryResult;
  queryResult.setQueryResultType(BOOL);

  if (nextTable.find(firstLineNumber) == nextTable.end() ||
    nextTable[firstLineNumber].find(secondLineNumber) == nextTable[firstLineNumber].end()) {
    queryResult.setIsTrue(false);
    return queryResult;
  }

  queryResult.setIsTrue(true);
  return queryResult;
}

spa::QueryResult spa::CFGStorage::getNextLineStatement(PKBQueryArg firstArg, PKBQueryArg secondArg) {
  int firstLineNumber = firstArg.getLineNumber().lineNo;
  Statement stmt = secondArg.getStatement();
  QueryResult queryResult;
  queryResult.setQueryResultType(TUPLE);

  std::vector<std::pair<int, int>> lineNumberLineNumberPairs;
  queryResult.setLineNumberLineNumberPairs(lineNumberLineNumberPairs);
  if (nextTable.find(firstLineNumber) == nextTable.end()) {
    return queryResult;
  }
  for (auto& secondLineNumber : nextTable[firstLineNumber]) {
    if (stmt.statementType && statementTypeTable[secondLineNumber] != stmt.statementType) {
      continue;
    }
    lineNumberLineNumberPairs.push_back({ firstLineNumber, secondLineNumber });
  }

  queryResult.setLineNumberLineNumberPairs(lineNumberLineNumberPairs);
  return queryResult;
}

spa::QueryResult spa::CFGStorage::getNextStatementLine(PKBQueryArg firstArg, PKBQueryArg secondArg) {
  Statement stmt = firstArg.getStatement();
  int lineNumber = secondArg.getLineNumber().lineNo;
  QueryResult queryResult;
  queryResult.setQueryResultType(TUPLE);

  std::vector<std::pair<int, int>> lineNumberLineNumberPairs;
  for (auto& itr = nextTable.begin(); itr != nextTable.end(); itr++) {
    if ((stmt.statementType && statementTypeTable[itr->first] != stmt.statementType) ||
      itr->second.find(lineNumber) == itr->second.end()) {
      continue;
    }
    lineNumberLineNumberPairs.push_back({ itr->first, lineNumber });
  }

  queryResult.setLineNumberLineNumberPairs(lineNumberLineNumberPairs);
  return queryResult;
}

spa::QueryResult spa::CFGStorage::getNextLineUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg) {
  int lineNumber = firstArg.getLineNumber().lineNo;
  QueryResult queryResult;
  queryResult.setQueryResultType(BOOL);

  if (nextTable.find(lineNumber) == nextTable.end()) {
    queryResult.setIsTrue(false);
    return queryResult;
  }

  queryResult.setIsTrue(true);
  return queryResult;
}

spa::QueryResult spa::CFGStorage::getNextUnderscoreLine(PKBQueryArg firstArg, PKBQueryArg secondArg) {
  int lineNumber = secondArg.getLineNumber().lineNo;
  QueryResult queryResult;
  queryResult.setQueryResultType(BOOL);

  for (auto& itr = nextTable.begin(); itr != nextTable.end(); itr++) {
    if (itr->second.find(lineNumber) != itr->second.end()) {
      queryResult.setIsTrue(true);
      return queryResult;
    }
  }

  queryResult.setIsTrue(false);
  return queryResult;
}

spa::QueryResult spa::CFGStorage::getNextStatementStatement(PKBQueryArg firstArg,
  PKBQueryArg secondArg) {
  Statement firstStmt = firstArg.getStatement();
  Statement secondStmt = secondArg.getStatement();
  QueryResult queryResult;
  queryResult.setQueryResultType(TUPLE);

  std::vector<std::pair<int, int>> lineNumberLineNumberPairs;
  for (auto& itr = nextTable.begin(); itr != nextTable.end(); itr++) {
    if (firstStmt.statementType && statementTypeTable[itr->first] != firstStmt.statementType) {
      continue;
    }
    for (auto& itr2 = nextTable[itr->first].begin(); itr2 != nextTable[itr->first].end(); itr2++) {
      if (secondStmt.statementType && statementTypeTable[*itr2] != secondStmt.statementType) {
        continue;
      }
      lineNumberLineNumberPairs.push_back({ itr->first, *itr2 });
    }
  }

  queryResult.setLineNumberLineNumberPairs(lineNumberLineNumberPairs);
  return queryResult;
}

spa::QueryResult spa::CFGStorage::getNextStatementUnderscore(PKBQueryArg firstArg,
  PKBQueryArg secondArg) {
  Statement firstStmt = firstArg.getStatement();
  QueryResult queryResult;
  queryResult.setQueryResultType(TUPLE);

  std::vector<std::pair<int, int>> lineNumberLineNumberPairs;
  for (auto& itr = nextTable.begin(); itr != nextTable.end(); itr++) {
    if (firstStmt.statementType && statementTypeTable[itr->first] != firstStmt.statementType) {
      continue;
    }
    for (auto& itr2 = nextTable[itr->first].begin(); itr2 != nextTable[itr->first].end(); itr2++) {
      lineNumberLineNumberPairs.push_back({ itr->first, *itr2 });
    }
  }

  queryResult.setLineNumberLineNumberPairs(lineNumberLineNumberPairs);
  return queryResult;
}

spa::QueryResult spa::CFGStorage::getNextUnderscoreStatement(PKBQueryArg firstArg,
  PKBQueryArg secondArg) {
  Statement secondStmt = secondArg.getStatement();
  QueryResult queryResult;
  queryResult.setQueryResultType(TUPLE);

  std::vector<std::pair<int, int>> lineNumberLineNumberPairs;
  for (auto& itr = nextTable.begin(); itr != nextTable.end(); itr++) {
    for (auto& lineNumber : nextTable[itr->first]) {
      if (secondStmt.statementType && statementTypeTable[lineNumber] != secondStmt.statementType) {
        continue;
      }
      lineNumberLineNumberPairs.push_back({ itr->first, lineNumber });
    }
  }

  queryResult.setLineNumberLineNumberPairs(lineNumberLineNumberPairs);
  return queryResult;
}

spa::QueryResult spa::CFGStorage::getNextUnderscoreUnderscore(PKBQueryArg firstArg,
  PKBQueryArg secondArg) {
  QueryResult queryResult;
  queryResult.setQueryResultType(BOOL);
  queryResult.setIsTrue(!nextTable.empty());
  return queryResult;
}

bool spa::CFGStorage::addCfgNode(int lineNumber, spa::CFGNode cfgNode) {
  if (cfgNodeTable.find(lineNumber) != cfgNodeTable.end()) {
    return false;
  }

  cfgNodeTable.insert({ lineNumber, cfgNode });
  return true;
}

bool spa::CFGStorage::updateCfgNode(int lineNumber, spa::CFGNode newCfgNode) {
  if (cfgNodeTable.find(lineNumber) == cfgNodeTable.end()) {
    return false;
  }

  cfgNodeTable[lineNumber] = newCfgNode;
  return true;
}

bool spa::CFGStorage::deleteCfgNode(int lineNumber) {
  if (cfgNodeTable.find(lineNumber) == cfgNodeTable.end()) {
    return false;
  }

  cfgNodeTable.erase(lineNumber);
  return true;
}

spa::QueryResult spa::CFGStorage::getCfgNode(int lineNumber) {
  QueryResult queryResult;
  queryResult.setQueryResultType(TUPLE);

  std::vector<spa::CFGNode> cfgNodes;
  queryResult.setCfgNodes(cfgNodes);
  if (cfgNodeTable.find(lineNumber) == cfgNodeTable.end()) {
    return queryResult;
  }

  cfgNodes.push_back(cfgNodeTable[lineNumber]);
  queryResult.setCfgNodes(cfgNodes);
  return queryResult;
}

void spa::CFGStorage::setNextTable(std::unordered_map<int, std::unordered_set<int>> nextTable) {
  this->nextTable = nextTable;
}

void spa::CFGStorage::setCfgNodeTable(std::unordered_map<int, spa::CFGNode> cfgNodeTable) {
  this->cfgNodeTable = cfgNodeTable;
}
