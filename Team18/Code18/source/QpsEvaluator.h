#pragma once

#include "QpsPreprocessor.h"
#include "Token.h"
#include "PqlArgument.h"
#include "PKBQueryTypes.h"

namespace spa {
class QueryResult {
};

class QpsEvaluator {
 private:
  ParsedQuery queryObject;
 public:
  explicit QpsEvaluator(ParsedQuery queryObject);
  QueryResult evaluate();
};
}  // namespace spa
