#include "QueryResult.h"

const spa::QueryResultType& spa::QueryResult::getQueryResultType() {
  return queryResultType;
}

const std::vector<std::string>& spa::QueryResult::getNames() {
  return names.value();
}

const std::vector<int>& spa::QueryResult::getLineNumbers() {
  return lineNumbers.value();
}

const std::vector<std::pair<int, std::string>>& spa::QueryResult::getLineNumberVariablePairs() {
  return lineNumberVariablePairs.value();
}

const std::vector<std::pair<int, int>>& spa::QueryResult::getLineNumberLineNumberPairs() {
  return lineNumberLineNumberPairs.value();
}

const std::vector<std::pair<std::string, std::string>>& spa::QueryResult::getProcNameProcNamePairs() {
  return procNameProcNamePairs.value();
}

const bool& spa::QueryResult::getIsTrue() {
  return isTrue.value();
}

void spa::QueryResult::setQueryResultType(QueryResultType queryResultType) {
  this->queryResultType = queryResultType;
}

void spa::QueryResult::setNames(std::vector<std::string> names) {
  this->names = names;
}

void spa::QueryResult::setLineNumbers(std::vector<int> lineNumbers) {
  this->lineNumbers = lineNumbers;
}

void spa::QueryResult::setLineNumberVariablePairs(std::vector<std::pair<int, std::string>> lineNumberVariablePairs) {
  this->lineNumberVariablePairs = lineNumberVariablePairs;
}

void spa::QueryResult::setLineNumberLineNumberPairs(std::vector<std::pair<int, int>> lineNumberLineNumberPairs) {
  this->lineNumberLineNumberPairs = lineNumberLineNumberPairs;
}

void spa::QueryResult::setIsTrue(bool isTrue) {
  this->isTrue = isTrue;
}

void spa::QueryResult::setProcNameProcNamePairs(std::vector<std::pair<std::string, std::string>> procNameProcNamePairs) {
  this->procNameProcNamePairs = procNameProcNamePairs;
}
