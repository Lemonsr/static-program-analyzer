#pragma once

#include "QpsPreprocessor.h"

struct QueryResult {
};

class QpsEvaluator {
 public:
  explicit QpsEvaluator(QueryObject queryObject);
  QueryResult evaluate();
 private:
  QueryObject queryObject;
};
