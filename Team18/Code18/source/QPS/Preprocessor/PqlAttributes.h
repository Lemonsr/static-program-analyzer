#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

#include "DesignEntity.h"

namespace spa {
inline std::unordered_map<DesignEntityType, std::unordered_set<std::string>> pqlAttributesMap {
  { ASSIGN, { "stmt#" }},
  { CONSTANT, { "value" }},
  { STMT, { "stmt#" } },
  { READ, { "stmt#", "varName" }},
  { PRINT, { "stmt#", "varName" }},
  { CALL, { "stmt#", "procName" }},
  { WHILE, { "stmt#" }},
  { IF, { "stmt#" }},
  { VARIABLE, { "varName"}},
  { PROCEDURE, { "procName" }}
};
}  // namespace spa
