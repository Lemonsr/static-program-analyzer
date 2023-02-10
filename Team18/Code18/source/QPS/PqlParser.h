#pragma once

#include "Token.h"
#include "Stream.h"
#include "ParsedQuery.h"

namespace spa {

enum PqlParseStatus {
  PQL_PARSE_SUCCESS,
  PQL_PARSE_ERROR,
  PQL_PARSE_MISMATCH
};

class PqlParser {
 public:
  virtual PqlParseStatus parse(Stream<Token>& tokens, ParsedQuery& query) = 0;
};
}  // namespace spa

