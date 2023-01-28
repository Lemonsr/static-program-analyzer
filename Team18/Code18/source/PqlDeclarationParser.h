#pragma once

#include "PqlParser.h"

namespace spa {
class PqlDeclarationParser {
 private:
  PqlParseStatus addDeclarations(DesignEntityType type,
                                  Stream<Token>& tokens,
                                  ParsedQuery& query);
 public:
  PqlParseStatus parse(Stream<Token>& tokens, ParsedQuery& query);
};
}  // namespace spa
