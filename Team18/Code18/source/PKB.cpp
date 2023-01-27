#include "PKB.h"
#include "PKBTypes.h"
#include "PKBQueryTypes.h"
#include "PKBTables.h"
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

class Pkb : public spa::PKBTables {
 public:
  const spa::VarTable& Pkb::getVarTable() const {
    return this->varTable;
  }

  const spa::ConstTable& Pkb::getConstTable() const {
    return this->constTable;
  }

  const spa::ProcTable& Pkb::getProcTable() const {
    return this->procTable;
  }

  const spa::FollowsTable& Pkb::getFollowsTable() const {
    return this->followsTable;
  }

  const spa::FollowsStarTable& Pkb::getFollowsStarTable() const {
    return this->followsStarTable;
  }

  const spa::ParentTable& Pkb::getParentTable() const {
    return this->parentTable;
  }

  const spa::ParentStarTable& Pkb::getParentStarTable() const {
    return this->parentStarTable;
  }

  const spa::ModifiesTable& Pkb::getModifiesTable() const {
    return this->modifiesTable;
  }

  const spa::ModifiesProcTable& Pkb::getModifiesProcTable() const {
    return this->modifiesProcTable;
  }

  const spa::UsesTable& Pkb::getUsesTable() const {
    return this->usesTable;
  }

  const spa::UsesProcTable& Pkb::getUsesProcTable() const {
     return this->usesProcTable;
  }

  const spa::AssignTable& Pkb::getAssignTable() const {
    return this->assignTable;
  }

  const spa::StatementTypeTable& Pkb::getStatementTypeTable() const {
     return this->statementTypeTable;
  }

  const spa::StatementProcTable& Pkb::getStatementProcTable() const {
     return this->statementProcTable;
  }
};
