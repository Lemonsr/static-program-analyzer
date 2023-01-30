#include "PKB.h"
#include "PKBTypes.h"
#include "PKBQueryTypes.h"
#include "TNode.h"

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <optional>

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
void spa::PKB::addFollows(LineNo lineNo, Follow follow) {
  followsTable.insert({ lineNo, follow });  // s2 -follows-> s1
}

void spa::PKB::addFollowsStar(LineNo lineNo, Follows follows) {
  followsStarTable.insert({ lineNo, follows });  // s2 -follows*-> s1
}

// Parent methods
void spa::PKB::addParent(Child child, Parent parent) {
  parentTable.insert({ child, parent });  // s2 -parent-> s1
}

void spa::PKB::addParentStar(Child child, Parents parents) {
  parentStarTable.insert({ child, parents });  // s2 -parent*-> s1
}

// Modifies methods

/**
 * @brief Adds {lineNo, modifies} to the modifiesTable if 
 * lineNo is not in modifiesTable. 
 * @param lineNo Integer line number in the SIMPLE code.
 * @param modifies Set of varNames modified in the line number.
 */
void spa::PKB::addModifies(LineNo lineNo, Modifies modifies) {
}

/**
 * @brief Checks whether the specified line number modifies 
 * the specified variable. E.g. Modifies(6, "x")
 * @param lineNumber Struct of the line number in the SIMPLE code.
 * @param varName String of the specified variable name.
 * @return boolean.
 */
bool spa::PKB::getModifies(LineNumber lineNumber, Name varName) {
  return false;
}

/**
 * @brief Checks whether the specified line number modifies
 * any variables. E.g. Modifies(6, _)
 * @param lineNumber Struct of the line number in the SIMPLE code.
 * @param underscore Empty Underscore struct.
 * @return boolean.
 */
bool spa::PKB::getModifies(LineNumber lineNumber, Underscore underscore) {
  return false;
}

/**
 * @brief Retrieves variables modified by the specified line number.
 * E.g. Modifies(6, v)
 * @param lineNumber Struct of the line number in the SIMPLE code.
 * @param variable Empty Variable struct.
 * @return A set of variable names.
 */
spa::VarNames spa::PKB::getModifies(LineNumber lineNumber, Variable var) {
  return VarNames();
}

/**
 * @brief Retrieves line numbers of statements of the specified statement type
 * that modifies the specified variable. E.g. Modifies(s, "x")
 * @param stmt Statement with a specified type.
 * @param varName String of the specified variable name.
 * @return A set of SIMPLE source line numbers.
 */
spa::LineNos spa::PKB::getModifies(Statement stmt, Name varName) {
  return LineNos();
}

/**
 * @brief Retrieves line numbers of statements of the specified statement type
 * that modifies any variables. E.g. Modifies(s, _)
 * @param stmt Statement with a specified type.
 * @param underscore Empty Underscore struct.
 * @return A set of SIMPLE source line numbers.
 */
spa::LineNos spa::PKB::getModifies(Statement stmt, Underscore underscore) {
  return LineNos();
}

/**
 * @brief Retrieves line numbers of statements of the specified statement type
 * and the variables modified in those statements. E.g. Modifies(s, v)
 * @param stmt Statement with a specified type.
 * @param variable Empty Variable struct.
 * @return A pair of vectors containing line numbers and variable names.
 */
spa::LineNo_Var_Pairs spa::PKB::getModifies(Statement stmt, Variable var) {
  return LineNo_Var_Pairs();
}

// Uses methods

/**
 * @brief Adds {lineNo, modifies} to the usesTable if 
 * lineNo is not in usesTable. 
 * @param lineNo Integer line number in the SIMPLE code.
 * @param uses Set of varNames used in the line number.
 */
void spa::PKB::addUses(LineNo lineNo, Uses uses) {
}

/**
 * @brief Checks whether the specified line number uses
 * the specified variable. E.g. Uses(6, "x")
 * @param lineNumber Struct of the line number in the SIMPLE code.
 * @param varName String of the specified variable name.
 * @return boolean.
 */
bool spa::PKB::getUses(LineNumber lineNumber, Name varName) {
  return false;
}

/**
 * @brief Checks whether the specified line number uses
 * any variables. E.g. Uses(6, _)
 * @param lineNumber Struct of the line number in the SIMPLE code.
 * @param underscore Empty Underscore struct.
 * @return boolean.
 */
bool spa::PKB::getUses(LineNumber lineNumber, Underscore underscore) {
  return false;
}

/**
 * @brief Retrieves variables used by the specified line number.
 * E.g. Uses(6, v)
 * @param lineNumber Struct of the line number in the SIMPLE code.
 * @param variable Empty Variable struct.
 * @return A set of variable names.
 */
spa::VarNames spa::PKB::getUses(LineNumber lineNumber, Variable var) {
  return VarNames();
}

/**
 * @brief Retrieves line numbers of statements of the specified statement type
 * that uses the specified variable. E.g. Uses(s, "x")
 * @param stmt Statement with a specified type.
 * @param varName String of the specified variable name.
 * @return A set of SIMPLE source line numbers.
 */
spa::LineNos spa::PKB::getUses(Statement stmt, Name varName) {
  return LineNos();
}

/**
 * @brief Retrieves line numbers of statements of the specified statement type
 * that uses any variables. E.g. Uses(s, _)
 * @param stmt Statement with a specified type.
 * @param underscore Empty Underscore struct.
 * @return A set of SIMPLE source line numbers.
 */
spa::LineNos spa::PKB::getUses(Statement stmt, Underscore underscore) {
  return LineNos();
}

/**
 * @brief Retrieves line numbers of statements of the specified statement type
 * and the variables used in those statements. E.g. Uses(s, v)
 * @param stmt Statement with a specified type.
 * @param variable Empty Variable struct.
 * @return A pair of vectors containing line numbers and variable names.
 */
spa::LineNo_Var_Pairs spa::PKB::getUses(Statement stmt, Variable var) {
  return LineNo_Var_Pairs();
}

// Assign methods

/**
 * @brief Adds {lineNo, Assignment} to the assignTable if
 * lineNo is not in assignTable.
 * @param lineNo Integer line number in the SIMPLE code.
 * @param varName Variable on the LHS of the assignment statement.
 * @param postfixString RHS of assignment statement string in postfix notation.
 */
void spa::PKB::addAssign(LineNo lineNo, VarName varName,
                         PostfixString postfixString) {
  assignTable.insert({ lineNo, Assignment({varName, postfixString}) });
}

/**
 * @brief Method to factorize out common code from getAssign methods.
 * @param pattern Pattern struct.
 * @return A set of SIMPLE source line numbers.
 */
spa::LineNos spa::PKB::getAssignCommonLogic(Pattern pattern,
                                            std::optional<Name> name) {
  spa::LineNos result;

  for (auto& [lineNo, assignment] : spa::PKB::assignTable) {
    if (name.has_value() && name.value().name != assignment.first) {
      continue;
    }
    const std::string postfix = assignment.second;
    const std::string patternValue = pattern.getValue();

    switch (pattern.getType()) {
    case EXACT:
      if (postfix == patternValue) {
        result.insert(lineNo);
      }
      break;
    case PARTIAL:
      if (postfix.find(patternValue) != std::string::npos) {
        result.insert(lineNo);
      }
      break;
    case ANY:
      result.insert(lineNo);
    }
  }

  return result;
}

/**
 * @brief Retrieves line numbers that are assignment statements where any
 * variable is on the LHS and the specified pattern matches the RHS.
 * @param underscore Empty Underscore struct.
 * @param pattern Pattern struct.
 * @return A set of SIMPLE source line numbers.
 */
spa::LineNos spa::PKB::getAssign(Underscore underscore, Pattern pattern) {
  return getAssignCommonLogic(pattern, std::nullopt);
}

/**
 * @brief Retrieves line numbers that are assignment statements where the
 * specified variable is on the LHS and the specified pattern matches the RHS.
 * @param variable Empty Variable struct.
 * @param pattern Pattern struct.
 * @return A set of SIMPLE source line numbers.
 */
spa::LineNos spa::PKB::getAssign(Variable variable, Pattern pattern) {
  return getAssignCommonLogic(pattern, std::nullopt);
}

/**
 * @brief Retrieves line numbers that are assignment statements where the
 * specified name is on the LHS and the specified pattern matches the RHS.
 * @param name Empty variable Name struct.
 * @param pattern Pattern struct.
 * @return A set of SIMPLE source line numbers.
 */
spa::LineNos spa::PKB::getAssign(Name name, Pattern pattern) {
  return getAssignCommonLogic(pattern, name);
}
