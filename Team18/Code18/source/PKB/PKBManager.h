#pragma once

#include "ParsedQuery.h"
#include "PKBQueryArg.h"
#include "QueryResult.h"
#include "CFGNode.h"
#include "RelationshipStorage.h"

#include <string>
#include <unordered_set>

namespace spa {
class PKBManager {
 public:
  virtual const bool addRelationship(RelationshipType relationshipType,
                                     std::string firstArg, std::string secondArg) = 0;
  virtual const bool addEntity(DesignEntityType entityType, std::string arg) = 0;
  virtual const bool addPattern(std::string lineNo, std::string lhs, std::string rhs) = 0;
  virtual const bool addContainerPattern(DesignEntityType entityType, std::string lineNo,
                                         std::string varName) = 0;
  virtual const bool addCallsContainerParent(std::string procName, std::string lineNo) = 0;
  virtual const bool addCallsProc(int lineNumber, std::string procName) = 0;
  virtual const bool addStatementType(std::string lineNo, StatementType statementType) = 0;

  // Node methods
  virtual const bool addCfgNode(int lineNumber, spa::CFGNode cfgNode) = 0;
  virtual const bool addCfgEndNode(int lineNumber) = 0;
  virtual const bool addEdge(int lineNumberOne, int lineNumberTwo) = 0;
  virtual const bool addModifiedVariable(int lineNumber, std::string varName) = 0;
  virtual const bool removeDummyNode() = 0;

  // Next*, Affects, Affects* methods
  virtual const bool populateNextStar() = 0;
  virtual const bool populateAffects() = 0;
  virtual const bool populateAffectsStar() = 0;
  virtual const bool clearAll() = 0;  // Clears all Next*, Affects and Affects* tables

  virtual const QueryResult getRelationship(RelationshipType relationshipType,
                                            PKBQueryArg firstArg, PKBQueryArg secondArg) = 0;
  virtual const QueryResult getEntity(DesignEntityType entityType) = 0;
  virtual const QueryResult getPattern(PKBQueryArg lhs, Pattern rhs) = 0;
  virtual const QueryResult getContainerPattern(DesignEntityType entityType, PKBQueryArg firstArg) = 0;
  virtual const QueryResult getCallsContainerParent(std::string procName) = 0;
  virtual const QueryResult getCallsProc() = 0;
  virtual const QueryResult getCfgNode(int lineNumber) = 0;
  virtual const QueryResult getCfgEndNodes() = 0;
  virtual const QueryResult getAffectsTable() = 0;
};
}  // namespace spa
