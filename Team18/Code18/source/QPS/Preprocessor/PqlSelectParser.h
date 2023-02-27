#pragma once

#include "PqlParser.h"
#include "PqlAttributeParser.h"

#include <optional>

namespace spa {
class PqlSelectParser : public PqlParser {
 private:
  PqlAttributeParser attributeParser;
  PqlParseStatus parseSynonymOrAttribute(bool parseBoolean, Stream<Token>& tokens, ParsedQuery& query);
  PqlParseStatus parseTuple(Stream<Token>& tokens, ParsedQuery& query);
 public:
  PqlParseStatus parse(Stream<Token>& tokens, ParsedQuery& query);
};
}  // namespace spa
