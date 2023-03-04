#pragma once

#include "PqlParser.h"
#include "PqlAttributeParser.h"

#include <optional>

namespace spa {
class PqlWithSubParser : public PqlParser {
 private:
  PqlAttributeParser attribParser;
  std::optional<WithArgument> parseArgument(Stream<Token>& tokens, ParsedQuery& query);
 public:
  PqlParseStatus parse(Stream<Token>& tokens, ParsedQuery& query);
};
}  // namespace spa
