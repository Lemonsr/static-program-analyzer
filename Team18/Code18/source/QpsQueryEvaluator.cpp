#include "QpsQueryEvaluator.h"
#include "SimpleEvaluator.h"

#include <memory>

spa::QpsQueryEvaluator::QpsQueryEvaluator(ParsedQuery& parsedQuery) : parsedQuery(parsedQuery) {
}

spa::QpsResultTable spa::QpsQueryEvaluator::evaluate(PKBManager& pkbManager) {
  std::unique_ptr<QpsEvaluator> simpleEvaluator = std::make_unique<SimpleEvaluator>(parsedQuery.getSelectSynonymType());
  QpsResultTable simpleResultTable = simpleEvaluator->evaluate(pkbManager);

  QpsResultTable suchThatResultTable;
  if (parsedQuery.getSuchThatClause()) {
    SuchThatClause suchThatClause = parsedQuery.getSuchThatClause().value();
    suchThatResultTable = suchThatClause.getEvaluator()->evaluate(pkbManager);
  }
  
  QpsResultTable patternResultTable;
  if (parsedQuery.getPatternClause()) {
    PatternClause patternClause = parsedQuery.getPatternClause().value();
    patternResultTable = patternClause.getEvaluator()->evaluate(pkbManager);
  }
  
  simpleResultTable.innerJoin(suchThatResultTable);
  simpleResultTable.innerJoin(patternResultTable);
  return simpleResultTable;
}
