#include "CFGEvaluator.h"
#include "UtilsFunction.h"

bool spa::CFGEvaluator::isValidAffectsArgs() {
  for (auto arg : { firstArg, secondArg }) {
    if (arg.getType() == SYNONYM && arg.getDesignEntity() != ASSIGN && arg.getDesignEntity() != STMT) {
      return false;
    }
  }
  return true;
}

spa::CFGEvaluator::CFGEvaluator(PqlArgument& firstArg, PqlArgument& secondArg,
  RelationshipType designAbstraction) : firstArg(firstArg), secondArg(secondArg),
  designAbstraction(designAbstraction) {
}

spa::QpsResultTable spa::CFGEvaluator::evaluate(PKBManager& pkbManager) {
  QpsResultTable table;
  table.addHeader(firstArg);
  table.addHeader(secondArg);

  switch (designAbstraction) {
  case NEXT_STAR: {
    pkbManager.populateNextStar();
    break;
  }
  case AFFECTS: {
    pkbManager.populateAffects();
    break;
  }
  case AFFECTS_STAR: {
    pkbManager.populateAffectsStar();
    break;
  }
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
    bool isSameSynonym = UtilsFunction::isSameSynonym(firstArg, secondArg);
    for (auto& pair : result.getLineNumberLineNumberPairs()) {
      if (isSameSynonym && pair.first != pair.second) {
        continue;
      }
      table.addRow({ QpsValue(pair.first), QpsValue(pair.second) });
    }
  }
  return table;
}
