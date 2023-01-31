#include "PKB.h"
#include "PKBManager.h"
#include "RelationshipStorage.h"
#include "EntityStorage.h"
#include "PatternStorage.h"
#include "QueryResult.h"

#include <string>
#include <vector>
#include <tuple>
#include <any>
#include <unordered_map>

const bool spa::PKB::addRelationship(RelationshipType relationshipType, std::string arg1, std::string arg2) {
  return false;
}

const bool spa::PKB::addEntity(DesignEntityType entityType, std::string arg) {
  return false;
}

const bool spa::PKB::addPattern(std::string lineNo, std::string lhs, std::string rhs) {
  return false;
}

const bool spa::PKB::addStatementType(std::string lineNo, StatementType statementType) {
  return false;
}

const bool spa::PKB::addStatementProc(std::string lineNo, std::string procedure) {
  return false;
}

const spa::QueryResult spa::PKB::getRelationship(RelationshipType relationshipType, PKBQueryArg arg1, PKBQueryArg arg2) {
  return QueryResult();
}

const spa::QueryResult spa::PKB::getEntity(DesignEntityType entityType) {
  return QueryResult();
}

const spa::QueryResult spa::PKB::getPattern(PKBQueryArg lhs) {
  return QueryResult();
}
