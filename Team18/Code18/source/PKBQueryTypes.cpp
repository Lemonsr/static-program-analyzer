#include "PKBQueryTypes.h"

spa::Pattern::Pattern(PatternType type) : type(type) {
}

spa::Pattern::Pattern(PatternType type, std::string value) : type(type), value(value) {
}
