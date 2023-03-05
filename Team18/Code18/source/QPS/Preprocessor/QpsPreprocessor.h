#pragma once

#include <string>
#include <utility>

#include "PqlParser.h"
#include "ParsedQuery.h"

namespace spa {
class QpsPreprocessor {
 public:
  std::pair<PqlParseStatus, ParsedQuery> preprocess(std::string query);
};
}
