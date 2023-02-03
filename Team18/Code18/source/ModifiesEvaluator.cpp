#include "ModifiesEvaluator.h"

spa::ModifiesEvaluator::ModifiesEvaluator(PqlArgument& firstArg, PqlArgument& secondArg) :
  firstArg(firstArg), secondArg(secondArg) {
}

spa::QpsResultTable spa::ModifiesEvaluator::evaluate(PKBManager& pkbManager) {
    return QpsResultTable();
}
