#include "ContainerStatement.h"

std::vector<spa::ProgramStatement*> spa::ContainerStatement::getStatementList() {
  return statementList;
}

// Constructor for IfContainerStatement
spa::IfContainerStatement::IfContainerStatement(std::string parentProcedureVal,
                                                int statementLineNum,
                                                std::vector<ProgramStatement*> statementList) {
  this->parentProcedureVal = parentProcedureVal;
  this->statementLineNum = statementLineNum;
  this->statementList = statementList;
}

// Constructor for WhileContainerStatement
spa::WhileContainerStatement::WhileContainerStatement(std::string parentProcedureVal,
                                                      int statementLineNum,
                                                      std::vector<ProgramStatement*>
                                                      statementList) {
  this->parentProcedureVal = parentProcedureVal;
  this->statementLineNum = statementLineNum;
  this->statementList = statementList;
}

// Constructor for IfContainerStatement
spa::InnerBlockStatement::InnerBlockStatement(std::string parentProcedureVal,
                                              std::vector<ProgramStatement*> statementList) {
  this->parentProcedureVal = parentProcedureVal;
  this->statementList = statementList;
}

void spa::IfContainerStatement::processStatement(PKBManager& pkbManager) {
  ProgramStatement* ifConditionStatement = statementList[0];
  ProgramStatement* thenStatement = statementList[1];
  ProgramStatement* elseStatement = statementList[2];
  ifConditionStatement->processStatement(pkbManager);
  thenStatement->processStatement(pkbManager);
  elseStatement->processStatement(pkbManager);
}

void spa::WhileContainerStatement::processStatement(PKBManager& pkbManager) {
  ProgramStatement* whileConditionStatement = statementList[0];
  ProgramStatement* innerWhileBlockStatements = statementList[1];
  whileConditionStatement->processStatement(pkbManager);
  innerWhileBlockStatements->processStatement(pkbManager);
}

void spa::InnerBlockStatement::processStatement(PKBManager& pkbManager) {
  for (auto statement : statementList) {
    statement->processStatement(pkbManager);
  }
}
