#pragma once

#include "ParsedQuery.h"
#include "QpsResultTable.h"
#include "PKBManager.h"

namespace spa {
class QpsQueryEvaluator : public QpsEvaluator {
 private:
  ParsedQuery& parsedQuery;
 public:
  explicit QpsQueryEvaluator(ParsedQuery& parsedQuery);
  QpsResultTable evaluate(PKBManager& pkbManager);
};
}  // namespace spa
