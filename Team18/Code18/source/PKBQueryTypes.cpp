#include "PKBQueryTypes.h"

spa::Pattern::Pattern(PatternType type) : type(type) {
}

spa::Pattern::Pattern(PatternType type, std::vector<Token> value)
  : type(type), value(value) {
}
