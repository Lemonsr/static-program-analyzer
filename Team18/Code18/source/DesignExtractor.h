#pragma once
#include <vector>
#include <string>

#include "ContainerStatement.h"
#include "PKB.h"
#include "ProcedureStatement.h"

namespace spa {
class DesignExtractor {
 private:
  PKBManager& pkbManager;
  std::vector<ProcedureStatement>& procedureList;

  void extractDesignAbstraction(std::vector<ProgramStatement*> statementList);
  void extractParentAbstraction(std::vector<ProgramStatement*> statementList);
  void extractFollows(std::vector<ProgramStatement*> statementList);
  void extractFollowsStar(std::vector<ProgramStatement*> statementList);
  void extractParent(ContainerStatement* containerStatement);
  void extractParentStar(ContainerStatement* containerStatement, std::string ancestorLineNum);
  void extractUsesAndModifies(std::vector<ProgramStatement*> statementList);

 public:
  DesignExtractor(PKBManager& pkbManager, std::vector<ProcedureStatement>& procedureList);
  void extractRelationship();
};
}  // namespace spa
