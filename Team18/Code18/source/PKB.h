#pragma once

#include "PKBTypes.h"
#include "PKBQueryTypes.h"

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

typedef int16_t PROC;

namespace spa {
class TNode;

class PKB {
 public:
  static int setProcToAST(PROC p, TNode* r);
  static TNode* getRootAST(PROC p);

  const VarTable& getVarTable() const;
  const ConstTable& getConstTable() const;
  const ProcTable& getProcTable() const;
  const FollowsTable& getFollowsTable() const;
  const FollowsStarTable& getFollowsStarTable() const;
  const ParentTable& getParentTable() const;
  const ParentStarTable& getParentStarTable() const;
  const ModifiesTable& getModifiesTable() const;
  const ModifiesProcTable& getModifiesProcTable() const;
  const UsesTable& getUsesTable() const;
  const UsesProcTable& getUsesProcTable() const;
  const AssignTable& getAssignTable() const;
  const StatementTypeTable& getStatementTypeTable() const;
  const StatementProcTable& getStatementProcTable() const;

  // Follows methods
  void addFollows(LineNo lineNo1, LineNo lineNo2);

  // Parent methods
  void addParent(LineNo lineNo1, LineNo lineNo2);

  // Modifies methods
  void addModifies(LineNo lineNo, Name varName);
  bool getModifies(LineNumber lineNumber, Name varName);
  VarNames getModifies(LineNumber lineNumber, Variable var);
  bool getModifies(LineNumber lineNumber, Underscore underscore);
  LineNos getModifies(Statement stmt, Name varName);
  LineNo_Var_Pairs getModifies(LineNo lineNo, Name varName);
  LineNos getModifies(Statement stmt, Underscore underscore);

  // Uses methods
  void addUses(LineNo lineNo, Name var);
  bool getUses(LineNumber lineNumber, Name varName);
  VarNames getUses(LineNumber lineNumber, Variable var);
  bool getUses(LineNumber lineNumber, Underscore underscore);
  LineNos getUses(Statement stmt, Name varName);
  LineNo_Var_Pairs getUses(LineNo lineNo, Name varName);
  LineNos getUses(Statement stmt, Underscore underscore);

  // Assign methods
  void addAssign(LineNo lineNo, PostfixString postfixString);

  // TODO: add getAssign methods.

  // Statement Type methods
  void addStatementType(LineNo lineNo,
    StatementType statementType);

 private:
  VarTable varTable;
  ConstTable constTable;
  ProcTable procTable;
  FollowsTable followsTable;
  FollowsStarTable followsStarTable;
  ParentTable parentTable;
  ParentStarTable parentStarTable;
  ModifiesTable modifiesTable;
  ModifiesProcTable modifiesProcTable;
  UsesTable usesTable;
  UsesProcTable usesProcTable;
  AssignTable assignTable;
  StatementTypeTable statementTypeTable;
  StatementProcTable statementProcTable;
};
}  // namespace spa
