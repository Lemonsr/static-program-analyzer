#include "PatternContainerEvaluator.h"

spa::PatternContainerEvaluator::PatternContainerEvaluator(DesignEntityType entityType, PqlArgument& patternSynonym,
                                                          PqlArgument& firstArg) :
  entityType(entityType), patternSynonym(patternSynonym), firstArg(firstArg) {
}


spa::QpsResultTable spa::PatternContainerEvaluator::evaluate(PKBManager& pkbManager) {
  QueryResult result = pkbManager.getContainerPattern(entityType, PKBQueryArg(firstArg));

  QpsResultTable resultTable;
  resultTable.addHeader(patternSynonym);
  resultTable.addHeader(firstArg);

  for (auto& [lineNumber, varName] : result.getLineNumberNamePairs()) {
    resultTable.addRow({ QpsValue(lineNumber), QpsValue(varName) });
  }
  return resultTable;
}
