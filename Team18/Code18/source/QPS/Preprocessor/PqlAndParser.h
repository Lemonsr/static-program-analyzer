#pragma once

#include "PqlParser.h"
#include "PqlSuchThatSubParser.h"
#include "PqlPatternSubParser.h"
#include "PqlWithSubParser.h"

namespace spa {
class PqlAndParser : public PqlParser {
 private:
  PqlSuchThatSubParser suchThatParser;
  PqlPatternSubParser patternParser;
  PqlWithSubParser withParser;
 public:
  PqlParseStatus parse(Stream<Token>& tokens, ParsedQuery& query);
};
}  // namespace spa
