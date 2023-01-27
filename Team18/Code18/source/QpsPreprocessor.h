#pragma once

#include <string>
#include "PqlParser.h"

namespace spa {
class QpsPreprocessor {
 public:
  ParsedQuery preprocess(std::string query);
};
}

