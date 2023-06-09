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
    std::function<QueryResult(RelationshipStorage&, PKBQueryArg, PKBQueryArg)>,
    TupleHash,
    TupleEquality>
  relationshipQueryFunctionMap;

  std::unordered_map<
    DesignEntityType,
    std::function<QueryResult(EntityStorage&)>>
  entityQueryFunctionMap;

  std::unordered_map<
    PKBQueryArgType,
    std::function<QueryResult(PatternStorage&, PKBQueryArg, Pattern)>>
  patternQueryFunctionMap;

  std::unordered_map<
    std::tuple<DesignEntityType, PKBQueryArgType>,
    std::function<QueryResult(PatternStorage&, PKBQueryArg)>,
    TupleHash,
    TupleEquality>
  patternContainerQueryFunctionMap;

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
  const bool addContainerPattern(DesignEntityType entityType, std::string lineNo,
                                 std::string varName);
  const bool addCallsContainerParent(std::string procName, std::string lineNo);
  const bool addCallsProc(int lineNumber, std::string procName);
  const bool addStatementType(std::string lineNo, StatementType statementType);
  const bool setAffectsTable(std::unordered_map<int, std::unordered_set<int>> affectsTable);

  // Node methods
  const bool addCfgNode(int lineNo, spa::CFGNode cfgNode);
  const bool addCfgEndNode(int lineNumber);
  const bool addEdge(int lineNumberOne, int lineNumberTwo);
  const bool addModifiedVariable(int lineNumber, std::string varName);
  const bool addUsesVariable(int lineNumber, std::string varName);

  const bool populateNextStar();
  const bool populateAffects();
  const bool populateAffectsStar();
  const bool clearAll();

  const QueryResult getRelationship(RelationshipType relationshipType,
                                    PKBQueryArg firstArg, PKBQueryArg secondArg);
  const QueryResult getEntity(DesignEntityType entityType);
  const QueryResult getPattern(PKBQueryArg lhs, Pattern rhs);
  const QueryResult getContainerPattern(DesignEntityType entityType, PKBQueryArg firstArg);
  const QueryResult getCallsContainerParent(std::string procName);
  const QueryResult getCallsProc();
  const QueryResult getCfgNode(int lineNumber);
  const QueryResult getCfgEndNodes();
  const QueryResult getNextTable();
  const QueryResult getAffectsTable();
};
}  // namespace spa
