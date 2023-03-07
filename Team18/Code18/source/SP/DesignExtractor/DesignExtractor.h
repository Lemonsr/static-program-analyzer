#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "ContainerStatement.h"
#include "PKB.h"
#include "ProcedureStatement.h"

namespace spa {
class DesignExtractor {
 private:
  PKBManager& pkbManager;
  std::vector<ProcedureStatement>& procedureList;
  std::unordered_map<std::string, std::unordered_set<std::string>> procCallMap;

  void extractDesignAbstraction(std::vector<ProgramStatement*> statementList);
  void extractParentAbstraction(std::vector<ProgramStatement*> statementList);
  void extractFollows(std::vector<ProgramStatement*> statementList);
  void extractFollowsStar(std::vector<ProgramStatement*> statementList);
  void extractParent(ContainerStatement* containerStatement);
  void extractParentStar(ContainerStatement* containerStatement, std::string ancestorLineNum);
  void extractUsesAndModifies(std::vector<ProgramStatement*> statementList);
  void extractCallsStar();
  void extractUsesAndModifiesProc();
  void extractNestedProcUsesAndModifies();
  void extractCallsModifiesAndUses();

  void dfsCallsStar(std::string parent, std::string child);
  std::vector<std::pair<std::string, std::string>> getResFromPkbHelper(
    std::string procName, std::string synonym, DesignEntityType type, RelationshipType relType);
  void addUsesModifies(std::string relArg, std::vector<std::pair<std::string, std::string>> varUses,
                              std::vector<std::pair<std::string, std::string>> varModifies);
  void addUsesModifies(std::string relArg, std::vector<std::pair<std::string, std::string>> varUses,
                           std::vector<std::pair<std::string, std::string>> varModifies, bool isCallStmt);
 public:
  DesignExtractor(PKBManager& pkbManager, std::vector<ProcedureStatement>& procedureList);
  void extractRelationship();
};
}  // namespace spa
