#include "PatternStorage.h"
#include "UtilsFunction.h"

#include <string>
#include <vector>

bool spa::PatternStorage::addAssign(std::string lineNo, std::string varName,
  std::string postfixString) {
  int lineNumber = std::stoi(lineNo);
  if (assignTable.find(lineNumber) != assignTable.end()) {
    return false;
  }

  assignTable.insert({ lineNumber, { varName, postfixString } });
  return true;
}

bool spa::PatternStorage::addPatternIf(std::string lineNo, std::string varName) {
  int lineNumber = std::stoi(lineNo);
  if (patternIfTable.find(lineNumber) != patternIfTable.end() &&
      patternIfTable[lineNumber].find(varName) != patternIfTable[lineNumber].end()) {
    return false;
  }

  patternIfTable[lineNumber].insert(varName);
  return true;
}

bool spa::PatternStorage::addPatternWhile(std::string lineNo, std::string varName) {
  int lineNumber = std::stoi(lineNo);
  if (patternWhileTable.find(lineNumber) != patternWhileTable.end() &&
      patternWhileTable[lineNumber].find(varName) != patternWhileTable[lineNumber].end()) {
    return false;
  }

  patternWhileTable[lineNumber].insert(varName);
  return true;
}

spa::QueryResult spa::PatternStorage::getAssignUnderscore(PKBQueryArg lhs, Pattern rhs) {
  PatternType type = rhs.getType();
  std::string queryPattern = UtilsFunction::infixToPostfix(rhs.getValue());
  QueryResult queryResult;
  queryResult.setQueryResultType(TUPLE);

  std::vector<std::pair<int, std::string>> lineNumberVariablePairs;
  for (auto& itr = assignTable.begin(); itr != assignTable.end(); itr++) {
    std::string postfixString = itr->second.second;
    if (type == EXACT) {
      if (queryPattern == postfixString) {
        lineNumberVariablePairs.push_back({ itr->first, itr->second.first });
      }
    } else if (type == PARTIAL) {
      if (postfixString.find(queryPattern) != std::string::npos) {
        lineNumberVariablePairs.push_back({ itr->first, itr->second.first });
      }
    } else if (type == ANY) {
      lineNumberVariablePairs.push_back({ itr->first, itr->second.first });
    }
  }

  queryResult.setLineNumberVariablePairs(lineNumberVariablePairs);
  return queryResult;
}

spa::QueryResult spa::PatternStorage::getAssignVar(PKBQueryArg lhs, Pattern rhs) {
  PatternType type = rhs.getType();
  std::string queryPattern = UtilsFunction::infixToPostfix(rhs.getValue());
  QueryResult queryResult;
  queryResult.setQueryResultType(TUPLE);

  std::vector<std::pair<int, std::string>> lineNumberVariablePairs;
  for (auto& itr = assignTable.begin(); itr != assignTable.end(); itr++) {
    std::string postfixString = itr->second.second;
    if (type == EXACT) {
      if (queryPattern == postfixString) {
        lineNumberVariablePairs.push_back({ itr->first, itr->second.first });
      }
    } else if (type == PARTIAL) {
      if (postfixString.find(queryPattern) != std::string::npos) {
        lineNumberVariablePairs.push_back({ itr->first, itr->second.first });
      }
    } else if (type == ANY) {
      lineNumberVariablePairs.push_back({ itr->first, itr->second.first });
    }
  }

  queryResult.setLineNumberVariablePairs(lineNumberVariablePairs);
  return queryResult;
}

spa::QueryResult spa::PatternStorage::getAssignVarName(PKBQueryArg lhs, Pattern rhs) {
  PatternType type = rhs.getType();
  std::string queryPattern = UtilsFunction::infixToPostfix(rhs.getValue());
  std::string varName = lhs.getName().name;
  QueryResult queryResult;
  queryResult.setQueryResultType(TUPLE);

  std::vector<std::pair<int, std::string>> lineNumberVariablePairs;
  for (auto& itr = assignTable.begin(); itr != assignTable.end(); itr++) {
    if (itr->second.first != varName) {
      continue;
    }
    std::string postfixString = itr->second.second;
    if (type == EXACT) {
      if (queryPattern == postfixString) {
        lineNumberVariablePairs.push_back({ itr->first, itr->second.first });
      }
    } else if (type == PARTIAL) {
      if (postfixString.find(queryPattern) != std::string::npos) {
        lineNumberVariablePairs.push_back({ itr->first, itr->second.first });
      }
    } else if (type == ANY) {
      lineNumberVariablePairs.push_back({ itr->first, itr->second.first });
    }
  }

  queryResult.setLineNumberVariablePairs(lineNumberVariablePairs);
  return queryResult;
}

void spa::PatternStorage::setAssignTable(std::unordered_map<int, std::pair<std::string, std::string>> assignTable) {
  this->assignTable = assignTable;
}

void spa::PatternStorage::setPatternIfTable(std::unordered_map<int, std::unordered_set<std::string>> patternIfTable) {
  this->patternIfTable = patternIfTable;
}

void spa::PatternStorage::setPatternWhileTable(std::unordered_map<int, std::unordered_set<std::string>> patternWhileTable) {
  this->patternWhileTable = patternWhileTable;
}
