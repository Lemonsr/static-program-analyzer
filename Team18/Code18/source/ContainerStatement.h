#pragma once
#include <string>
#include <vector>

#include "ProgramStatement.h"

namespace spa {
class ContainerStatement : public ProgramStatement {
 protected:
  std::vector<ProgramStatement*> statementList;

 public:
  std::vector<ProgramStatement*> getStatementList();
};

class IfContainerStatement : public ContainerStatement {
 public:
  IfContainerStatement(std::string parentProcedureVal, int statementLineNum,
                       std::vector<ProgramStatement*> statementList);
  void processStatement(PKBManager& pkbManager) override;
};

class WhileContainerStatement : public ContainerStatement {
 public:
  WhileContainerStatement(std::string parentProcedureVal, int statementLineNum,
                          std::vector<ProgramStatement*> statementList);
  void processStatement(PKBManager& pkbManager) override;
};

class InnerBlockStatement : public ContainerStatement {
 public:
  InnerBlockStatement(std::string parentProcedureVal, std::vector<ProgramStatement*> statementList);
  void processStatement(PKBManager& pkbManager) override;
};
}  // namespace spa
