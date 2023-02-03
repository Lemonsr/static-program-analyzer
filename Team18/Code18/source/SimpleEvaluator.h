#pragma once

#include "PKBManager.h"
#include "QpsEvaluator.h"
#include "QpsResultTable.h"

namespace spa {
class SimpleEvaluator {
private:
  DesignEntityType designEntityType;
public:
  SimpleEvaluator(DesignEntityType designEntityType);
  QueryResult evaluate();
};
}  // namespace spa