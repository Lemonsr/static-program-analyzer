#include "PqlAttributeParser.h"

#include "PqlAttributes.h"

std::optional<std::string> spa::PqlAttributeParser::parseAttribute(Stream<Token>& tokens, ParsedQuery& query) {
  std::string synonym = tokens[0].getValue();
  DesignEntityType entityType = query.getDeclarationType(synonym);
  std::string attribute = tokens[2].getValue();
  bool hasHash = tokens.remaining() >= 4 && tokens[3].getType() == TOKEN_HASH;
  if (hasHash) {
    attribute.append("#");
  }
  auto& attributes = pqlAttributesMap[entityType];
  auto it = attributes.find(attribute);
  if (it == attributes.end()) {
    return {};
  }
  tokens.seek(3);
  if (hasHash) {
    tokens.seek(1);
  }
  query.addUsedDeclaration(synonym, entityType);
  return { synonym.append(".").append(attribute) };
}

std::optional<std::string> spa::PqlAttributeParser::parse(Stream<Token>& tokens, ParsedQuery& query) {
  bool matchStatus = tokens.match({
    { TOKEN_NAME, "" },
    { TOKEN_FULL_STOP, "."},
    { TOKEN_NAME, ""}
  });
  if (!matchStatus) {
    return {};
  }
  return parseAttribute(tokens, query);
}
