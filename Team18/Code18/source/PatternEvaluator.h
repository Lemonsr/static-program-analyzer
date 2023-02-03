#pragma once

#include "PKBManager.h"
#include "QpsEvaluator.h"
#include "QpsResultTable.h"

namespace spa {
class PatternEvaluator {
private:
  PatternClause& patternClause;
  PKBManager& pkbManager;
public:
  PatternEvaluator(PatternClause& patternClause, PKBManager& pkbManager);
  QueryResult evaluate();
};
}  // namespace spa
