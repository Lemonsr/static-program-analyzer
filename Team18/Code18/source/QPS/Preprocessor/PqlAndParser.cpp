#include "PqlAndParser.h"

spa::PqlParseStatus spa::PqlAndParser::parse(Stream<Token>& tokens, ParsedQuery& query) {
  if (!tokens.match({ {TOKEN_NAME, "and"} })) {
    return PQL_PARSE_MISMATCH;
  }
  if (!query.hasClauses()) {
    return PQL_PARSE_SYNTAX_ERROR;
  }
  tokens.seek(1);
  const PqlClauseType lastAddedClause = query.getLastAddedClause();
  switch (lastAddedClause) {
    case PqlClauseType::SUCH_THAT_CLAUSE:
      return suchThatParser.parse(tokens, query);
    case PqlClauseType::PATTERN_CLAUSE:
      return patternParser.parse(tokens, query);
    case PqlClauseType::WITH_CLAUSE:
      return withParser.parse(tokens, query);
    default:
      return PQL_PARSE_SYNTAX_ERROR;
  }
}
