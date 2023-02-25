#pragma once

#include "ParsedQuery.h";

namespace spa {
class PqlSemanticChecker {
 private:
  bool isValid(SuchThatClause& suchThatClause);
  bool isValid(PatternClause& patternClause);
  bool checkModifiesArguments(PqlArgument& firstArg, PqlArgument& secondArg);
  bool checkUsesArguments(PqlArgument& firstArg, PqlArgument& secondArg);
  bool checkParentArguments(PqlArgument& firstArg, PqlArgument& secondArg);
  bool checkFollowsArguments(PqlArgument& firstArg, PqlArgument& secondArg);
 public:
  bool isSemanticallyValid(ParsedQuery& parsedQuery);
};
}  // namespace spa
