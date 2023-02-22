#include "PqlAndParser.h"

spa::PqlParseStatus spa::PqlAndParser::parse(Stream<Token>& tokens, ParsedQuery& query) {
  if (!tokens.match({ {TOKEN_NAME, "and"} })) {
    return PQL_PARSE_MISMATCH;
  }
  if (!query.hasClauses()) {
    return PQL_PARSE_ERROR;
  }
  tokens.seek(1);
  const PqlClauseType lastAddedClause = query.getLastAddedClause();
  switch (lastAddedClause) {
    case PqlClauseType::SUCH_THAT_CLAUSE:
      return suchThatParser.parse(tokens, query);
    default:
      return patternParser.parse(tokens, query);
  }
}
