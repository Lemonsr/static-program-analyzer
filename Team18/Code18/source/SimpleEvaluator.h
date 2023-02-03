#pragma once

#include "PKBManager.h"
#include "QpsEvaluator.h"
#include "QpsResultTable.h"

namespace spa {
class SimpleEvaluator : public QpsEvaluator {
private:
  DesignEntityType designEntityType;
public:
  SimpleEvaluator(DesignEntityType designEntityType);
  QpsResultTable evaluate(PKBManager& pkbManager);
};
}  // namespace spa