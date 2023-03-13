#pragma once
#include <string>
#include <vector>
#include <unordered_set>
#include <utility>

#include "ProgramStatement.h"

namespace spa {
class ContainerStatement : public ProgramStatement {
 protected:
  std::vector<ProgramStatement*> statementList;
  std::unordered_set<std::string> proceduresCalled;

 public:
  std::vector<ProgramStatement*> getStatementList();
  std::unordered_set<std::string> getProceduresCalled();
};

class IfContainerStatement : public ContainerStatement {
 public:
  IfContainerStatement(std::string parentProcedureVal, int statementLineNum,
                       std::vector<ProgramStatement*> statementList);
  std::pair<CFGNode, CFGNode> processStatement(PKBManager& pkbManager) override;
};

class WhileContainerStatement : public ContainerStatement {
 public:
  WhileContainerStatement(std::string parentProcedureVal, int statementLineNum,
                          std::vector<ProgramStatement*> statementList);
  std::pair<CFGNode, CFGNode> processStatement(PKBManager& pkbManager) override;
};

class InnerBlockStatement : public ContainerStatement {
 public:
  InnerBlockStatement(std::string parentProcedureVal, std::vector<ProgramStatement*> statementList);
  std::pair<CFGNode, CFGNode> processStatement(PKBManager& pkbManager) override;
};
}  // namespace spa
