#pragma once
#include "ContainerStatement.h"
#include "PKB.h"
#include "ProcedureStatement.h"

namespace spa {
  class DesignExtractor {
  private:
    PKB& pkb;
    std::vector<ProcedureStatement>& procedureList;
  public:
    DesignExtractor(PKB& pkb, std::vector<ProcedureStatement> procedureList);

    void extractRelationship();
    void extractDesignAbstraction(std::vector<ProgramStatement*> statementList);
    void extractParentAbstraction(std::vector<ProgramStatement*> statementList);
    void extractFollows(std::vector<ProgramStatement*> statementList);
    void extractFollowsStar(std::vector<ProgramStatement*> statementList);
    void extractParent(ContainerStatement* containerStatement);
    void extractParentStar(ContainerStatement* containerStatement, int ancestorLineNum);
    void extractUsesAndModifies(std::vector<ProgramStatement*> statementList);
  };
}  // namespace spa
