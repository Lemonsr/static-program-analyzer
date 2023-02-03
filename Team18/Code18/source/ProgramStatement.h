#pragma once
#include <string>

namespace spa {
  class ProgramStatement {
   protected:
    int statementLineNum;
    std::string parentProcedureVal;

   public:
    int getStatementLineNum();
    const std::string getParentProcedureVal();
  };
}  // namespace spa
