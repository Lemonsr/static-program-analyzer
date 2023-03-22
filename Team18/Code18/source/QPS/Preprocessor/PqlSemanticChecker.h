#pragma once

#include "ParsedQuery.h"

namespace spa {
class PqlSemanticChecker {
 private:
  bool isValid(SuchThatClause& suchThatClause);
  bool isValid(PatternClause& patternClause);
  bool isValid(WithClause& withClause);
  bool checkModifiesArguments(PqlArgument& firstArg, PqlArgument& secondArg);
  bool checkUsesArguments(PqlArgument& firstArg, PqlArgument& secondArg);
  bool checkStmtRefArguments(PqlArgument& firstArg, PqlArgument& secondArg);
  bool checkCallsArguments(PqlArgument& firstArg, PqlArgument& secondArg);
  QpsValueType getWithArgumentType(WithArgument& withArgument);
 public:
  bool isSemanticallyValid(ParsedQuery& parsedQuery);
};
}  // namespace spa
