#pragma once

#include "PqlParser.h"
#include "PqlDeclarationParser.h"
#include "PqlSuchThatParser.h"
#include "PqlPatternParser.h"
#include "ParsedQuery.h"

namespace spa {
  class PqlQueryParser : public PqlParser {
  private:
    PqlDeclarationParser declareParser;
    PqlSuchThatParser suchThatParser;
    PqlPatternParser patternParser;
    PqlParseStatus parseDeclarations(Stream<Token>& tokens,
                                     ParsedQuery& query);
    PqlParseStatus parseSelect(Stream<Token>& tokens,
                               ParsedQuery& query);
    PqlParseStatus parseClauses(Stream<Token>& tokens,
                                ParsedQuery& query);
  public:
    PqlParseStatus parse(Stream<Token>& tokens, ParsedQuery& query);
  };
}  // namespace spa
