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

/** Maps argument type combinations to its respective patterns. */
std::unordered_map<spa::PKBQueryArgType, std::any> patternMap;

const bool spa::PKB::addRelationship(RelationshipType relationshipType,
                                     std::string arg1, std::string arg2) {
  switch (relationshipType) {
  case FOLLOWS: {
    return relationshipStorage.addFollows(arg1, arg2);
  }
  case FOLLOWS_STAR: {
    return relationshipStorage.addFollowsStar(arg1, arg2);
  }
  case PARENT: {
    return relationshipStorage.addParent(arg1, arg2);
  }
  case PARENT_STAR: {
    return relationshipStorage.addParentStar(arg1, arg2);
  }
  case MODIFIES: {
    return relationshipStorage.addModifies(arg1, arg2);
  }
  case USES: {
    return relationshipStorage.addUses(arg1, arg2);
  }
  default: {
    return false;
  }
  }
  return false;
}

const bool spa::PKB::addEntity(DesignEntityType entityType, std::string arg) {
  switch (entityType) {
  case VARIABLE: {
    return entityStorage.addVar(arg);
  }
  case CONSTANT: {
    return entityStorage.addConst(arg);
  }
  case PROCEDURE: {
    return entityStorage.addProc(arg);
  }
  default: {
    return false;
  }
  }
  return false;
}

const bool spa::PKB::addPattern(std::string lineNo, std::string lhs, std::string rhs) {
  return patternStorage.addAssign(lineNo, lhs, rhs);
}

const bool spa::PKB::addStatementType(std::string lineNo, StatementType statementType) {
  return relationshipStorage.addStatementType(lineNo, statementType);
}

const bool spa::PKB::addStatementProc(std::string lineNo, std::string procedure) {
  return relationshipStorage.addStatementProc(lineNo, procedure);
}

const spa::QueryResult spa::PKB::getRelationship(RelationshipType relationshipType,
                                                 PKBQueryArg arg1, PKBQueryArg arg2) {
  return QueryResult();
}

const spa::QueryResult spa::PKB::getEntity(DesignEntityType entityType) {
  return QueryResult();
}

const spa::QueryResult spa::PKB::getPattern(PKBQueryArg lhs) {
  return QueryResult();
}
