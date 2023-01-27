#pragma once

#include <string>

#include "Argument.h"

namespace spa {
class PqlArgument {
 private:
  ArgumentType type;
  std::string value;
 public:
  PqlArgument(ArgumentType type, std::string value);
  const ArgumentType& getType();
  const std::string& getValue();

  friend bool operator==(const PqlArgument& p1, const PqlArgument& p2);
  friend bool operator!=(const PqlArgument& p1, const PqlArgument& p2);
};
}  // namespace spa
