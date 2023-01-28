#pragma once

#include "PKBTypes.h"

#include <string>
#include <optional>

namespace spa {
struct Variable { };

struct Statement {
  std::optional<StatementType> statementType;
};

struct Procedure { };

struct LineNumber { };

// Procedure / Variable name
struct Name {
  std::string name;
};

struct Underscore { };
}  // namespace spa
