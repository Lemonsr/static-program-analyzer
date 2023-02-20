#include "PqlPatternParser.h"

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include "ParsedQuery.h"
#include "Token.h"

spa::PqlParseStatus spa::PqlPatternParser::parse(Stream<Token>& tokens,
                                                 ParsedQuery& query) {
  bool matchResult = tokens.match({
    { spa::TOKEN_NAME, "pattern"}
  });
  if (!matchResult) {
    return PQL_PARSE_MISMATCH;
  }
  tokens.seek(1);
  return subParser.parse(tokens, query);
}
