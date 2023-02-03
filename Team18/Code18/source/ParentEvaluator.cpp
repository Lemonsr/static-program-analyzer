#include "ParentEvaluator.h"

spa::ParentEvaluator::ParentEvaluator(SuchThatClause& suchThatClause, PKBManager& pkbManager) :
  suchThatClause(suchThatClause), pkbManager(pkbManager) {
}

spa::QueryResult spa::ParentEvaluator::evaluate() {
  return QueryResult();
}
