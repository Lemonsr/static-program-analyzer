#pragma once

#include <optional>

#include "ParsedQuery.h"
#include "PqlArgument.h"
#include "Stream.h"
#include "Token.h"

namespace spa {
  class PqlArgumentParser {
   private:
    std::optional<PqlArgument> getSynonym(Stream<Token>& tokens,
                                          ParsedQuery& query);
    std::optional<PqlArgument> getWildcard(Stream<Token>& tokens);
    std::optional<PqlArgument> getLineNo(Stream<Token>& tokens);
    std::optional<PqlArgument> getVariableName(Stream<Token>& tokens);
   public:
    std::optional<PqlArgument> parse(Stream<Token>& tokens,
                                     ParsedQuery& query);
  };
}  // namespace spa
