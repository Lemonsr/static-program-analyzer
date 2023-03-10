#include "PKB.h"
#include "PKBQueryTypes.h"

#include <unordered_set>

void spa::PKB::createRelationshipQueryFunctionMap() {
  relationshipQueryFunctionMap = {
    // Follows
    {{RelationshipType::FOLLOWS, PKBQueryArgType::LINE_NUMBER, PKBQueryArgType::LINE_NUMBER},
      &RelationshipStorage::getFollowsLineLine},
    {{RelationshipType::FOLLOWS, PKBQueryArgType::LINE_NUMBER, PKBQueryArgType::STATEMENT},
      &RelationshipStorage::getFollowsLineStatement},
    {{RelationshipType::FOLLOWS, PKBQueryArgType::STATEMENT, PKBQueryArgType::LINE_NUMBER},
      &RelationshipStorage::getFollowsStatementLine},
    {{RelationshipType::FOLLOWS, PKBQueryArgType::LINE_NUMBER, PKBQueryArgType::UNDERSCORE},
      &RelationshipStorage::getFollowsLineUnderscore},
    {{RelationshipType::FOLLOWS, PKBQueryArgType::UNDERSCORE, PKBQueryArgType::LINE_NUMBER},
      &RelationshipStorage::getFollowsUnderscoreLine},
    {{RelationshipType::FOLLOWS, PKBQueryArgType::STATEMENT, PKBQueryArgType::STATEMENT},
      &RelationshipStorage::getFollowsStatementStatement},
    {{RelationshipType::FOLLOWS, PKBQueryArgType::STATEMENT, PKBQueryArgType::UNDERSCORE},
      &RelationshipStorage::getFollowsStatementUnderscore},
    {{RelationshipType::FOLLOWS, PKBQueryArgType::UNDERSCORE, PKBQueryArgType::STATEMENT},
      &RelationshipStorage::getFollowsUnderscoreStatement},
    {{RelationshipType::FOLLOWS, PKBQueryArgType::UNDERSCORE, PKBQueryArgType::UNDERSCORE},
      &RelationshipStorage::getFollowsUnderscoreUnderscore},

    // Follows Star
    {{RelationshipType::FOLLOWS_STAR, PKBQueryArgType::LINE_NUMBER, PKBQueryArgType::LINE_NUMBER},
      &RelationshipStorage::getFollowsStarLineLine},
    {{RelationshipType::FOLLOWS_STAR, PKBQueryArgType::LINE_NUMBER, PKBQueryArgType::STATEMENT},
      &RelationshipStorage::getFollowsStarLineStatement},
    {{RelationshipType::FOLLOWS_STAR, PKBQueryArgType::STATEMENT, PKBQueryArgType::LINE_NUMBER},
      &RelationshipStorage::getFollowsStarStatementLine},
    {{RelationshipType::FOLLOWS_STAR, PKBQueryArgType::LINE_NUMBER, PKBQueryArgType::UNDERSCORE},
      &RelationshipStorage::getFollowsStarLineUnderscore},
    {{RelationshipType::FOLLOWS_STAR, PKBQueryArgType::UNDERSCORE, PKBQueryArgType::LINE_NUMBER},
      &RelationshipStorage::getFollowsStarUnderscoreLine},
    {{RelationshipType::FOLLOWS_STAR, PKBQueryArgType::STATEMENT, PKBQueryArgType::STATEMENT},
      &RelationshipStorage::getFollowsStarStatementStatement},
    {{RelationshipType::FOLLOWS_STAR, PKBQueryArgType::STATEMENT, PKBQueryArgType::UNDERSCORE},
      &RelationshipStorage::getFollowsStarStatementUnderscore},
    {{RelationshipType::FOLLOWS_STAR, PKBQueryArgType::UNDERSCORE, PKBQueryArgType::STATEMENT},
      &RelationshipStorage::getFollowsStarUnderscoreStatement},
    {{RelationshipType::FOLLOWS_STAR, PKBQueryArgType::UNDERSCORE, PKBQueryArgType::UNDERSCORE},
      &RelationshipStorage::getFollowsStarUnderscoreUnderscore},

    // Parent
    {{RelationshipType::PARENT, PKBQueryArgType::LINE_NUMBER, PKBQueryArgType::LINE_NUMBER},
      &RelationshipStorage::getParentLineLine},
    {{RelationshipType::PARENT, PKBQueryArgType::LINE_NUMBER, PKBQueryArgType::STATEMENT},
      &RelationshipStorage::getParentLineStatement},
    {{RelationshipType::PARENT, PKBQueryArgType::STATEMENT, PKBQueryArgType::LINE_NUMBER},
      &RelationshipStorage::getParentStatementLine},
    {{RelationshipType::PARENT, PKBQueryArgType::LINE_NUMBER, PKBQueryArgType::UNDERSCORE},
      &RelationshipStorage::getParentLineUnderscore},
    {{RelationshipType::PARENT, PKBQueryArgType::UNDERSCORE, PKBQueryArgType::LINE_NUMBER},
      &RelationshipStorage::getParentUnderscoreLine},
    {{RelationshipType::PARENT, PKBQueryArgType::STATEMENT, PKBQueryArgType::STATEMENT},
      &RelationshipStorage::getParentStatementStatement},
    {{RelationshipType::PARENT, PKBQueryArgType::STATEMENT, PKBQueryArgType::UNDERSCORE},
      &RelationshipStorage::getParentStatementUnderscore},
    {{RelationshipType::PARENT, PKBQueryArgType::UNDERSCORE, PKBQueryArgType::STATEMENT},
      &RelationshipStorage::getParentUnderscoreStatement},
    {{RelationshipType::PARENT, PKBQueryArgType::UNDERSCORE, PKBQueryArgType::UNDERSCORE},
      &RelationshipStorage::getParentUnderscoreUnderscore},

    // Parent Star
    {{RelationshipType::PARENT_STAR, PKBQueryArgType::LINE_NUMBER, PKBQueryArgType::LINE_NUMBER},
      &RelationshipStorage::getParentStarLineLine},
    {{RelationshipType::PARENT_STAR, PKBQueryArgType::LINE_NUMBER, PKBQueryArgType::STATEMENT},
      &RelationshipStorage::getParentStarLineStatement},
    {{RelationshipType::PARENT_STAR, PKBQueryArgType::STATEMENT, PKBQueryArgType::LINE_NUMBER},
      &RelationshipStorage::getParentStarStatementLine},
    {{RelationshipType::PARENT_STAR, PKBQueryArgType::LINE_NUMBER, PKBQueryArgType::UNDERSCORE},
      &RelationshipStorage::getParentStarLineUnderscore},
    {{RelationshipType::PARENT_STAR, PKBQueryArgType::UNDERSCORE, PKBQueryArgType::LINE_NUMBER},
      &RelationshipStorage::getParentStarUnderscoreLine},
    {{RelationshipType::PARENT_STAR, PKBQueryArgType::STATEMENT, PKBQueryArgType::STATEMENT},
      &RelationshipStorage::getParentStarStatementStatement},
    {{RelationshipType::PARENT_STAR, PKBQueryArgType::STATEMENT, PKBQueryArgType::UNDERSCORE},
      &RelationshipStorage::getParentStarStatementUnderscore},
    {{RelationshipType::PARENT_STAR, PKBQueryArgType::UNDERSCORE, PKBQueryArgType::STATEMENT},
      &RelationshipStorage::getParentStarUnderscoreStatement},
    {{RelationshipType::PARENT_STAR, PKBQueryArgType::UNDERSCORE, PKBQueryArgType::UNDERSCORE},
      &RelationshipStorage::getParentStarUnderscoreUnderscore},

    // Uses
    {{RelationshipType::USES, PKBQueryArgType::LINE_NUMBER, PKBQueryArgType::NAME},
      &RelationshipStorage::getUsesLineVarName},
    {{RelationshipType::USES, PKBQueryArgType::LINE_NUMBER, PKBQueryArgType::UNDERSCORE},
      &RelationshipStorage::getUsesLineUnderscore},
    {{RelationshipType::USES, PKBQueryArgType::LINE_NUMBER, PKBQueryArgType::VARIABLE},
      &RelationshipStorage::getUsesLineVar},
    {{RelationshipType::USES, PKBQueryArgType::STATEMENT, PKBQueryArgType::NAME},
      &RelationshipStorage::getUsesStmtVarName},
    {{RelationshipType::USES, PKBQueryArgType::STATEMENT, PKBQueryArgType::UNDERSCORE},
      &RelationshipStorage::getUsesStmtUnderscore},
    {{RelationshipType::USES, PKBQueryArgType::STATEMENT, PKBQueryArgType::VARIABLE},
      &RelationshipStorage::getUsesStmtVar},

    // Uses Proc
    {{RelationshipType::USES, PKBQueryArgType::PROCEDURE, PKBQueryArgType::NAME},
      &RelationshipStorage::getUsesProcedureVarName},
    {{RelationshipType::USES, PKBQueryArgType::PROCEDURE, PKBQueryArgType::UNDERSCORE},
      &RelationshipStorage::getUsesProcedureUnderscore},
    {{RelationshipType::USES, PKBQueryArgType::PROCEDURE, PKBQueryArgType::VARIABLE},
      &RelationshipStorage::getUsesProcedureVar},
    {{RelationshipType::USES, PKBQueryArgType::NAME, PKBQueryArgType::NAME},
      &RelationshipStorage::getUsesProcNameVarName},
    {{RelationshipType::USES, PKBQueryArgType::NAME, PKBQueryArgType::UNDERSCORE},
      &RelationshipStorage::getUsesProcNameUnderscore},
    {{RelationshipType::USES, PKBQueryArgType::NAME, PKBQueryArgType::VARIABLE},
      &RelationshipStorage::getUsesProcNameVar},

    // Modifies
    {{RelationshipType::MODIFIES, PKBQueryArgType::LINE_NUMBER, PKBQueryArgType::NAME},
      &RelationshipStorage::getModifiesLineVarName},
    {{RelationshipType::MODIFIES, PKBQueryArgType::LINE_NUMBER, PKBQueryArgType::UNDERSCORE},
      &RelationshipStorage::getModifiesLineUnderscore},
    {{RelationshipType::MODIFIES, PKBQueryArgType::LINE_NUMBER, PKBQueryArgType::VARIABLE},
      &RelationshipStorage::getModifiesLineVar},
    {{RelationshipType::MODIFIES, PKBQueryArgType::STATEMENT, PKBQueryArgType::NAME},
      &RelationshipStorage::getModifiesStmtVarName},
    {{RelationshipType::MODIFIES, PKBQueryArgType::STATEMENT, PKBQueryArgType::UNDERSCORE},
      &RelationshipStorage::getModifiesStmtUnderscore},
    {{RelationshipType::MODIFIES, PKBQueryArgType::STATEMENT, PKBQueryArgType::VARIABLE},
      &RelationshipStorage::getModifiesStmtVar},

    // Modifies Proc
    {{RelationshipType::MODIFIES, PKBQueryArgType::PROCEDURE, PKBQueryArgType::NAME},
      &RelationshipStorage::getModifiesProcedureVarName},
    {{RelationshipType::MODIFIES, PKBQueryArgType::PROCEDURE, PKBQueryArgType::UNDERSCORE},
      &RelationshipStorage::getModifiesProcedureUnderscore},
    {{RelationshipType::MODIFIES, PKBQueryArgType::PROCEDURE, PKBQueryArgType::VARIABLE},
      &RelationshipStorage::getModifiesProcedureVar},
    {{RelationshipType::MODIFIES, PKBQueryArgType::NAME, PKBQueryArgType::NAME},
      &RelationshipStorage::getModifiesProcNameVarName},
    {{RelationshipType::MODIFIES, PKBQueryArgType::NAME, PKBQueryArgType::UNDERSCORE},
      &RelationshipStorage::getModifiesProcNameUnderscore},
    {{RelationshipType::MODIFIES, PKBQueryArgType::NAME, PKBQueryArgType::VARIABLE},
      &RelationshipStorage::getModifiesProcNameVar},

    // Calls
    {{RelationshipType::CALLS, PKBQueryArgType::NAME, PKBQueryArgType::NAME},
      &RelationshipStorage::getCallsNameName},
    {{RelationshipType::CALLS, PKBQueryArgType::NAME, PKBQueryArgType::UNDERSCORE},
      &RelationshipStorage::getCallsNameUnderscore},
    {{RelationshipType::CALLS, PKBQueryArgType::NAME, PKBQueryArgType::PROCEDURE},
      &RelationshipStorage::getCallsNameProcedure},
    {{RelationshipType::CALLS, PKBQueryArgType::UNDERSCORE, PKBQueryArgType::NAME},
      &RelationshipStorage::getCallsUnderscoreName},
    {{RelationshipType::CALLS, PKBQueryArgType::UNDERSCORE, PKBQueryArgType::UNDERSCORE},
      &RelationshipStorage::getCallsUnderscoreUnderscore},
    {{RelationshipType::CALLS, PKBQueryArgType::UNDERSCORE, PKBQueryArgType::PROCEDURE},
      & RelationshipStorage::getCallsUnderscoreProcedure},
    {{RelationshipType::CALLS, PKBQueryArgType::PROCEDURE, PKBQueryArgType::NAME},
      &RelationshipStorage::getCallsProcedureName},
    {{RelationshipType::CALLS, PKBQueryArgType::PROCEDURE, PKBQueryArgType::UNDERSCORE},
      &RelationshipStorage::getCallsProcedureUnderscore},
    {{RelationshipType::CALLS, PKBQueryArgType::PROCEDURE, PKBQueryArgType::PROCEDURE},
      &RelationshipStorage::getCallsProcedureProcedure},

    // Calls Star
    {{RelationshipType::CALLS_STAR, PKBQueryArgType::NAME, PKBQueryArgType::NAME},
      &RelationshipStorage::getCallsStarNameName},
    {{RelationshipType::CALLS_STAR, PKBQueryArgType::NAME, PKBQueryArgType::UNDERSCORE},
      &RelationshipStorage::getCallsStarNameUnderscore},
    {{RelationshipType::CALLS_STAR, PKBQueryArgType::NAME, PKBQueryArgType::PROCEDURE},
      &RelationshipStorage::getCallsStarNameProcedure},
    {{RelationshipType::CALLS_STAR, PKBQueryArgType::UNDERSCORE, PKBQueryArgType::NAME},
      &RelationshipStorage::getCallsStarUnderscoreName},
    {{RelationshipType::CALLS_STAR, PKBQueryArgType::UNDERSCORE, PKBQueryArgType::UNDERSCORE},
      &RelationshipStorage::getCallsStarUnderscoreUnderscore},
    {{RelationshipType::CALLS_STAR, PKBQueryArgType::UNDERSCORE, PKBQueryArgType::PROCEDURE},
      & RelationshipStorage::getCallsStarUnderscoreProcedure},
    {{RelationshipType::CALLS_STAR, PKBQueryArgType::PROCEDURE, PKBQueryArgType::NAME},
      &RelationshipStorage::getCallsStarProcedureName},
    {{RelationshipType::CALLS_STAR, PKBQueryArgType::PROCEDURE, PKBQueryArgType::UNDERSCORE},
      &RelationshipStorage::getCallsStarProcedureUnderscore},
    {{RelationshipType::CALLS_STAR, PKBQueryArgType::PROCEDURE, PKBQueryArgType::PROCEDURE},
      &RelationshipStorage::getCallsStarProcedureProcedure}
  };
}

void spa::PKB::createEntityQueryFunctionMap() {
  entityQueryFunctionMap = {
    {DesignEntityType::VARIABLE, &EntityStorage::getVars},
    {DesignEntityType::CONSTANT, &EntityStorage::getConsts},
    {DesignEntityType::PROCEDURE, &EntityStorage::getProcs}
  };
}

void spa::PKB::createPatternQueryFunctionMap() {
  patternQueryFunctionMap = {
    {PKBQueryArgType::UNDERSCORE, &PatternStorage::getAssignUnderscore},
    {PKBQueryArgType::VARIABLE, &PatternStorage::getAssignVar},
    {PKBQueryArgType::NAME, &PatternStorage::getAssignVarName}
  };
}

void spa::PKB::createPatternContainerQueryFunctionMap() {
  patternContainerQueryFunctionMap = {
    {{DesignEntityType::IF, PKBQueryArgType::UNDERSCORE }, &PatternStorage::getPatternIfUnderscore},
    {{DesignEntityType::IF, PKBQueryArgType::VARIABLE }, &PatternStorage::getPatternIfVar},
    {{DesignEntityType::IF, PKBQueryArgType::NAME }, &PatternStorage::getPatternIfVarName},
    {{DesignEntityType::WHILE, PKBQueryArgType::UNDERSCORE }, &PatternStorage::getPatternWhileUnderscore},
    {{DesignEntityType::WHILE, PKBQueryArgType::VARIABLE }, &PatternStorage::getPatternWhileVar},
    {{DesignEntityType::WHILE, PKBQueryArgType::NAME }, &PatternStorage::getPatternWhileVarName}
  };
}

void spa::PKB::createCfgQueryFunctionMap() {
  cfgQueryFunctionMap = {
    // Next
    {{RelationshipType::NEXT, PKBQueryArgType::LINE_NUMBER, PKBQueryArgType::LINE_NUMBER},
      &CFGStorage::getNextLineLine},
    {{RelationshipType::NEXT, PKBQueryArgType::LINE_NUMBER, PKBQueryArgType::STATEMENT},
      &CFGStorage::getNextLineStatement},
    {{RelationshipType::NEXT, PKBQueryArgType::STATEMENT, PKBQueryArgType::LINE_NUMBER},
      &CFGStorage::getNextStatementLine},
    {{RelationshipType::NEXT, PKBQueryArgType::LINE_NUMBER, PKBQueryArgType::UNDERSCORE},
      &CFGStorage::getNextLineUnderscore},
    {{RelationshipType::NEXT, PKBQueryArgType::UNDERSCORE, PKBQueryArgType::LINE_NUMBER},
      &CFGStorage::getNextUnderscoreLine},
    {{RelationshipType::NEXT, PKBQueryArgType::STATEMENT, PKBQueryArgType::STATEMENT},
      &CFGStorage::getNextStatementStatement},
    {{RelationshipType::NEXT, PKBQueryArgType::STATEMENT, PKBQueryArgType::UNDERSCORE},
      &CFGStorage::getNextStatementUnderscore},
    {{RelationshipType::NEXT, PKBQueryArgType::UNDERSCORE, PKBQueryArgType::STATEMENT},
      &CFGStorage::getNextUnderscoreStatement},
    {{RelationshipType::NEXT, PKBQueryArgType::UNDERSCORE, PKBQueryArgType::UNDERSCORE},
      &CFGStorage::getNextUnderscoreUnderscore}
  };
}

spa::PKB::PKB() : cfgStorage(relationshipStorage) {
  createRelationshipQueryFunctionMap();
  createEntityQueryFunctionMap();
  createPatternQueryFunctionMap();
  createPatternContainerQueryFunctionMap();
  createCfgQueryFunctionMap();
}

const bool spa::PKB::addRelationship(RelationshipType relationshipType,
  std::string firstArg, std::string secondArg) {
  switch (relationshipType) {
  case FOLLOWS: {
    return relationshipStorage.addFollows(firstArg, secondArg);
  }
  case FOLLOWS_STAR: {
    return relationshipStorage.addFollowsStar(firstArg, secondArg);
  }
  case PARENT: {
    return relationshipStorage.addParent(firstArg, secondArg);
  }
  case PARENT_STAR: {
    return relationshipStorage.addParentStar(firstArg, secondArg);
  }
  case MODIFIES: {
    return relationshipStorage.addModifies(firstArg, secondArg);
  }
  case MODIFIES_P: {
    return relationshipStorage.addModifiesProc(firstArg, secondArg);
  }
  case USES: {
    return relationshipStorage.addUses(firstArg, secondArg);
  }
  case USES_P: {
    return relationshipStorage.addUsesProc(firstArg, secondArg);
  }
  case CALLS: {
    return relationshipStorage.addCalls(firstArg, secondArg);
  }
  case CALLS_STAR: {
    return relationshipStorage.addCallsStar(firstArg, secondArg);
  }
  case NEXT: {
    return cfgStorage.addNext(firstArg, secondArg);
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
  case STMT:
  case READ:
  case PRINT:
  case ASSIGN:
  case CALL:
  case WHILE:
  case IF: {
    auto statementTypeItr = statementTypeMap.find(entityType);
    return addStatementType(arg, statementTypeItr->second.value());
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

const bool spa::PKB::addContainerPattern(DesignEntityType entityType, std::string lineNo, std::string varName) {
  switch (entityType) {
  case IF: {
    return patternStorage.addPatternIf(lineNo, varName);
  }
  case WHILE: {
    return patternStorage.addPatternWhile(lineNo, varName);
  }
  default: {
    return false;
  }
  }
  return false;
}

const bool spa::PKB::addCallsContainerParent(std::string procName, std::string lineNo) {
  return relationshipStorage.addCallsContainerParent(procName, lineNo);
}

const bool spa::PKB::addCallsProc(int lineNo, std::string procedure) {
  return relationshipStorage.addCallsProc(lineNo, procedure);
}

const bool spa::PKB::addStatementType(std::string lineNo, StatementType statementType) {
  return relationshipStorage.addStatementType(lineNo, statementType);
}

const bool spa::PKB::addStatementProc(std::string lineNo, std::string procedure) {
  return relationshipStorage.addStatementProc(lineNo, procedure);
}

const bool spa::PKB::addCfgNode(int lineNo, spa::CFGNode cfgNode) {
  return cfgStorage.addCfgNode(lineNo, cfgNode);
}

const bool spa::PKB::updateCfgNode(int lineNo, spa::CFGNode newCfgNode) {
  return cfgStorage.updateCfgNode(lineNo, newCfgNode);
}

const bool spa::PKB::deleteCfgNode(int lineNo) {
  return cfgStorage.deleteCfgNode(lineNo);
}

const spa::QueryResult spa::PKB::getRelationship(RelationshipType relationshipType,
  PKBQueryArg firstArg, PKBQueryArg secondArg) {
  auto relationshipFunctionItr = relationshipQueryFunctionMap.find({ relationshipType,
                                                                     firstArg.getType(), secondArg.getType() });
  return (relationshipFunctionItr->second)(relationshipStorage, firstArg, secondArg);
}

const spa::QueryResult spa::PKB::getEntity(DesignEntityType entityType) {
  if (entityType != DesignEntityType::VARIABLE &&
    entityType != DesignEntityType::PROCEDURE &&
    entityType != DesignEntityType::CONSTANT) {
    auto statementTypeItr = statementTypeMap.find(entityType);
    return relationshipStorage.getStatements(statementTypeItr->second);
  }

  auto entityFunctionItr = entityQueryFunctionMap.find(entityType);
  return (entityFunctionItr->second)(entityStorage);
}

const spa::QueryResult spa::PKB::getPattern(PKBQueryArg lhs, Pattern rhs) {
  auto patternFunctionItr = patternQueryFunctionMap.find(lhs.getType());
  return (patternFunctionItr->second)(patternStorage, lhs, rhs);
}

const spa::QueryResult spa::PKB::getContainerPattern(DesignEntityType entityType, PKBQueryArg firstArg) {
  auto patternContainerFunctionItr = patternContainerQueryFunctionMap.find({ entityType, firstArg.getType() });
  return (patternContainerFunctionItr->second)(patternStorage, firstArg);
}

const spa::QueryResult spa::PKB::getCallsContainerParent(std::string procName) {
  return relationshipStorage.getCallsContainerParent(procName);
}

const spa::QueryResult spa::PKB::getCallsProc() {
  return relationshipStorage.getCallsProc();
}

const spa::QueryResult spa::PKB::getCfgNode(int lineNo) {
  return cfgStorage.getCfgNode(lineNo);
}
