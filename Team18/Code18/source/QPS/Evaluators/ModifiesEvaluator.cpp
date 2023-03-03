#include "ModifiesEvaluator.h"

spa::ModifiesEvaluator::ModifiesEvaluator(PqlArgument& firstArg, PqlArgument& secondArg) :
  firstArg(firstArg), secondArg(secondArg) {
}

spa::QpsResultTable spa::ModifiesEvaluator::evaluate(PKBManager& pkbManager) {
  QpsResultTable table;
  table.addHeader(firstArg);
  table.addHeader(secondArg);

  QueryResult result = pkbManager.getRelationship(MODIFIES,
                                                  PKBQueryArg(firstArg),
                                                  PKBQueryArg(secondArg));
  if (result.getQueryResultType() == BOOL) {
    if (result.getIsTrue()) {
      table.addRow({ QpsValue(0), QpsValue(0) });
    }
  } else {
    if (firstArg.getDesignEntity() == PROCEDURE ||
        firstArg.getType() == ArgumentType::LITERAL_STRING) {
      for (auto& pair : result.getNameNamePairs()) {
        table.addRow({ QpsValue(pair.first), QpsValue(pair.second) });
      }
    } else {
      for (auto& pair : result.getLineNumberNamePairs()) {
        table.addRow({ QpsValue(pair.first), QpsValue(pair.second) });
      }
    }
  }
  return table;
}
