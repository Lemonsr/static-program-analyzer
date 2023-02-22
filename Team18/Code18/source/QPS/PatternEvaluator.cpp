#include "PatternEvaluator.h"

spa::PatternEvaluator::PatternEvaluator(PqlArgument& assignSynonym, PqlArgument& firstArg, Pattern& pattern) :
  assignSynonym(assignSynonym), firstArg(firstArg), pattern(pattern) {
}

spa::QpsResultTable spa::PatternEvaluator::evaluate(PKBManager& pkbManager) {
  QueryResult result = pkbManager.getPattern(PKBQueryArg(firstArg), pattern);

  QpsResultTable resultTable;
  resultTable.addHeader(assignSynonym);
  resultTable.addHeader(firstArg);

  for (auto& [lineNumber, varName] : result.getLineNumberNamePairs()) {
    resultTable.addRow({ QpsValue(lineNumber), QpsValue(varName) });
  }
  return resultTable;
}
