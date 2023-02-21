#include "PqlAttributeParser.h"

#include <unordered_map>
#include <unordered_set>

std::unordered_map<spa::DesignEntityType, std::unordered_set<std::string>> attributesMap {
  { spa::ASSIGN, { "stmt#" }},
  { spa::CONSTANT, { "value" }},
  { spa::STMT, { "stmt#" } },
  { spa::READ, { "stmt#", "varName" }},
  { spa::PRINT, { "stmt#", "varName" }},
  { spa::CALL, { "stmt#", "procName" }},
  { spa::WHILE, { "stmt#" }},
  { spa::IF, { "stmt#" }},
  { spa::VARIABLE, { "varName"}},
  { spa::PROCEDURE, { "procName" }}
};

std::optional<std::string> spa::PqlAttributeParser::parseAttribute(Stream<Token>& tokens, ParsedQuery& query) {
  std::string synonym = tokens[0].getValue();
  std::optional<DesignEntityType> entityOpt = query.getDeclarationType(synonym);
  if (!entityOpt) {
    return {};
  }
  std::string attribute = tokens[2].getValue();
  bool hasHash = tokens.remaining() >= 4 && tokens[3].getType() == TOKEN_HASH;
  if (hasHash) {
    attribute.append("#");
  }
  auto& attributes = attributesMap[entityOpt.value()];
  auto it = attributes.find(attribute);
  if (it == attributes.end()) {
    return {};
  }
  tokens.seek(3);
  if (hasHash) {
    tokens.seek(1);
  }
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
