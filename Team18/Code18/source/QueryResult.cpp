#include "QueryResult.h"

const spa::QueryResultType& spa::QueryResult::getQueryResultType() {
  return queryResultType;
}

const std::vector<std::string>& spa::QueryResult::getVariableNames() {
  return variableNames.value();
}

const std::vector<int>& spa::QueryResult::getLineNumbers() {
  return lineNumbers.value();
}

const std::vector<std::pair<int, std::string>>& spa::QueryResult::getLineNumberVariablePairs() {
  return lineNumberVariablePairs.value();
}

const bool& spa::QueryResult::getIsTrue() {
  return isTrue.value();
}

void spa::QueryResult::setQueryResultType(QueryResultType queryResultType) {
  this->queryResultType = queryResultType;
}

void spa::QueryResult::setVariableNames(std::vector<std::string> variableNames) {
  this->variableNames = variableNames;
}

void spa::QueryResult::setLineNumbers(std::vector<int> lineNumbers) {
  this->lineNumbers = lineNumbers;
}

void spa::QueryResult::setLineNumberVariablePairs(std::vector<std::pair<int, std::string>> lineNumberVariablePairs) {
  this->lineNumberVariablePairs = lineNumberVariablePairs;
}

void spa::QueryResult::setIsTrue(bool isTrue) {
  this->isTrue = isTrue;
}
