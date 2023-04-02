#pragma once

#include "PqlParser.h"
#include "PqlWithSubParser.h"

namespace spa {
class PqlWithParser : public PqlParser {
 private:
  PqlWithSubParser subParser;
 public:
  PqlParseStatus parse(Stream<Token>& tokens, ParsedQuery& query);
};
}  // namespace spa
