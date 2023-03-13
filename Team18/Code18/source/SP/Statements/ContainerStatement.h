#pragma once
#include <memory>
#include <string>
#include <vector>
#include <unordered_set>

#include "ProgramStatement.h"

namespace spa {
class ContainerStatement : public ProgramStatement {
 protected:
  std::vector<std::shared_ptr<ProgramStatement>> statementList;
  std::unordered_set<std::string> proceduresCalled;

 public:
  std::vector<std::shared_ptr<spa::ProgramStatement>>& getStatementList();
  std::unordered_set<std::string> getProceduresCalled();
};

class IfContainerStatement : public ContainerStatement {
 public:
  IfContainerStatement(std::string parentProcedureVal, int statementLineNum,
                       std::vector<std::shared_ptr<ProgramStatement>>& statementList);
  std::pair<CFGNode, CFGNode> processStatement(PKBManager& pkbManager) override;
};

class WhileContainerStatement : public ContainerStatement {
 public:
  WhileContainerStatement(std::string parentProcedureVal, int statementLineNum,
                          std::vector<std::shared_ptr<ProgramStatement>> statementList);
  std::pair<CFGNode, CFGNode> processStatement(PKBManager& pkbManager) override;
};

class InnerBlockStatement : public ContainerStatement {
 public:
  InnerBlockStatement(std::string parentProcedureVal, std::vector<std::shared_ptr<ProgramStatement>> statementList);
  std::pair<CFGNode, CFGNode> processStatement(PKBManager& pkbManager) override;
};
}  // namespace spa
