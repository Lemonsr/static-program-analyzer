#pragma once

#include "PKBQueryTypes.h"
#include "PKBTypes.h"

#include <string>

namespace spa {
/*
An abstract class to define all PKB tables and its related add/get methods.
*/
class PKBTables {
 public:
    virtual const VarTable& getVarTable() const = 0;

    virtual const ConstTable& getConstTable() const = 0;

    virtual const ProcTable& getProcTable() const = 0;

    virtual const FollowsTable& getFollowsTable() const = 0;

    virtual const FollowsStarTable& getFollowsStarTable() const = 0;

    virtual const ParentTable& getParentTable() const = 0;

    virtual const ParentStarTable& getParentStarTable() const = 0;

    virtual const ModifiesTable& getModifiesTable() const = 0;

    virtual const ModifiesProcTable& getModifiesProcTable() const = 0;

    virtual const UsesTable& getUsesTable() const = 0;

    virtual const UsesProcTable& getUsesProcTable() const = 0;

    virtual const AssignTable& getAssignTable() const = 0;

    virtual const StatementTypeTable& getStatementTypeTable() const = 0;

    virtual const StatementProcTable& getStatementProcTable() const = 0;

    // Follows methods
    virtual void addFollows(LineNo lineNo1, LineNo lineNo2) = 0;

    // Parent methods
    virtual void addParent(LineNo lineNo1, LineNo lineNo2) = 0;

    // Modifies methods
    virtual void addModifies(LineNo lineNo, Name varName) = 0;

    virtual bool getModifies(LineNumber lineNumber, Name varName) = 0;

    virtual VarNames getModifies(LineNumber lineNumber, Variable var) = 0;

    virtual bool getModifies(LineNumber lineNumber, Underscore underscore) = 0;

    virtual LineNo getModifies(Statement stmt, Name varName) = 0;

    virtual LineNo_Var_Pairs getModifies(LineNo lineNo, Name varName) = 0;

    virtual LineNo getModifies(Statement stmt, Underscore underscore) = 0;

    // Uses methods
    virtual void addUses(LineNo lineNo, Name var) = 0;

    virtual bool getUses(LineNumber lineNumber, Name varName) = 0;

    virtual VarNames getUses(LineNumber lineNumber, Variable var) = 0;

    virtual bool getUses(LineNumber lineNumber, Underscore underscore) = 0;

    virtual LineNo getUses(Statement stmt, Name varName) = 0;

    virtual LineNo_Var_Pairs getUses(LineNo lineNo, Name varName) = 0;

    virtual LineNo getUses(Statement stmt, Underscore underscore) = 0;

    // Assign methods
    virtual void addAssign(LineNo lineNo, PostfixString postfixString) = 0;

    // TODO: add getAssign methods.

    // Statement Type methods
    virtual void addStatementType(LineNo lineNo, StatementType statementType) = 0;

 protected:
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
} // namespace spa
