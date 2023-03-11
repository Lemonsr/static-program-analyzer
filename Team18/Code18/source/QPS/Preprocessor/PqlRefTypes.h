#pragma once

#include <unordered_set>

#include "ParsedQuery.h"

namespace spa {
inline const std::unordered_set<ArgumentType> stmtRef{
  spa::LINE_NO,
  spa::WILDCARD,
  spa::SYNONYM
};

inline const std::unordered_set<ArgumentType> entityRef{
  spa::LITERAL_STRING,
  spa::WILDCARD,
  spa::SYNONYM
};

inline const std::unordered_set<ArgumentType> stmtEntityRef{
  spa::LINE_NO,
  spa::WILDCARD,
  spa::SYNONYM,
  spa::LITERAL_STRING
};
}