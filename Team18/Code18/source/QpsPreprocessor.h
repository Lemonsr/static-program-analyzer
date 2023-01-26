#pragma once

#include <string>
#include "PqlTokenizer.h"
#include "PqlParser.h"

class QpsPreprocessor {
 public:
  QueryObject preprocess(std::string query);
};
