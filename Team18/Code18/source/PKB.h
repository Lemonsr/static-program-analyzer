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

  LineNos getAssignCommonLogic(Pattern pattern);

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
  void addFollows(LineNo lineNo, Follow follow);
  void addFollowsStar(LineNo lineNo, Follows follows);

  // Parent methods
  void addParent(Child child, Parent parent);
  void addParentStar(Child child, Parents parents);

  // Modifies methods
  void addModifies(LineNo lineNo, Modifies modifies);
  bool getModifies(LineNumber lineNumber, Name varName);
  bool getModifies(LineNumber lineNumber, Underscore underscore);
  VarNames getModifies(LineNumber lineNumber, Variable var);
  LineNos getModifies(Statement stmt, Name varName);
  LineNos getModifies(Statement stmt, Underscore underscore);
  LineNo_Var_Pairs getModifies(Statement stmt, Variable var);

  // TODO: ModifiesProc methods

  // Uses methods
  void addUses(LineNo lineNo, Uses uses);
  bool getUses(LineNumber lineNumber, Name varName);
  bool getUses(LineNumber lineNumber, Underscore underscore);
  VarNames getUses(LineNumber lineNumber, Variable var);
  LineNos getUses(Statement stmt, Name varName);
  LineNos getUses(Statement stmt, Underscore underscore);
  LineNo_Var_Pairs getUses(Statement stmt, Variable var);

  // TODO: UsesProc methods

  // Assign methods
  void addAssign(LineNo lineNo, VarName varName, PostfixString postfixString);
  LineNos getAssign(Underscore underscore, Pattern pattern);
  LineNos getAssign(Variable variable, Pattern pattern);
  LineNos getAssign(Name name, Pattern pattern);

  // Statement Type methods
  void addStatementType(LineNo lineNo, StatementType statementType);
};
}  // namespace spa
