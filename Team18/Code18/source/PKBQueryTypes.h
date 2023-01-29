#pragma once

#include "PKBTypes.h"
#include "Token.h"

#include <string>
#include <optional>
#include <vector>

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
  std::vector<Token> value;
 public:
  explicit Pattern(PatternType type);
  Pattern(PatternType type, std::vector<Token> value);
  friend bool operator==(const Pattern& p1, const Pattern& p2);
  friend bool operator!=(const Pattern& p1, const Pattern& p2);
};
}  // namespace spa
