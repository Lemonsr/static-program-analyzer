#include "PqlAttributeParser.h"

#include "PqlAttributes.h"
#include "Literal.h"

bool spa::PqlAttributeParser::attributeExists(DesignEntityType entityType, const std::string& attribute) {
  auto& v = pqlAttributesMap[entityType];
  for (auto& s : v) {
    if (s == attribute) {
      return true;
    }
  }
  return false;
}

std::optional<std::string> spa::PqlAttributeParser::parseAttribute(Stream<Token>& tokens, ParsedQuery& query) {
  std::string synonym = tokens[0].getValue();
  DesignEntityType entityType = query.getDeclarationType(synonym);
  std::string attribute = tokens[2].getValue();
  bool hasHash = tokens.remaining() >= 4 && tokens[3].getType() == TOKEN_HASH;
  if (hasHash) {
    attribute.append(HASH_LITERAL);
  }
  if (!attributeExists(entityType, attribute)) {
    return {};
  }
  tokens.seek(3);
  if (hasHash) {
    tokens.seek(1);
  }
  query.addUsedDeclaration(synonym, entityType);
  query.addSelectWithDeclaration(synonym, entityType);
  return { synonym.append(FULL_STOP_LITERAL).append(attribute) };
}

std::optional<std::string> spa::PqlAttributeParser::parse(Stream<Token>& tokens, ParsedQuery& query) {
  bool matchStatus = tokens.match({
    { TOKEN_NAME, EMPTY_LITERAL },
    { TOKEN_FULL_STOP, FULL_STOP_LITERAL },
    { TOKEN_NAME, EMPTY_LITERAL }
  });
  if (!matchStatus) {
    return {};
  }
  return parseAttribute(tokens, query);
}
