#include "StmtStmtEvaluator.h"
#include "UtilsFunction.h"

spa::StmtStmtEvaluator::StmtStmtEvaluator(PqlArgument& firstArg, PqlArgument& secondArg,
  RelationshipType designAbstraction) : firstArg(firstArg), secondArg(secondArg),
  designAbstraction(designAbstraction) {
}

spa::QpsResultTable spa::StmtStmtEvaluator::evaluate(PKBManager& pkbManager) {
  QpsResultTable table;
  table.addHeader(firstArg);
  table.addHeader(secondArg);
  if (UtilsFunction::isSameSynonym(firstArg, secondArg)) {
    return table;
  }

  QueryResult result = pkbManager.getRelationship(designAbstraction, PKBQueryArg(firstArg), PKBQueryArg(secondArg));
  if (result.getQueryResultType() == BOOL) {
    if (result.getIsTrue()) {
      table.addDummyRow();
    }
  } else {
    for (auto& pair : result.getLineNumberLineNumberPairs()) {
      table.addRow({ QpsValue(pair.first), QpsValue(pair.second) });
    }
  }
  return table;
}
