#include "FollowsStarEvaluator.h"

spa::FollowsStarEvaluator::FollowsStarEvaluator(PqlArgument& firstArg, PqlArgument& secondArg) :
  firstArg(firstArg), secondArg(secondArg) {
}

spa::QpsResultTable spa::FollowsStarEvaluator::evaluate(PKBManager& pkbManager) {
    return QpsResultTable();
}
