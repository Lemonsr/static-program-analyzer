#pragma once

#include "PKBTypes.h"

#include <string>
#include <optional>

namespace spa {

/**
 * A struct for a declared variable in a PQL query. E.g. variable v.
 */
struct Variable { };

/**
 * A struct for a declared statement in a PQL query.
 * If the statement type is in StatementType enum, statementType will have a
 * value (E.g. assign a).
 * Else, statementType is left optional (E.g. statement s).
 */
struct Statement {
  std::optional<StatementType> statementType;
};

/**
 * A struct for a declared procedure in a PQL query. E.g. procedure p.
 */
struct Procedure { };

/**
 * A struct for a line number in a PQL query.
 */
struct LineNumber { };

/**
 * A struct for a procedure or variable name in a PQL query. E.g. "x", "main".
 */
struct Name {
  std::string name;
};

/**
 * A struct for an underscore used in a PQL query. E.g. _
 */
struct Underscore { };

enum PatternType {
  EXACT,
  PARTIAL,
  ANY
};

class Pattern {
 private:
  const PatternType type;
  const std::string value;
 public:
  explicit Pattern(PatternType type);
  Pattern(PatternType type, std::string value);
};
}  // namespace spa
