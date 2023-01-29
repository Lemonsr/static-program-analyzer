#include "PqlArgument.h"
#include "DesignEntity.h"

spa::PqlArgument::PqlArgument(ArgumentType type, std::string value,
  std::optional<DesignEntityType> designEntity) : type(type), value(value),
  designEntity(designEntity) {
}

const spa::ArgumentType& spa::PqlArgument::getType() {
  return type;
}

const std::string& spa::PqlArgument::getValue() {
  return value;
}

const std::optional<spa::DesignEntityType>&
    spa::PqlArgument::getDesignEntity() {
  return designEntity;
}

bool spa::operator==(const PqlArgument& p1, const PqlArgument& p2) {
  bool typeMatch = p1.type == p2.type;
  bool valueMatch = p1.value == p2.value;
  if (p1.designEntity.has_value() != p2.designEntity.has_value()) {
    return false;
  } else if (!p1.designEntity) {
    return typeMatch && valueMatch;
  } else {
    bool designEntityMatch = (p1.designEntity.value() ==
                              p2.designEntity.value());
    return typeMatch && valueMatch && designEntityMatch;
  }
}

bool spa::operator!=(const PqlArgument& p1, const PqlArgument& p2) {
  return !(p1 == p2);
}
