#pragma once

#include "PKBManager.h"
#include "RelationshipStorage.h"
#include "EntityStorage.h"
#include "PatternStorage.h"
#include "CFGStorage.h"
#include "HashTuple.h"
#include "CFGNode.h"

#include <string>
#include <vector>
#include <tuple>
#include <functional>
#include <unordered_map>
#include <unordered_set>

namespace spa {

class PKB : public PKBManager {
 private:
  // Storage Tables
  RelationshipStorage relationshipStorage;
  EntityStorage entityStorage;
  PatternStorage patternStorage;
  CFGStorage cfgStorage;

  // Storage Maps
  std::unordered_map<
    std::tuple<RelationshipType, PKBQueryArgType, PKBQueryArgType>,
    std::function<QueryResult(RelationshipStorage& relationshipStorage, PKBQueryArg, PKBQueryArg)>,
    TupleHash,
    TupleEquality> relationshipQueryFunctionMap;

  std::unordered_map<
    DesignEntityType,
    std::function<QueryResult(EntityStorage& entityStorage)>> entityQueryFunctionMap;

  std::unordered_map<
    PKBQueryArgType,
    std::function<QueryResult(PatternStorage& patternStorage, PKBQueryArg, Pattern)>> patternQueryFunctionMap;

  std::unordered_map<
    std::tuple<DesignEntityType, PKBQueryArgType>,
    std::function<QueryResult(PatternStorage& patternStorage, PKBQueryArg)>,
    TupleHash,
    TupleEquality> patternContainerQueryFunctionMap;

  void createRelationshipQueryFunctionMap();
  void createEntityQueryFunctionMap();
  void createPatternQueryFunctionMap();
  void createPatternContainerQueryFunctionMap();

 public:
  PKB();
  const bool addRelationship(RelationshipType relationshipType,
                             std::string firstArg, std::string secondArg);
  const bool addEntity(DesignEntityType entityType, std::string arg);
  const bool addPattern(std::string lineNo, std::string lhs, std::string rhs);
  const bool addContainerPattern(DesignEntityType entityType, std::string lineNo, std::string varName);
  const bool addCallsContainerParent(std::string procName, std::string lineNo);
  const bool addCallsProc(int lineNumber, std::string procName);
  const bool addStatementType(std::string lineNo, StatementType statementType);
  const bool addStatementProc(std::string lineNo, std::string procName);

  // Node methods
  const bool addCfgEndNode(int lineNumber);
  const bool addEdge(int lineNumber1, int lineNumber2, RelationshipStorage& relationshipStorage);
  const bool addModifiedVariable(int lineNumber, std::string varName);
  const bool removeDummyNode();

  const QueryResult getRelationship(RelationshipType relationshipType,
                                    PKBQueryArg firstArg, PKBQueryArg secondArg);
  const QueryResult getEntity(DesignEntityType entityType);
  const QueryResult getPattern(PKBQueryArg lhs, Pattern rhs);
  const QueryResult getContainerPattern(DesignEntityType entityType, PKBQueryArg firstArg);
  const QueryResult getCallsContainerParent(std::string procName);
  const QueryResult getCallsProc();
  const QueryResult getCfgNode(int lineNumber);
};
}  // namespace spa
