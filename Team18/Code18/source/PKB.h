#pragma once

#include "PKBManager.h"
#include "RelationshipStorage.h"
#include "EntityStorage.h"
#include "PatternStorage.h"

#include <string>
#include <vector>
#include <tuple>
#include <functional>
#include <unordered_map>

namespace spa {
class TNode;

class PKB : public PKBManager {
 private:
  // Storage Tables
  RelationshipStorage relationshipStorage;
  EntityStorage entityStorage;
  PatternStorage patternStorage;

  // Storage Maps
  std::unordered_map<std::tuple<RelationshipType, PKBQueryArgType, PKBQueryArgType>,
    std::function<QueryResult(RelationshipStorage& relationshipStorage, PKBQueryArg, PKBQueryArg)>> relationshipQueryFunctionMap;
  std::unordered_map<DesignEntityType,
    std::function<QueryResult(EntityStorage& entityStorage)>> entityQueryFunctionMap;
  std::unordered_map<PKBQueryArgType,
    std::function<QueryResult(PatternStorage& patternStorage, PKBQueryArg, Pattern)>> patternQueryFunctionMap;

  void createRelationshipQueryFunctionMap();
  void createEntityQueryFunctionMap();
  void createPatternQueryFunctionMap();

public:
  PKB();
  const bool addRelationship(RelationshipType relationshipType, std::string arg1, std::string arg2);
  const bool addEntity(DesignEntityType entityType, std::string arg);
  const bool addPattern(std::string lineNo, std::string lhs, std::string rhs);
  const bool addStatementType(std::string lineNo, StatementType statementType);
  const bool addStatementProc(std::string lineNo, std::string procedure);
  const QueryResult getRelationship(RelationshipType relationshipType, PKBQueryArg arg1, PKBQueryArg arg2);
  const QueryResult getEntity(DesignEntityType entityType);  // if stmtTypes/stmtProcs, shld route to RelStorage
  const QueryResult getPattern(PKBQueryArg lhs);
};
}  // namespace spa
