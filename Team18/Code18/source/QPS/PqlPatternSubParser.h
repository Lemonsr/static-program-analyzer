#pragma once

#include <optional>

#include "PqlArgumentParser.h"
#include "PqlParser.h"
#include "ParsedQuery.h"

namespace spa {
class PqlPatternSubParser : public PqlParser {
 private:
  PqlArgumentParser argParser;
  std::optional<Pattern> parseWildcard(Stream<Token>& tokens,
      ParsedQuery& query);
  std::optional<Pattern> parseExact(Stream<Token>& tokens,
      ParsedQuery& query);
  std::optional<Pattern> parsePartial(Stream<Token>& tokens,
      ParsedQuery& query);
  std::optional<Pattern> parsePattern(Stream<Token>& tokens,
      ParsedQuery& query);
  std::optional<PqlArgument> parseDesignEntity(Token& token, ParsedQuery& query);
 public:
  PqlParseStatus parse(Stream<Token>& tokens, ParsedQuery& query);
};
}  // namespace spa
