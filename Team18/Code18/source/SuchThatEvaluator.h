#pragma once

#include "PKBManager.h"
#include "QpsEvaluator.h"
#include "QpsResultTable.h"

namespace spa {
class SuchThatEvaluator : QpsEvaluator {
  virtual QueryResult evaluate() = 0;
};
}
