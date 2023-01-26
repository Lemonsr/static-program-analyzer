#pragma once

#include <vector>

#include "PqlTokenizer.h"

struct QueryObject {
};

class PqlParser {
 public:
  PqlParser(std::vector<PqlToken> tokens);
  QueryObject parse();
 private:
  std::vector<PqlToken> tokens;
};
