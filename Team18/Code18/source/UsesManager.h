#include "UsesEvaluator.h"

spa::UsesEvaluator::UsesEvaluator(SuchThatClause& suchThatClause, PKBManager& pkbManager) :
  suchThatClause(suchThatClause), pkbManager(pkbManager) {
}

spa::QueryResult spa::UsesEvaluator::evaluate() {
  return QueryResult();
}
