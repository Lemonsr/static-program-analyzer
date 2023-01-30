#pragma once

#include <string>
#include <optional>

#include "ParsedQuery.h"

namespace spa {
class QpsPreprocessor {
 public:
  std::optional<ParsedQuery> preprocess(std::string query);
};
}

