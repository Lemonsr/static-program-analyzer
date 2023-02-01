#include "PatternStorage.h"

#include <string>
#include <vector>

#include "UtilsFunction.h"

bool spa::PatternStorage::addAssign(std::string lineNo, std::string varName,
  std::string postfixString) {
  int lineNumber = std::stoi(lineNo);
  if (assignTable.find(lineNumber) != assignTable.end()) {
    return false;
  }

  assignTable.insert({ lineNumber, { varName, postfixString } });
  return true;
}

spa::QueryResult spa::PatternStorage::getAssignUnderscore(PkbQueryArg lhs, Pattern rhs) {
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

spa::QueryResult spa::PatternStorage::getAssignVar(PkbQueryArg lhs, Pattern rhs) {
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

spa::QueryResult spa::PatternStorage::getAssignVarName(PkbQueryArg lhs, Pattern rhs) {
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
