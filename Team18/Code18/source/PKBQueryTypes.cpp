#include "PKBQueryTypes.h"

spa::Pattern::Pattern(PatternType type) : type(type) {
}

spa::Pattern::Pattern(PatternType type, std::vector<Token> value)
  : type(type), value(value) {
}

bool spa::operator==(const spa::Pattern& p1, const spa::Pattern& p2) {
  bool typeMatch = p1.type == p2.type;
  if (p1.value.size() != p2.value.size()) {
    return false;
  }
  for (int i = 0; i < p1.value.size(); ++i) {
    if (p1.value[i] != p2.value[i]) {
      return false;
    }
  }
  return typeMatch;
}

bool spa::operator!=(const spa::Pattern& p1, const spa::Pattern& p2) {
  return !(p1 == p2);
}
