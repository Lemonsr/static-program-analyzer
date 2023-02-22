#include "SimpleEvaluator.h"
#include "PqlAttributes.h"

const std::unordered_map<spa::DesignEntityType,
  std::function<spa::QpsResultTable(spa::SimpleEvaluator&, spa::PKBManager&)>> designEntityEvaluateFunctionMap {
    {{spa::DesignEntityType::PROCEDURE}, &spa::SimpleEvaluator::evaluateProcedure},
    {{spa::DesignEntityType::STMT}, &spa::SimpleEvaluator::evaluateStmt},
    {{spa::DesignEntityType::READ}, &spa::SimpleEvaluator::evaluateRead},
    {{spa::DesignEntityType::PRINT}, &spa::SimpleEvaluator::evaluatePrint},
    {{spa::DesignEntityType::ASSIGN}, &spa::SimpleEvaluator::evaluateAssign},
    {{spa::DesignEntityType::CALL}, &spa::SimpleEvaluator::evaluateCall},
    {{spa::DesignEntityType::WHILE}, &spa::SimpleEvaluator::evaluateWhile},
    {{spa::DesignEntityType::IF}, &spa::SimpleEvaluator::evaluateIf},
    {{spa::DesignEntityType::VARIABLE}, &spa::SimpleEvaluator::evaluateVariable},
    {{spa::DesignEntityType::CONSTANT}, &spa::SimpleEvaluator::evaluateConstant}
};

spa::SimpleEvaluator::SimpleEvaluator(std::string selectSynonym, DesignEntityType designEntityType) :
  selectSynonym(selectSynonym), designEntityType(designEntityType) {
}

spa::QpsResultTable spa::SimpleEvaluator::evaluate(PKBManager& pkbManager) {
  switch (designEntityType) {
  case PROCEDURE: {
    return evaluateProcedure(pkbManager);
  }
  case STMT: {
    return evaluateStmt(pkbManager);
  }
  case READ: {
    return evaluateRead(pkbManager);
  }
  case PRINT: {
    return evaluatePrint(pkbManager);
  }
  case ASSIGN: {
    return evaluateAssign(pkbManager);
  }
  case CALL: {
    return evaluateCall(pkbManager);
  }
  case WHILE: {
    return evaluateWhile(pkbManager);
  }
  case IF: {
    return evaluateIf(pkbManager);
  }
  case VARIABLE: {
    return evaluateVariable(pkbManager);
  }
  case CONSTANT: {
    return evaluateConstant(pkbManager);
  }
  default: {
    return {};
  }
  }
}

spa::QpsResultTable spa::SimpleEvaluator::evaluateProcedure(PKBManager& pkbManager) {
  QpsResultTable resultTable;
  resultTable.addHeader(selectSynonym);
  for (auto& attr : pqlAttributesMap[designEntityType]) {
    resultTable.addHeader(selectSynonym.append(".").append(attr));
  }

  QueryResult result = pkbManager.getEntity(designEntityType);
  for (auto& procName : result.getNames()) {
    resultTable.addRow({ QpsValue(procName), QpsValue(procName) });
  }

  return resultTable;
}

spa::QpsResultTable spa::SimpleEvaluator::evaluateStmt(PKBManager& pkbManager) {
  QpsResultTable resultTable;
  resultTable.addHeader(selectSynonym);
  for (auto& attr : pqlAttributesMap[designEntityType]) {
    resultTable.addHeader(selectSynonym.append(".").append(attr));
  }

  QueryResult result = pkbManager.getEntity(designEntityType);
  for (auto& stmtNum : result.getLineNumbers()) {
    resultTable.addRow({ QpsValue(stmtNum), QpsValue(stmtNum) });
  }

  return resultTable;
}

spa::QpsResultTable spa::SimpleEvaluator::evaluateRead(PKBManager& pkbManager) {
  PKBQueryArg readSynonymArg(PqlArgument(ArgumentType::SYNONYM, "re", designEntityType));
  PKBQueryArg varSynonymArg(PqlArgument(ArgumentType::SYNONYM, "v", DesignEntityType::VARIABLE));
  QpsResultTable resultTable;
  resultTable.addHeader(selectSynonym);
  for (auto& attr : pqlAttributesMap[designEntityType]) {
    resultTable.addHeader(selectSynonym.append(".").append(attr));
  }

  QueryResult result = pkbManager.getRelationship(RelationshipType::MODIFIES, readSynonymArg, varSynonymArg);
  for (auto& pair : result.getLineNumberVariablePairs()) {
    int lineNumber = pair.first;
    std::string varName = pair.second;
    resultTable.addRow({ QpsValue(lineNumber), QpsValue(lineNumber), QpsValue(varName) });
  }

  return resultTable;
}

spa::QpsResultTable spa::SimpleEvaluator::evaluatePrint(PKBManager& pkbManager) {
  PKBQueryArg readSynonymArg(PqlArgument(ArgumentType::SYNONYM, "pr", designEntityType));
  PKBQueryArg varSynonymArg(PqlArgument(ArgumentType::SYNONYM, "v", DesignEntityType::VARIABLE));
  QpsResultTable resultTable;
  resultTable.addHeader(selectSynonym);
  for (auto& attr : pqlAttributesMap[designEntityType]) {
    resultTable.addHeader(selectSynonym.append(".").append(attr));
  }

  QueryResult result = pkbManager.getRelationship(RelationshipType::MODIFIES, readSynonymArg, varSynonymArg);
  for (auto& pair : result.getLineNumberVariablePairs()) {
    int lineNumber = pair.first;
    std::string varName = pair.second;
    resultTable.addRow({ QpsValue(lineNumber), QpsValue(lineNumber), QpsValue(varName) });
  }

  return resultTable;
}

spa::QpsResultTable spa::SimpleEvaluator::evaluateAssign(PKBManager& pkbManager) {
  QpsResultTable resultTable;
  resultTable.addHeader(selectSynonym);
  for (auto& attr : pqlAttributesMap[designEntityType]) {
    resultTable.addHeader(selectSynonym.append(".").append(attr));
  }

  QueryResult result = pkbManager.getEntity(designEntityType);
  for (auto& assignNum : result.getLineNumbers()) {
    resultTable.addRow({ QpsValue(assignNum), QpsValue(assignNum) });
  }

  return resultTable;
}

spa::QpsResultTable spa::SimpleEvaluator::evaluateCall(PKBManager& pkbManager) {
  PKBQueryArg readSynonymArg(PqlArgument(ArgumentType::SYNONYM, "c", designEntityType));
  PKBQueryArg varSynonymArg(PqlArgument(ArgumentType::SYNONYM, "v", DesignEntityType::VARIABLE));
  QpsResultTable resultTable;
  resultTable.addHeader(selectSynonym);
  for (auto& attr : pqlAttributesMap[designEntityType]) {
    resultTable.addHeader(selectSynonym.append(".").append(attr));
  }

  QueryResult result = pkbManager.getRelationship(RelationshipType::MODIFIES, readSynonymArg, varSynonymArg);
  for (auto& pair : result.getLineNumberVariablePairs()) {
    int lineNumber = pair.first;
    std::string varName = pair.second;
    resultTable.addRow({ QpsValue(lineNumber), QpsValue(lineNumber), QpsValue(varName) });
  }

  return resultTable;
}

spa::QpsResultTable spa::SimpleEvaluator::evaluateWhile(PKBManager& pkbManager) {
  QpsResultTable resultTable;
  resultTable.addHeader(selectSynonym);
  for (auto& attr : pqlAttributesMap[designEntityType]) {
    resultTable.addHeader(selectSynonym.append(".").append(attr));
  }

  QueryResult result = pkbManager.getEntity(designEntityType);
  for (auto& assignNum : result.getLineNumbers()) {
    resultTable.addRow({ QpsValue(assignNum), QpsValue(assignNum) });
  }

  return resultTable;
}

spa::QpsResultTable spa::SimpleEvaluator::evaluateIf(PKBManager& pkbManager) {
  QpsResultTable resultTable;
  resultTable.addHeader(selectSynonym);
  for (auto& attr : pqlAttributesMap[designEntityType]) {
    resultTable.addHeader(selectSynonym.append(".").append(attr));
  }

  QueryResult result = pkbManager.getEntity(designEntityType);
  for (auto& assignNum : result.getLineNumbers()) {
    resultTable.addRow({ QpsValue(assignNum), QpsValue(assignNum) });
  }

  return resultTable;
}

spa::QpsResultTable spa::SimpleEvaluator::evaluateVariable(PKBManager& pkbManager) {
  QpsResultTable resultTable;
  resultTable.addHeader(selectSynonym);
  for (auto& attr : pqlAttributesMap[designEntityType]) {
    resultTable.addHeader(selectSynonym.append(".").append(attr));
  }

  QueryResult result = pkbManager.getEntity(designEntityType);
  for (auto& varName : result.getNames()) {
    resultTable.addRow({ QpsValue(varName), QpsValue(varName) });
  }

  return resultTable;
}

spa::QpsResultTable spa::SimpleEvaluator::evaluateConstant(PKBManager& pkbManager) {
  QpsResultTable resultTable;
  resultTable.addHeader(selectSynonym);
  for (auto& attr : pqlAttributesMap[designEntityType]) {
    resultTable.addHeader(selectSynonym.append(".").append(attr));
  }

  QueryResult result = pkbManager.getEntity(designEntityType);
  for (auto& varName : result.getNames()) {
    resultTable.addRow({ QpsValue(varName), QpsValue(varName) });
  }

  return resultTable;
}
