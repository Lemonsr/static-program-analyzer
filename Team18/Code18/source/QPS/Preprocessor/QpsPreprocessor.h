#pragma once

#include <string>
#include <utility>
#include <optional>

#include "PqlParser.h"
#include "ParsedQuery.h"

namespace spa {
class QpsPreprocessor {
 public:
  std::pair<PqlParseStatus, ParsedQuery> preprocess(std::string query);
  std::optional<Stream<Token>> tokenize(std::string query);
  PqlParseStatus parse(Stream<Token>& tokens, ParsedQuery& query);
  bool checkSemantics(ParsedQuery& query);
};
}
