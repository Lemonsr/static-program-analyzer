#pragma once
#include <unordered_set>

#include "Statement.h"
#include "Token.h"

namespace spa {
  class NonContainerStatement : public Statement {
  protected:
    std::unordered_set<int> whileStmtParents;
    std::unordered_set<int> ifStmtParents;
  };

  class OneVarNonContainerStatement : public NonContainerStatement {
  protected:
    std::string variableName;

  public:
    std::string getVariableName();
  };

  class ReadStatement : public OneVarNonContainerStatement {
  public:
    ReadStatement(std::string parentProcedureVal, std::string variableName, std::unordered_set<int> whileStmtParents,
                  std::unordered_set<int> ifStmtParents, int statementLineNum);
  };

  class PrintStatement : public OneVarNonContainerStatement {
  public:
    PrintStatement(std::string parentProcedureVal, std::string variableName, std::unordered_set<int> whileStmtParents,
                   std::unordered_set<int> ifStmtParents, int statementLineNum);
  };

  class CallStatement : public OneVarNonContainerStatement {
  public:
    CallStatement(std::string parentProcedureVal, std::string variableName, std::unordered_set<int> whileStmtParents,
                  std::unordered_set<int> ifStmtParents, int statementLineNum);
  };

  class MultiVarNonContainerStatement : public NonContainerStatement {
  protected:
    std::string postfixExpr;
  };

  class AssignStatement : public MultiVarNonContainerStatement {
  private:
    std::string assignVar;

  public:
    AssignStatement(std::string parentProcedureVal, std::string assignVar, std::string postfixExpr,
                    std::unordered_set<int> whileStmtParents,
                    std::unordered_set<int> ifStmtParents, int statementLineNum);
  };

  class IfConditionStatement : public MultiVarNonContainerStatement {
  public:
    IfConditionStatement(std::string parentProcedureVal, std::string postfixExpr,
                         std::unordered_set<int> whileStmtParents,
                         std::unordered_set<int> ifStmtParents, int statementLineNum);
  };

  class WhileConditionStatement : public MultiVarNonContainerStatement {
  public:
    WhileConditionStatement(std::string parentProcedureVal, std::string postfixExpr,
                            std::unordered_set<int> whileStmtParents,
                            std::unordered_set<int> ifStmtParents, int statementLineNum);
  };
}  // namespace spa
