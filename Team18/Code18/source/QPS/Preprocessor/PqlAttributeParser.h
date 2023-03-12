#pragma once

#include <string>
#include <optional>

#include "Stream.h"
#include "Token.h"
#include "ParsedQuery.h"

namespace spa {
class PqlAttributeParser {
 private:
  bool attributeExists(DesignEntityType entityType, const std::string& attribute);
  std::optional<std::string> parseAttribute(Stream<Token>& tokens, ParsedQuery& query);
 public:
  std::optional<std::string> parse(Stream<Token>& tokens, ParsedQuery& query);
};
}  // namespace spa
