#include "ContainerStatement.h"

#include <memory>
#include <unordered_set>

#include "NonContainerStatement.h"

std::vector<std::shared_ptr<spa::ProgramStatement>>& spa::ContainerStatement::getStatementList() {
  return statementList;
}

std::unordered_set<std::string> spa::ContainerStatement::getProceduresCalled() {
  if (proceduresCalled.empty()) {
    for (auto& statement : statementList) {
      if (std::dynamic_pointer_cast<spa::CallStatement>(statement)) {
        auto callStatement = std::dynamic_pointer_cast<spa::CallStatement>(statement);
        proceduresCalled.emplace(callStatement->getVariableName());
      } else if (std::dynamic_pointer_cast<spa::ContainerStatement>(statement)) {
        auto containerStatement = std::dynamic_pointer_cast<spa::ContainerStatement>(statement);
        std::unordered_set<std::string> calledSet = containerStatement->getProceduresCalled();
        for (auto& called : calledSet) {
          proceduresCalled.emplace(called);
        }
      }
    }
  }
  return proceduresCalled;
}

// Constructor for IfContainerStatement
spa::IfContainerStatement::IfContainerStatement(std::string parentProcedureVal,
                                                int statementLineNum,
                                                std::vector<std::shared_ptr<ProgramStatement>>&
                                                statementList) {
  this->parentProcedureVal = parentProcedureVal;
  this->statementLineNum = statementLineNum;
  this->statementList = statementList;
}

// Constructor for WhileContainerStatement
spa::WhileContainerStatement::WhileContainerStatement(std::string parentProcedureVal,
                                                      int statementLineNum,
                                                      std::vector<std::shared_ptr<ProgramStatement>>
                                                      statementList) {
  this->parentProcedureVal = parentProcedureVal;
  this->statementLineNum = statementLineNum;
  this->statementList = statementList;
}

// Constructor for IfContainerStatement
spa::InnerBlockStatement::InnerBlockStatement(std::string parentProcedureVal,
                                              std::vector<std::shared_ptr<ProgramStatement>>
                                              statementList) {
  this->parentProcedureVal = parentProcedureVal;
  this->statementList = statementList;
}

void spa::IfContainerStatement::processStatement(PKBManager& pkbManager) {
  ProgramStatement* ifConditionStatement = statementList[0].get();
  ProgramStatement* thenStatement = statementList[1].get();
  ProgramStatement* elseStatement = statementList[2].get();
  ifConditionStatement->processStatement(pkbManager);
  thenStatement->processStatement(pkbManager);
  elseStatement->processStatement(pkbManager);
}

void spa::WhileContainerStatement::processStatement(PKBManager& pkbManager) {
  ProgramStatement* whileConditionStatement = statementList[0].get();
  ProgramStatement* innerWhileBlockStatements = statementList[1].get();
  whileConditionStatement->processStatement(pkbManager);
  innerWhileBlockStatements->processStatement(pkbManager);
}

void spa::InnerBlockStatement::processStatement(PKBManager& pkbManager) {
  for (auto& statement : statementList) {
    statement->processStatement(pkbManager);
  }
}
