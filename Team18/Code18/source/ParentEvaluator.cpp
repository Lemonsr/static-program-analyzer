#include "ParentEvaluator.h"

spa::ParentEvaluator::ParentEvaluator(PqlArgument& firstArg, PqlArgument& secondArg) :
  firstArg(firstArg), secondArg(secondArg) {
}

spa::QpsResultTable spa::ParentEvaluator::evaluate(PKBManager& pkbManager) {
  return QpsResultTable();
}
