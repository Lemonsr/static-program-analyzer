#pragma once

#include "QpsResultTable.h"

namespace spa {
class PKBManager;

class ParsedQuery;

class QpsEvaluator {
 public:
  virtual QpsResultTable evaluate(ParsedQuery& queryObject, PKBManager& pkbManager) = 0;
};
}  // namespace spa
