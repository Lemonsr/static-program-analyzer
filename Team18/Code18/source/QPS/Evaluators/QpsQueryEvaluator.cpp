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
  std::pair<NoSynonymClauseGroup, std::vector<ConnectedSynonymClauseGroup>> groups = optimizer.getGroups(parsedQuery);

  if (!groups.first.isEmpty()) {
    QpsResultTable noSynonymResultTable = groups.first.evaluate(pkbManager);
    if (noSynonymResultTable.isEmpty()) {
      return noSynonymResultTable;
    }
  }

  std::vector<QpsResultTable> resultTables;
  for (auto& group : groups.second) {
    resultTables.push_back(group.evaluate(pkbManager));
  }

  for (auto& declaration : parsedQuery.getUsedDeclarations()) {
    std::string declarationSynonym = declaration.first;
    DesignEntityType declarationType = declaration.second;
    std::unique_ptr<QpsEvaluator> evaluator = std::make_unique<SimpleEvaluator>(declarationSynonym, declarationType);
    resultTables.push_back(evaluator->evaluate(pkbManager));
  }

  QpsResultTable resultTable = resultTables[0];
  for (int i = 1; i < resultTables.size(); i++) {
    resultTable = resultTable.innerJoin(resultTables[i]);
  }
  return resultTable;
}
