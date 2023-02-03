#include "PatternEvaluator.h"

spa::PatternEvaluator::PatternEvaluator(PatternClause& patternClause, PKBManager& pkbManager) :
  patternClause(patternClause), pkbManager(pkbManager) {

}

spa::QueryResult spa::PatternEvaluator::evaluate() {
  return QueryResult();
}
