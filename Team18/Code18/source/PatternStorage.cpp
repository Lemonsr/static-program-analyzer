#include "PatternStorage.h"

#include <string>

#include "UtilsFunction.h"

bool spa::PatternStorage::addAssign(std::string lineNo, std::string varName, std::string postfixString) {
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

  std::vector<int> lineNumbers;
  for (auto& itr = assignTable.begin(); itr != assignTable.end(); itr++) {
    std::string postfixString = itr->second.second;
    if (type == EXACT) {
      if (queryPattern == postfixString) {
        lineNumbers.push_back(itr->first);
      }
    }
    else if (type == PARTIAL) {
      if (postfixString.find(queryPattern) != std::string::npos) {
        lineNumbers.push_back(itr->first);
      }
    }
    else if (type == ANY) {
      lineNumbers.push_back(itr->first);
    }
  }

  queryResult.setLineNumbers(lineNumbers);
  return queryResult;
}

spa::QueryResult spa::PatternStorage::getAssignVar(PkbQueryArg lhs, Pattern rhs) {
  PatternType type = rhs.getType();
  std::string queryPattern = UtilsFunction::infixToPostfix(rhs.getValue());
  QueryResult queryResult;
  queryResult.setQueryResultType(TUPLE);

  std::vector<int> lineNumbers;
  for (auto& itr = assignTable.begin(); itr != assignTable.end(); itr++) {
    std::string postfixString = itr->second.second;
    if (type == EXACT) {
      if (queryPattern == postfixString) {
        lineNumbers.push_back(itr->first);
      }
    }
    else if (type == PARTIAL) {
      if (postfixString.find(queryPattern) != std::string::npos) {
        lineNumbers.push_back(itr->first);
      }
    }
    else if (type == ANY) {
      lineNumbers.push_back(itr->first);
    }
  }

  queryResult.setLineNumbers(lineNumbers);
  return queryResult;
}

spa::QueryResult spa::PatternStorage::getAssignVarName(PkbQueryArg lhs, Pattern rhs) {
  PatternType type = rhs.getType();
  std::string queryPattern = UtilsFunction::infixToPostfix(rhs.getValue());
  std::string varName = lhs.getName().name;
  QueryResult queryResult;
  queryResult.setQueryResultType(TUPLE);

  std::vector<int> lineNumbers;
  for (auto& itr = assignTable.begin(); itr != assignTable.end(); itr++) {
    if (itr->second.first != varName) {
      continue;
    }
    std::string postfixString = itr->second.second;
    if (type == EXACT) {
      if (queryPattern == postfixString) {
        lineNumbers.push_back(itr->first);
      }
    }
    else if (type == PARTIAL) {
      if (postfixString.find(queryPattern) != std::string::npos) {
        lineNumbers.push_back(itr->first);
      }
    }
    else if (type == ANY) {
      lineNumbers.push_back(itr->first);
    }
  }

  queryResult.setLineNumbers(lineNumbers);
  return queryResult;
}
