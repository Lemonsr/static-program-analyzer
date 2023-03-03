#include "QpsQueryEvaluator.h"
#include "SimpleEvaluator.h"

#include <vector>
#include <memory>
#include <string>

spa::QpsQueryEvaluator::QpsQueryEvaluator(ParsedQuery& parsedQuery) : parsedQuery(parsedQuery) {
}

spa::QpsResultTable spa::QpsQueryEvaluator::evaluate(PKBManager& pkbManager) {
  std::vector<std::unique_ptr<QpsEvaluator>> evaluators;
  for (auto& declarations : parsedQuery.getUsedDeclarations()) {
    std::string declaration = declarations.first;
    DesignEntityType declarationType = declarations.second;
    evaluators.push_back(
      std::make_unique<SimpleEvaluator>(declaration, declarationType));
  }
  for (auto& clause : parsedQuery.getSuchThatClauses()) {
    evaluators.push_back(clause.getEvaluator());
  }
  for (auto& clause : parsedQuery.getPatternClauses()) {
    evaluators.push_back(clause.getEvaluator());
  }
  QpsResultTable resultTable = evaluators[0]->evaluate(pkbManager);
  for (int i = 1; i < evaluators.size(); ++i) {
    resultTable = resultTable.innerJoin(evaluators[i]->evaluate(pkbManager));
  }
  return resultTable;
}
