#pragma once

#include "PKBManager.h"
#include "QpsEvaluator.h"
#include "QpsResultTable.h"

#include <string>

namespace spa {
class SimpleEvaluator : public QpsEvaluator {
 private:
  std::string selectSynonym;
  DesignEntityType designEntityType;
 public:
  explicit SimpleEvaluator(std::string selectSynonym, DesignEntityType designEntityType);
  QpsResultTable evaluate(PKBManager& pkbManager);
};
}  // namespace spa
