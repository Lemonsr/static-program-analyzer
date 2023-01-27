#include "PqlArgument.h"

spa::PqlArgument::PqlArgument(ArgumentType type, std::string value) : type(type), value(value) {
}

const spa::ArgumentType& spa::PqlArgument::getType() {
  return type;
}

const std::string& spa::PqlArgument::getValue() {
  return value;
}

bool spa::operator==(const PqlArgument& p1, const PqlArgument& p2)
{
  bool typeMatch = p1.type == p2.type;
  bool valueMatch = p1.value == p2.value;
  return typeMatch && valueMatch;
}

bool spa::operator!=(const PqlArgument& p1, const PqlArgument& p2)
{
  return !(p1 == p2);
}
