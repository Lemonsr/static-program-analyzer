#include "QpsQueryEvaluator.h"
#include "SimpleEvaluator.h"

#include <memory>

spa::QpsQueryEvaluator::QpsQueryEvaluator(ParsedQuery& parsedQuery) : parsedQuery(parsedQuery) {
}

spa::QpsResultTable spa::QpsQueryEvaluator::evaluate(PKBManager& pkbManager) {
  std::unique_ptr<QpsEvaluator> simpleEvaluator =
    std::make_unique<SimpleEvaluator>(parsedQuery.getSelectSynonymType());
  QpsResultTable resultTable = simpleEvaluator->evaluate(pkbManager);

  if (parsedQuery.getSuchThatClause()) {
    SuchThatClause suchThatClause = parsedQuery.getSuchThatClause().value();
    resultTable.innerJoin(suchThatClause.getEvaluator()->evaluate(pkbManager));
  }

  if (parsedQuery.getPatternClause()) {
    PatternClause patternClause = parsedQuery.getPatternClause().value();
    resultTable.innerJoin(patternClause.getEvaluator()->evaluate(pkbManager));
  }

  return resultTable;
}
