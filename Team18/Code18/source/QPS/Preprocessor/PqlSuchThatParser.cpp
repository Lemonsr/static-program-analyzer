#include "PqlSuchThatParser.h"

#include <string>
#include <unordered_map>

#include "ParsedQuery.h"
#include "Literal.h"

spa::PqlParseStatus spa::PqlSuchThatParser::parse(Stream<Token>& tokens,
                                                  ParsedQuery& query) {
  bool matchResult = tokens.match({
    { spa::TOKEN_NAME, SUCH_LITERAL},
    { spa::TOKEN_NAME, THAT_LITERAL},
  });
  if (!matchResult) {
    return spa::PQL_PARSE_MISMATCH;
  }
  tokens.seek(2);
  return subParser.parse(tokens, query);
}
