#pragma once

#include "QpsPreprocessor.h"
#include "Token.h"
#include "PqlArgument.h"
#include "Relationship.h"
#include "PKBQueryTypes.h"

namespace spa {
class QueryResult {
};

class QpsEvaluator {
 public:
  explicit QpsEvaluator(ParsedQuery queryObject);
  QueryResult evaluate();
 private:
  ParsedQuery queryObject;
};
}  // namespace spa
