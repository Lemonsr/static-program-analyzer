#include "PqlWithParser.h"
#include "Literal.h"

spa::PqlParseStatus spa::PqlWithParser::parse(Stream<Token>& tokens, ParsedQuery& query) {
  if (!tokens.match({ {TOKEN_NAME, WITH_LITERAL} })) {
    return PQL_PARSE_MISMATCH;
  }
  tokens.seek(1);
  return subParser.parse(tokens, query);
}
