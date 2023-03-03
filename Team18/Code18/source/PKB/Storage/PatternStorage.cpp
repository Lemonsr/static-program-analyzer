#include "PatternStorage.h"
#include "UtilsFunction.h"

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

bool spa::PatternStorage::isPostfixSubstring(std::string postfix, std::string patternPostfix) {
  std::vector<std::string> postfixTokens;
  std::vector<std::string> patternPostfixTokens;

  std::string token;
  std::stringstream ss(postfix);
  while (std::getline(ss, token, ' ')) {
    postfixTokens.push_back(token);
  }

  ss = std::stringstream(patternPostfix);
  while (std::getline(ss, token, ' ')) {
    patternPostfixTokens.push_back(token);
  }

  int postfixLen = postfixTokens.size();
  int patternPostfixLen = patternPostfixTokens.size();
  int postfixIdx = 0;
  int patternPostfixIdx = 0;

  while (postfixIdx < postfixLen && patternPostfixIdx < patternPostfixLen) {
    if (postfixTokens[postfixIdx] == patternPostfixTokens[patternPostfixIdx]) {
      postfixIdx++;
      patternPostfixIdx++;
      if (patternPostfixIdx == patternPostfixLen)
        return true;
    } else {
      postfixIdx = postfixIdx - patternPostfixIdx + 1;
      patternPostfixIdx = 0;
    }
  }
  return false;
}

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

  std::vector<std::pair<int, std::string>> lineNumberNamePairs;
  for (auto& itr = assignTable.begin(); itr != assignTable.end(); itr++) {
    std::string postfixString = itr->second.second;
    if (type == EXACT) {
      if (queryPattern == postfixString) {
        lineNumberNamePairs.push_back({ itr->first, itr->second.first });
      }
    } else if (type == PARTIAL) {
      if (isPostfixSubstring(postfixString, queryPattern)) {
        lineNumberNamePairs.push_back({ itr->first, itr->second.first });
      }
    } else if (type == ANY) {
      lineNumberNamePairs.push_back({ itr->first, itr->second.first });
    }
  }

  queryResult.setLineNumberNamePairs(lineNumberNamePairs);
  return queryResult;
}

spa::QueryResult spa::PatternStorage::getAssignVar(PKBQueryArg lhs, Pattern rhs) {
  PatternType type = rhs.getType();
  std::string queryPattern = UtilsFunction::infixToPostfix(rhs.getValue());
  QueryResult queryResult;
  queryResult.setQueryResultType(TUPLE);

  std::vector<std::pair<int, std::string>> lineNumberNamePairs;
  for (auto& itr = assignTable.begin(); itr != assignTable.end(); itr++) {
    std::string postfixString = itr->second.second;
    if (type == EXACT) {
      if (queryPattern == postfixString) {
        lineNumberNamePairs.push_back({ itr->first, itr->second.first });
      }
    } else if (type == PARTIAL) {
      if (isPostfixSubstring(postfixString, queryPattern)) {
        lineNumberNamePairs.push_back({ itr->first, itr->second.first });
      }
    } else if (type == ANY) {
      lineNumberNamePairs.push_back({ itr->first, itr->second.first });
    }
  }

  queryResult.setLineNumberNamePairs(lineNumberNamePairs);
  return queryResult;
}

spa::QueryResult spa::PatternStorage::getAssignVarName(PKBQueryArg lhs, Pattern rhs) {
  PatternType type = rhs.getType();
  std::string queryPattern = UtilsFunction::infixToPostfix(rhs.getValue());
  std::string varName = lhs.getName().name;
  QueryResult queryResult;
  queryResult.setQueryResultType(TUPLE);

  std::vector<std::pair<int, std::string>> lineNumberNamePairs;
  for (auto& itr = assignTable.begin(); itr != assignTable.end(); itr++) {
    if (itr->second.first != varName) {
      continue;
    }
    std::string postfixString = itr->second.second;
    if (type == EXACT) {
      if (queryPattern == postfixString) {
        lineNumberNamePairs.push_back({ itr->first, itr->second.first });
      }
    } else if (type == PARTIAL) {
      if (isPostfixSubstring(postfixString, queryPattern)) {
        lineNumberNamePairs.push_back({ itr->first, itr->second.first });
      }
    } else if (type == ANY) {
      lineNumberNamePairs.push_back({ itr->first, itr->second.first });
    }
  }

  queryResult.setLineNumberNamePairs(lineNumberNamePairs);
  return queryResult;
}

spa::QueryResult spa::PatternStorage::getPatternIfUnderscore(PKBQueryArg firstArg) {
  QueryResult queryResult;
  queryResult.setQueryResultType(TUPLE);

  std::vector<std::pair<int, std::string>> lineNumberNamePairs;
  for (auto& itr = patternIfTable.begin(); itr != patternIfTable.end(); itr++) {
    for (auto& varName : itr->second) {
      lineNumberNamePairs.push_back({ itr->first, varName });
    }
  }

  queryResult.setLineNumberNamePairs(lineNumberNamePairs);
  return queryResult;
}

spa::QueryResult spa::PatternStorage::getPatternIfVar(PKBQueryArg firstArg) {
  QueryResult queryResult;
  queryResult.setQueryResultType(TUPLE);

  std::vector<std::pair<int, std::string>> lineNumberNamePairs;
  for (auto& itr = patternIfTable.begin(); itr != patternIfTable.end(); itr++) {
    for (auto& varName : itr->second) {
      lineNumberNamePairs.push_back({ itr->first, varName });
    }
  }

  queryResult.setLineNumberNamePairs(lineNumberNamePairs);
  return queryResult;
}

spa::QueryResult spa::PatternStorage::getPatternIfVarName(PKBQueryArg firstArg) {
  std::string varName = firstArg.getName().name;
  QueryResult queryResult;
  queryResult.setQueryResultType(TUPLE);

  std::vector<std::pair<int, std::string>> lineNumberNamePairs;
  for (auto& itr = patternIfTable.begin(); itr != patternIfTable.end(); itr++) {
    if (itr->second.find(varName) == itr->second.end()) {
      continue;
    }
    lineNumberNamePairs.push_back({ itr->first, varName });
  }

  queryResult.setLineNumberNamePairs(lineNumberNamePairs);
  return queryResult;
}

spa::QueryResult spa::PatternStorage::getPatternWhileUnderscore(PKBQueryArg firstArg) {
  QueryResult queryResult;
  queryResult.setQueryResultType(TUPLE);

  std::vector<std::pair<int, std::string>> lineNumberNamePairs;
  for (auto& itr = patternWhileTable.begin(); itr != patternWhileTable.end(); itr++) {
    for (auto& varName : itr->second) {
      lineNumberNamePairs.push_back({ itr->first, varName });
    }
  }

  queryResult.setLineNumberNamePairs(lineNumberNamePairs);
  return queryResult;
}

spa::QueryResult spa::PatternStorage::getPatternWhileVar(PKBQueryArg firstArg) {
  QueryResult queryResult;
  queryResult.setQueryResultType(TUPLE);

  std::vector<std::pair<int, std::string>> lineNumberNamePairs;
  for (auto& itr = patternWhileTable.begin(); itr != patternWhileTable.end(); itr++) {
    for (auto& varName : itr->second) {
      lineNumberNamePairs.push_back({ itr->first, varName });
    }
  }

  queryResult.setLineNumberNamePairs(lineNumberNamePairs);
  return queryResult;
}

spa::QueryResult spa::PatternStorage::getPatternWhileVarName(PKBQueryArg firstArg) {
  std::string varName = firstArg.getName().name;
  QueryResult queryResult;
  queryResult.setQueryResultType(TUPLE);

  std::vector<std::pair<int, std::string>> lineNumberNamePairs;
  for (auto& itr = patternWhileTable.begin(); itr != patternWhileTable.end(); itr++) {
    if (itr->second.find(varName) == itr->second.end()) {
      continue;
    }
    lineNumberNamePairs.push_back({ itr->first, varName });
  }

  queryResult.setLineNumberNamePairs(lineNumberNamePairs);
  return queryResult;
}

void spa::PatternStorage::setAssignTable(std::unordered_map<int, std::pair<std::string, std::string>> assignTable) {
  this->assignTable = assignTable;
}

void spa::PatternStorage::setPatternIfTable(std::unordered_map<int, std::unordered_set<std::string>> patternIfTable) {
  this->patternIfTable = patternIfTable;
}

void spa::PatternStorage::setPatternWhileTable(std::unordered_map<int,
                                               std::unordered_set<std::string>> patternWhileTable) {
  this->patternWhileTable = patternWhileTable;
}
