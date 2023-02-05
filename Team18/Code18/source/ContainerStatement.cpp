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

// Constructor for IfContainerStatement
spa::WhileContainerStatement::WhileContainerStatement(std::string parentProcedureVal,
                                                      int statementLineNum,
                                                      std::vector<ProgramStatement*> statementList) {
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
