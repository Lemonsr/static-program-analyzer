#include "NextEvaluator.h"
#include "UtilsFunction.h"

spa::NextEvaluator::NextEvaluator(PqlArgument& firstArg, PqlArgument& secondArg) :
  firstArg(firstArg), secondArg(secondArg) {
}

spa::QpsResultTable spa::NextEvaluator::evaluate(PKBManager& pkbManager) {
  QpsResultTable table;
  table.addHeader(firstArg);
  table.addHeader(secondArg);
  if (UtilsFunction::isSameSynonym(firstArg, secondArg)) {
    return table;
  }

  QueryResult result = pkbManager.getRelationship(NEXT,
                                                  PKBQueryArg(firstArg),
                                                  PKBQueryArg(secondArg));
  if (result.getQueryResultType() == BOOL) {
    if (result.getIsTrue()) {
      table.addRow({ QpsValue(0), QpsValue(0) });
    }
  } else {
    for (auto& pair : result.getLineNumberLineNumberPairs()) {
      table.addRow({ QpsValue(pair.first), QpsValue(pair.second) });
    }
  }
  return table;
}
