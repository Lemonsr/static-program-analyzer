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

const std::vector<std::pair<int, std::string>>& spa::QueryResult::getLineNumberNamePairs() {
  return lineNumberNamePairs.value();
}

const std::vector<std::pair<int, int>>& spa::QueryResult::getLineNumberLineNumberPairs() {
  return lineNumberLineNumberPairs.value();
}

const std::vector<std::pair<std::string, std::string>>& spa::QueryResult::getNameNamePairs() {
  return nameNamePairs.value();
}

const std::vector<spa::CFGNode*>& spa::QueryResult::getCfgNodes() {
  return cfgNodes.value();
}

const bool& spa::QueryResult::getIsTrue() {
  return isTrue.value();
}

const std::unordered_map<int, std::unordered_set<int>>* spa::QueryResult::getIntToSetIntTable() {
    return intToSetIntTable.value();
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

void spa::QueryResult::setLineNumberNamePairs(
  std::vector<std::pair<int, std::string>> lineNumberNamePairs) {
  this->lineNumberNamePairs = lineNumberNamePairs;
}

void spa::QueryResult::setLineNumberLineNumberPairs(
  std::vector<std::pair<int, int>> lineNumberLineNumberPairs) {
  this->lineNumberLineNumberPairs = lineNumberLineNumberPairs;
}

void spa::QueryResult::setIsTrue(bool isTrue) {
  this->isTrue = isTrue;
}

void spa::QueryResult::setNameNamePairs(std::vector<std::pair<std::string,
                                                              std::string>>
  nameNamePairs) {
  this->nameNamePairs = nameNamePairs;
}

void spa::QueryResult::setCfgNodes(std::vector<spa::CFGNode*> cfgNodes) {
  this->cfgNodes = cfgNodes;
}

void spa::QueryResult::setCfgEndNodes(std::vector<int> cfgEndNodes) {
  this->cfgEndNodes = cfgEndNodes;
}

void spa::QueryResult::setIntToSetIntTable(std::unordered_map<int, std::unordered_set<int>>* intToSetIntTable) {
  this->intToSetIntTable = intToSetIntTable;
}
