#pragma once

#include "PKBTypes.h"
#include "Token.h"
#include "DesignEntity.h"

#include <string>
#include <optional>
#include <vector>
#include <unordered_map>

namespace spa {

/**
  * A struct for a declared constant in a PQL query. E.g. constant c.
  */
struct Constant {};

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
struct LineNumber {
  LineNo lineNo;
};

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
  PatternType type;
  std::vector<Token> value;
 public:
  explicit Pattern(PatternType type);
  Pattern(PatternType type, std::vector<Token> value);
  const PatternType& getType() {
    return this->type;
  }

  const std::vector<Token>& getValue() {
    return this->value;
  }
  friend bool operator==(const Pattern& p1, const Pattern& p2);
  friend bool operator!=(const Pattern& p1, const Pattern& p2);
};

inline std::unordered_map<DesignEntityType,
                          std::optional<StatementType>> statementTypeMap {
  { STMT, {} },
  { READ, {StatementType::READ} },
  { PRINT, {StatementType::PRINT} },
  { ASSIGN, {StatementType::ASSIGN} },
  { CALL, {StatementType::CALL} },
  { WHILE, {StatementType::WHILE} },
  { IF, {StatementType::IF} }
};
}  // namespace spa
