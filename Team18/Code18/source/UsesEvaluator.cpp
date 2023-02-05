#include "UsesEvaluator.h"

spa::UsesEvaluator::UsesEvaluator(PqlArgument& firstArg, PqlArgument& secondArg) :
  firstArg(firstArg), secondArg(secondArg) {
}

spa::QpsResultTable spa::UsesEvaluator::evaluate(PKBManager& pkbManager) {
  return QpsResultTable();
}
