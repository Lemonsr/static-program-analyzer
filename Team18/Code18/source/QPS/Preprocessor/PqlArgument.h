#pragma once

#include <string>
#include <optional>

#include "Argument.h"
#include "DesignEntity.h"

namespace spa {
class PqlArgument {
 private:
  ArgumentType type;
  std::string value;
  std::optional<DesignEntityType> designEntity;
 public:
  PqlArgument(ArgumentType type, std::string value, std::optional<DesignEntityType> designEntity);
  const ArgumentType& getType();
  const std::string& getValue();
  const std::optional<DesignEntityType>& getDesignEntity();

  friend bool operator==(const PqlArgument& p1, const PqlArgument& p2);
  friend bool operator!=(const PqlArgument& p1, const PqlArgument& p2);
};
}  // namespace spa
