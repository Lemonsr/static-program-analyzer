#include "ParentStarEvaluator.h"

spa::ParentStarEvaluator::ParentStarEvaluator(PqlArgument& firstArg, PqlArgument& secondArg) :
  firstArg(firstArg), secondArg(secondArg) {
}

spa::QpsResultTable spa::ParentStarEvaluator::evaluate(PKBManager& pkbManager) {
  return QpsResultTable();
}
