#pragma once

#include "ParsedQuery.h"
#include "PKBQueryArg.h"
#include "QueryResult.h"

#include <string>

namespace spa {
class PKBManager {
public:
  virtual const bool addRelationship(RelationshipType relationshipType, std::string firstArg, std::string secondArg) = 0;
  virtual const bool addEntity(DesignEntityType entityType, std::string arg) = 0;
  virtual const bool addPattern(std::string lineNo, std::string lhs, std::string rhs) = 0;
  virtual const bool addStatementType(std::string lineNo, StatementType statementType) = 0;
  virtual const bool addStatementProc(std::string lineNo, std::string procedure) = 0;
  virtual const QueryResult getRelationship(RelationshipType relationshipType, PKBQueryArg firstArg, PKBQueryArg secondArg) = 0;
  virtual const QueryResult getEntity(DesignEntityType entityType) = 0;
  virtual const QueryResult getPattern(PKBQueryArg lhs, Pattern rhs) = 0;
};
}  // namespace spa
