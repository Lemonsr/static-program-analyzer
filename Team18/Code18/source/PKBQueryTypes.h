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

enum PatternType {
  EXACT,
  PARTIAL,
  ANY
};

class Pattern {
 private:
  PatternType type;
  std::string value;
 public:
  explicit Pattern(PatternType type);
  Pattern(PatternType type, std::string value);
};
}  // namespace spa
