#include "SimpleEvaluator.h"

spa::SimpleEvaluator::SimpleEvaluator(DesignEntityType designEntityType) : designEntityType(designEntityType) {
}

spa::QpsResultTable spa::SimpleEvaluator::evaluate(PKBManager& pkbManager) {
  return QpsResultTable();
}
