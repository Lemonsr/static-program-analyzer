#include "ModifiesUsesEvaluator.h"
#include "UtilsFunction.h"

spa::ModifiesUsesEvaluator::ModifiesUsesEvaluator(PqlArgument& firstArg, PqlArgument& secondArg,
  RelationshipType designAbstraction) : firstArg(firstArg), secondArg(secondArg),
  designAbstraction(designAbstraction) {
}

spa::QpsResultTable spa::ModifiesUsesEvaluator::evaluate(PKBManager& pkbManager) {
  QpsResultTable table;
  table.addHeader(firstArg);
  table.addHeader(secondArg);

  QueryResult result = pkbManager.getRelationship(designAbstraction,
                                                  PKBQueryArg(firstArg),
                                                  PKBQueryArg(secondArg));
  if (result.getQueryResultType() == BOOL) {
    if (result.getIsTrue()) {
      table.addDummyRow();
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
