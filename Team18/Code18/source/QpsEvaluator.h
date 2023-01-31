#pragma once

#include "QpsPreprocessor.h"
#include "Token.h"
#include "PqlArgument.h"
#include "PKBQueryTypes.h"
#include "QueryResult.h"

namespace spa {
class QpsEvaluator {
 private:
  ParsedQuery queryObject;
 public:
  explicit QpsEvaluator(ParsedQuery queryObject);
  QueryResult evaluate();
};
}  // namespace spa
