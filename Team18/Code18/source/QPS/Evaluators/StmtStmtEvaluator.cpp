#include "StmtStmtEvaluator.h"
#include "UtilsFunction.h"

bool spa::StmtStmtEvaluator::isValidAffectsArgs() {
  for (auto arg : { firstArg, secondArg }) {
    if (arg.getType() == SYNONYM && arg.getDesignEntity() != ASSIGN && arg.getDesignEntity() != STMT) {
      return false;
    }
  }
  return true;
}

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

  if (designAbstraction == NEXT_STAR) {
    pkbManager.populateNextStar();
  }

  if (designAbstraction == AFFECTS) {
    pkbManager.populateAffects();
  }

  if (designAbstraction == AFFECTS_STAR) {
    pkbManager.populateAffectsStar();
  }

  if (designAbstraction == AFFECTS || designAbstraction == AFFECTS_STAR) {
    bool isValid = isValidAffectsArgs();
    if (!isValid) {
      return table;
    }
  }

  QueryResult result = pkbManager.getRelationship(designAbstraction,
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
