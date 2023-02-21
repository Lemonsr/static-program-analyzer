#pragma once

#include <optional>

#include "PqlPatternSubParser.h"
#include "PqlParser.h"
#include "ParsedQuery.h"

namespace spa {
class PqlPatternParser : public PqlParser {
 private:
  PqlPatternSubParser subParser;
 public:
  PqlParseStatus parse(Stream<Token>& tokens, ParsedQuery& query);
};
}  // namespace spa
