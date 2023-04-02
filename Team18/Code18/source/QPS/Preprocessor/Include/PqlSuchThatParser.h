#pragma once

#include "PqlParser.h"
#include "PqlSuchThatSubParser.h"

namespace spa {
class PqlSuchThatParser : public PqlParser {
 private:
  PqlSuchThatSubParser subParser;
 public:
  PqlParseStatus parse(Stream<Token>& tokens, ParsedQuery& query);
};
}  // namespace spa
