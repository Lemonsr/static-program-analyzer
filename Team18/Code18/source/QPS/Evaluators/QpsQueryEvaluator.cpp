#include "QpsQueryEvaluator.h"
#include "SimpleEvaluator.h"
#include "NoSynonymClauseGroup.h"
#include "QueryOptimizer.h"

#include <vector>
#include <memory>
#include <string>
#include <utility>

spa::QpsQueryEvaluator::QpsQueryEvaluator(ParsedQuery& parsedQuery) : parsedQuery(parsedQuery) {
}

spa::QpsResultTable spa::QpsQueryEvaluator::evaluate(PKBManager& pkbManager) {
  QueryOptimizer optimizer;
  ClauseGroups groups = optimizer.getGroups(parsedQuery);

  return groups.evaluate(pkbManager, parsedQuery.getUsedDeclarations());
}
