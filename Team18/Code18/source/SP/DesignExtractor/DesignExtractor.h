#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

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
  std::optional<std::vector<std::string>> getResFromPkbHelper(
    std::string procName, std::string synonym, DesignEntityType type);
  void addUsesModifiesAndProc(std::string relArg, std::optional<std::vector<std::string>> varUses, std::optional<std::vector<std::string>> varModifies, bool isByProc);
 public:
  DesignExtractor(PKBManager& pkbManager, std::vector<ProcedureStatement>& procedureList);
  void extractRelationship();
};
}  // namespace spa
