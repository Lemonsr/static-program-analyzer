#include "PatternEvaluator.h"

spa::PatternEvaluator::PatternEvaluator(PqlArgument& assignSynonym, PqlArgument& firstArg, Pattern& pattern) :
  assignSynonym(assignSynonym), firstArg(firstArg), pattern(pattern) {
}

spa::QpsResultTable spa::PatternEvaluator::evaluate(PKBManager& pkbManager) {
  return QpsResultTable();
}
