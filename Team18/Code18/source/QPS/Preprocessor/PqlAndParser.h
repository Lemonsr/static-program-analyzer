#pragma once

#include "PqlParser.h"
#include "PqlSuchThatSubParser.h"
#include "PqlPatternSubParser.h"

namespace spa {
class PqlAndParser : public PqlParser {
 private:
  PqlSuchThatSubParser suchThatParser;
  PqlPatternSubParser patternParser;
 public:
  PqlParseStatus parse(Stream<Token>& tokens, ParsedQuery& query);
};
}  // namespace spa
