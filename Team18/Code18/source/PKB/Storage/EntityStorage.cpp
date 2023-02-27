#include "EntityStorage.h"

#include <vector>

bool spa::EntityStorage::addVar(std::string varName) {
  if (varTable.find(varName) != varTable.end()) {
    return false;
  }

  varTable.insert(varName);
  return true;
}

bool spa::EntityStorage::addConst(std::string constName) {
  if (constTable.find(constName) != constTable.end()) {
    return false;
  }

  constTable.insert(constName);
  return true;
}

bool spa::EntityStorage::addProc(std::string procName) {
  if (procTable.find(procName) != procTable.end()) {
    return false;
  }

  procTable.insert(procName);
  return true;
}

spa::QueryResult spa::EntityStorage::getVars() {
  QueryResult queryResult;
  queryResult.setQueryResultType(TUPLE);

  std::vector<std::string> variableNames;
  for (auto& itr = varTable.begin(); itr != varTable.end(); itr++) {
    variableNames.push_back(*itr);
  }

  queryResult.setNames(variableNames);
  return queryResult;
}

spa::QueryResult spa::EntityStorage::getConsts() {
  QueryResult queryResult;
  queryResult.setQueryResultType(TUPLE);

  std::vector<std::string> constNames;
  for (auto& itr = constTable.begin(); itr != constTable.end(); itr++) {
    constNames.push_back(*itr);
  }

  queryResult.setNames(constNames);
  return queryResult;
}

spa::QueryResult spa::EntityStorage::getProcs() {
  QueryResult queryResult;
  queryResult.setQueryResultType(TUPLE);

  std::vector<std::string> procNames;
  for (auto& itr = procTable.begin(); itr != procTable.end(); itr++) {
    procNames.push_back(*itr);
  }

  queryResult.setNames(procNames);
  return queryResult;
}

void spa::EntityStorage::setVarTable(std::unordered_set<std::string> varTable) {
  this->varTable = varTable;
}

void spa::EntityStorage::setConstTable(std::unordered_set<std::string> constTable) {
  this->constTable = constTable;
}

void spa::EntityStorage::setProcTable(std::unordered_set<std::string> procTable) {
  this->procTable = procTable;
}
