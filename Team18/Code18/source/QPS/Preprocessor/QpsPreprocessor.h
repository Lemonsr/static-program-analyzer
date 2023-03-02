#pragma once

#include <string>
#include <optional>
#include <unordered_map>

#include "ParsedQuery.h"

namespace spa {
class QpsPreprocessor {
 public:
  std::optional<ParsedQuery> preprocess(std::string query);
  void initializeUsedDeclarations(ParsedQuery& pq);
  void addUsedDeclaration(std::unordered_map<std::string, DesignEntityType>& usedDeclarations, PqlArgument& argument);
};
}

