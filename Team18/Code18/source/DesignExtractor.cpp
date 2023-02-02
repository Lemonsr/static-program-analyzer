#include "DesignExtractor.h"

#include <vector>

#include "ContainerStatement.h"
#include "ProcedureStatement.h"

spa::DesignExtractor::DesignExtractor(PKBManager& pkbManager, std::vector<ProcedureStatement>& procedureList) :
  pkbManager(pkbManager), procedureList(procedureList) {}

void spa::DesignExtractor::extractRelationship() {
  for (ProcedureStatement& procedure : procedureList) {
    pkbManager.addEntity(PROCEDURE, procedure.getProcedureVarToken().getValue());
    std::vector<ProgramStatement*> statementList = procedure.getStatementLst();
    extractDesignAbstraction(statementList);
  }
}

void spa::DesignExtractor::extractDesignAbstraction(std::vector<ProgramStatement*> statementList) {
  extractFollows(statementList);
  extractFollowsStar(statementList);
  extractParentAbstraction(statementList);
  extractUsesAndModifies(statementList);
}

void spa::DesignExtractor::extractParentAbstraction(std::vector<ProgramStatement*> statementList) {
  for (ProgramStatement* statement : statementList) {
    if (dynamic_cast<ContainerStatement*>(statement)) {
      auto containerStatement = dynamic_cast<ContainerStatement*>(statement);
      extractParent(containerStatement);
      extractParentStar(containerStatement, containerStatement->getStatementLineNum());
    }
  }
}

void spa::DesignExtractor::extractFollows(std::vector<ProgramStatement*> statementList) {
  for (unsigned i = 0; i < statementList.size(); i++) {
    if (dynamic_cast<ContainerStatement*>(statementList[i])) {
      ContainerStatement* containerStatement = dynamic_cast<ContainerStatement*>(statementList[i]);
      extractFollows(containerStatement->getStatementList());
    }
    if (i == statementList.size() - 1) {
      break;
    }
    std::string followStmtOne = std::to_string(statementList[i]->getStatementLineNum());
    std::string followStmtTwo = std::to_string(statementList[i + 1]->getStatementLineNum());
    pkbManager.addRelationship(FOLLOWS, followStmtOne, followStmtTwo);
  }
}

void spa::DesignExtractor::extractFollowsStar(std::vector<ProgramStatement*> statementList) {
  for (unsigned i = 0; i < statementList.size(); i++) {
    for (unsigned j = i + 1; j < statementList.size(); j++) {
      int firstStatementLineNum = statementList[i]->getStatementLineNum();
      int secondStatementLineNum = statementList[j]->getStatementLineNum();
      if (firstStatementLineNum != -1 && secondStatementLineNum != -1) {
        std::string followStarStmtOne = std::to_string(statementList[i]->getStatementLineNum());
        std::string followStarStmtTwo = std::to_string(statementList[j]->getStatementLineNum());
        pkbManager.addRelationship(FOLLOWS_STAR, followStarStmtOne, followStarStmtTwo);
      }
      if (dynamic_cast<ContainerStatement*>(statementList[i])) {
        ContainerStatement* containerStatement = dynamic_cast<ContainerStatement*>(statementList[
          i]);
        extractFollowsStar(containerStatement->getStatementList());
      }
    }
  }
}

void spa::DesignExtractor::extractParent(ContainerStatement* containerStatement) {
  std::vector<ProgramStatement*> statementList = containerStatement->getStatementList();
  for (ProgramStatement* statement : statementList) {
    std::string parentStmtOne = std::to_string(containerStatement->getStatementLineNum());
    if (statement->getStatementLineNum() == containerStatement->getStatementLineNum()) {
      continue;
    }
    if (statement->getStatementLineNum() != -1) {
      std::string parentStmtTwo = std::to_string(statement->getStatementLineNum());
      pkbManager.addRelationship(PARENT, parentStmtOne, parentStmtTwo);
      continue;
    }
    auto innerBlockStatements = dynamic_cast<ContainerStatement*>(statement);
    std::vector<ProgramStatement*> innerBlockStmtList = innerBlockStatements->getStatementList();
    for (ProgramStatement* childStatement : innerBlockStmtList) {
      std::string parentStmtTwo = std::to_string(childStatement->getStatementLineNum());
      pkbManager.addRelationship(PARENT, parentStmtOne, parentStmtTwo);
      if (dynamic_cast<ContainerStatement*>(childStatement)) {
        auto childContainerStmt = dynamic_cast<ContainerStatement*>(childStatement);
        extractParent(childContainerStmt);
      }
    }
  }
}

void spa::DesignExtractor::extractParentStar(ContainerStatement* containerStatement,
                                             int ancestorLineNum) {
  std::vector<ProgramStatement*> statementList = containerStatement->getStatementList();
  for (ProgramStatement* statement : statementList) {
    std::string parentStmtOne = std::to_string(containerStatement->getStatementLineNum());
    if (statement->getStatementLineNum() == containerStatement->getStatementLineNum()) {
      continue;
    }
    if (statement->getStatementLineNum() != -1) {
      std::string parentStmtTwo = std::to_string(statement->getStatementLineNum());
      pkbManager.addRelationship(PARENT_STAR, parentStmtOne, parentStmtTwo);
      continue;
    }
    auto innerBlockStatements = dynamic_cast<ContainerStatement*>(statement);
    std::vector<ProgramStatement*> innerBlockStmtList = innerBlockStatements->getStatementList();
    for (ProgramStatement* childStatement : innerBlockStmtList) {
      std::string parentStmtTwo = std::to_string(childStatement->getStatementLineNum());
      pkbManager.addRelationship(PARENT_STAR, parentStmtOne, parentStmtTwo);
      if (dynamic_cast<ContainerStatement*>(childStatement)) {
        auto childContainerStmt = dynamic_cast<ContainerStatement*>(childStatement);
        extractParentStar(childContainerStmt, ancestorLineNum);
        extractParentStar(childContainerStmt, childStatement->getStatementLineNum());
      }
    }
  }
}

void spa::DesignExtractor::extractUsesAndModifies(std::vector<ProgramStatement*> statementList) {
  for (auto statement : statementList) {
    statement->processStatement(pkbManager);
  }
}
