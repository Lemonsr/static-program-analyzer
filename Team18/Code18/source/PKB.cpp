#include "PKB.h"
#include "PKBTypes.h"
#include "PKBQueryTypes.h"
#include "TNode.h"

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>


int spa::PKB::setProcToAST(PROC p, TNode* r) {
  return 0;
}

spa::TNode* spa::PKB::getRootAST(PROC p) {
  return nullptr;
}

// Get table methods
const spa::VarTable& spa::PKB::getVarTable() const {
  return varTable;
}

const spa::ConstTable& spa::PKB::getConstTable() const {
  return constTable;
}

const spa::ProcTable& spa::PKB::getProcTable() const {
  return procTable;
}

const spa::FollowsTable& spa::PKB::getFollowsTable() const {
  return followsTable;
}

const spa::FollowsStarTable& spa::PKB::getFollowsStarTable() const {
  return followsStarTable;
}

const spa::ParentTable& spa::PKB::getParentTable() const {
  return parentTable;
}

const spa::ParentStarTable& spa::PKB::getParentStarTable() const {
  return parentStarTable;
}

const spa::ModifiesTable& spa::PKB::getModifiesTable() const {
  return modifiesTable;
}

const spa::ModifiesProcTable& spa::PKB::getModifiesProcTable() const {
  return modifiesProcTable;
}

const spa::UsesTable& spa::PKB::getUsesTable() const {
  return usesTable;
}

const spa::UsesProcTable& spa::PKB::getUsesProcTable() const {
    return usesProcTable;
}

const spa::AssignTable& spa::PKB::getAssignTable() const {
  return assignTable;
}

const spa::StatementTypeTable& spa::PKB::getStatementTypeTable() const {
    return statementTypeTable;
}

const spa::StatementProcTable& spa::PKB::getStatementProcTable() const {
    return statementProcTable;
}

// Follows methods
void spa::PKB::addFollows(LineNo lineNo1, LineNo lineNo2) {
  followsTable.insert({ lineNo1, lineNo2 }); // s2 -follows-> s1
}

void spa::PKB::addFollowsStar(LineNo lineNo, Follow follow) {
  followsStarTable.insert({ lineNo, follow }); // s2 -follows*-> s1
}

// Parent methods
void spa::PKB::addParent(Child child, Parent parent) {
  parentTable.insert({ child, parent }); // s2 -parent-> s1 
}

void spa::PKB::addParentStar(Child child, Parents parents) {
  parentStarTable.insert({ child, parents }); // s2 -parent*-> s1 
}
