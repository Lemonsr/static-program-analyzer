#pragma once

#include "PqlArgumentParser.h"
#include "PqlParser.h"
#include "ParsedQuery.h"

namespace spa {
  class PqlSuchThatParser : public PqlParser {
   private:
    PqlArgumentParser argParser;
    PqlParseStatus getArgs(RelationshipType type,
                           Stream<Token>& tokens,
                           ParsedQuery& query);
   public:
    PqlParseStatus parse(Stream<Token>& tokens, ParsedQuery& query);
  };
}  // namespace spa
