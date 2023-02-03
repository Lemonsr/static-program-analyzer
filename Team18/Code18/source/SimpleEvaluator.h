#pragma once

#include "PKBManager.h"
#include "QpsEvaluator.h"
#include "QpsResultTable.h"

namespace spa {
class SimpleEvaluator : QpsEvaluator {
public:
  QpsResultTable evaluate(ParsedQuery& queryObject, PKBManager& pkbManager);
};
}  // namespace spa