#pragma once

#include <string>
#include "ParsedQuery.h"

namespace spa {
class QpsPreprocessor {
 public:
  ParsedQuery preprocess(std::string query);
};
}

