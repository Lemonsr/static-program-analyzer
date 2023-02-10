#include "UsesEvaluator.h"

spa::UsesEvaluator::UsesEvaluator(PqlArgument& firstArg, PqlArgument& secondArg) :
  firstArg(firstArg), secondArg(secondArg) {
}

spa::QpsResultTable spa::UsesEvaluator::evaluate(PKBManager& pkbManager) {
  QpsResultTable table;
  table.addHeader(firstArg);
  table.addHeader(secondArg);
  QueryResult result = pkbManager.getRelationship(USES,
                                                  PKBQueryArg(firstArg),
                                                  PKBQueryArg(secondArg));
  if (result.getQueryResultType() == BOOL) {
    if (result.getIsTrue()) {
      table.addRow({ QpsValue(0), QpsValue(0) });
    }
  } else {
    for (auto& pair : result.getLineNumberVariablePairs()) {
      table.addRow({ QpsValue(pair.first), QpsValue(pair.second) });
    }
  }
  return table;
}
