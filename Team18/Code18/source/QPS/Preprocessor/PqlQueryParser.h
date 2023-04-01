#pragma once

#include "PqlParser.h"
#include "PqlDeclarationParser.h"
#include "PqlSelectParser.h"
#include "PqlSuchThatParser.h"
#include "PqlPatternParser.h"
#include "PqlAndParser.h"
#include "PqlWithParser.h"
#include "ParsedQuery.h"

namespace spa {
class PqlQueryParser : public PqlParser {
 private:
  PqlDeclarationParser declareParser;
  PqlSelectParser selectParser;
  PqlSuchThatParser suchThatParser;
  PqlPatternParser patternParser;
  PqlAndParser andParser;
  PqlWithParser withParser;
  PqlParseStatus parseDeclarations(Stream<Token>& tokens, ParsedQuery& query);
  PqlParseStatus parseClauses(Stream<Token>& tokens, ParsedQuery& query);
 public:
  PqlParseStatus parse(Stream<Token>& tokens, ParsedQuery& query);
};
}  // namespace spa
