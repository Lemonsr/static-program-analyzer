#include "FollowsEvaluator.h"

spa::FollowsEvaluator::FollowsEvaluator(PqlArgument& firstArg, PqlArgument& secondArg) :
  firstArg(firstArg), secondArg(secondArg) {
}

spa::QpsResultTable spa::FollowsEvaluator::evaluate(PKBManager& pkbManager) {
  return QpsResultTable();
}
