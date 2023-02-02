#pragma once
#include <unordered_set>

#include "ProgramStatement.h"
#include "Token.h"

namespace spa {
  class NonContainerStatement : public ProgramStatement {
  protected:
    std::unordered_set<int> whileStmtParents;
    std::unordered_set<int> ifStmtParents;

    void addParentUses(PKBManager& pkbManager, std::string variableName, std::unordered_set<int> whileStmtParents,
                       std::unordered_set<int> ifStmtParents);
    void addParentModifies(PKBManager& pkbManager, std::string variableName,
                           std::unordered_set<int> whileStmtParents,
                           std::unordered_set<int> ifStmtParents);
  };

  class OneVarNonContainerStatement : public NonContainerStatement {
  protected:
    std::string variableName;

  public:
    std::string getVariableName();
  };

  class ReadStatement : public OneVarNonContainerStatement {
  public:
    ReadStatement(std::string parentProcedureVal, std::string variableName,
                  std::unordered_set<int> whileStmtParents,
                  std::unordered_set<int> ifStmtParents, int statementLineNum);
    void processStatement(PKBManager& pkbManager) override;
  };

  class PrintStatement : public OneVarNonContainerStatement {
  public:
    PrintStatement(std::string parentProcedureVal, std::string variableName,
                   std::unordered_set<int> whileStmtParents,
                   std::unordered_set<int> ifStmtParents, int statementLineNum);
    void processStatement(PKBManager& pkbManager) override;
  };

  class CallStatement : public OneVarNonContainerStatement {
  public:
    CallStatement(std::string parentProcedureVal, std::string variableName,
                  std::unordered_set<int> whileStmtParents,
                  std::unordered_set<int> ifStmtParents, int statementLineNum);
    void processStatement(PKBManager& pkbManager) override;
  };

  class MultiVarNonContainerStatement : public NonContainerStatement {
  protected:
    std::string postfixExpr;
    void spa::MultiVarNonContainerStatement::extractUsesFromPostfix(PKBManager& pkbManager, std::string postfix);

  };

  class AssignStatement : public MultiVarNonContainerStatement {
  private:
    std::string assignVar;

  public:
    AssignStatement(std::string parentProcedureVal, std::string assignVar, std::string postfixExpr,
                    std::unordered_set<int> whileStmtParents,
                    std::unordered_set<int> ifStmtParents, int statementLineNum);
    void processStatement(PKBManager& pkbManager) override;
  };

  class IfConditionStatement : public MultiVarNonContainerStatement {
  public:
    IfConditionStatement(std::string parentProcedureVal, std::string postfixExpr,
                         std::unordered_set<int> whileStmtParents,
                         std::unordered_set<int> ifStmtParents, int statementLineNum);
    void processStatement(PKBManager& pkbManager) override;
  };

  class WhileConditionStatement : public MultiVarNonContainerStatement {
  public:
    WhileConditionStatement(std::string parentProcedureVal, std::string postfixExpr,
                            std::unordered_set<int> whileStmtParents,
                            std::unordered_set<int> ifStmtParents, int statementLineNum);
    void processStatement(PKBManager& pkbManager) override;
  };
} // namespace spa
