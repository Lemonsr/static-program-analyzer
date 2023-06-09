#pragma once
#include <string>
#include <utility>

#include "PKB.h"

namespace spa {
class ProgramStatement {
 protected:
  int statementLineNum = -1;
  std::string parentProcedureVal;

 public:
  int getStatementLineNum();
  const std::string getParentProcedureVal();
  virtual std::pair<CFGNode, CFGNode> processStatement(PKBManager& pkbManager);
};
}  // namespace spa
