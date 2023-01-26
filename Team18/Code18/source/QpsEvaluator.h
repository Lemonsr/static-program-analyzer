#pragma once

#include "QpsPreprocessor.h"

struct QueryResult {
};

class QpsEvaluator {
 public:
  QpsEvaluator(QueryObject queryObject);
  QueryResult evaluate();
 private:
  QueryObject queryObject;
};
