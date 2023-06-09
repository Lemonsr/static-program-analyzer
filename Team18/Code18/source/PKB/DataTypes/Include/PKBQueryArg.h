#pragma once

#include <optional>

#include "PKBQueryTypes.h"

namespace spa {
enum class PKBQueryArgType {
  CONSTANT,
  VARIABLE,
  STATEMENT,
  PROCEDURE,
  LINE_NUMBER,
  NAME,
  UNDERSCORE
};

class PqlArgument;

class PKBQueryArg {
 private:
  PKBQueryArgType type;
  std::optional<Constant> constantOpt;
  std::optional<Variable> variableOpt;
  std::optional<Statement> statementOpt;
  std::optional<Procedure> procedureOpt;
  std::optional<LineNumber> lineNumberOpt;
  std::optional<Name> nameOpt;
  std::optional<Underscore> underscoreOpt;
 public:
  explicit PKBQueryArg(PqlArgument& pqlArg);
  const PKBQueryArgType& getType();
  const Constant& getConstant();
  const Variable& getVariable();
  const Statement& getStatement();
  const Procedure& getProcedure();
  const LineNumber& getLineNumber();
  const Name& getName();
  const Underscore& getUnderscore();
};
}  // namespace spa
