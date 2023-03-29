#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "DesignEntity.h"
#include "Literal.h"

namespace spa {
inline std::unordered_map<DesignEntityType, std::vector<std::string>> pqlAttributesMap {
  { ASSIGN, { STMT_NUM_ATTR_LITERAL }},
  { CONSTANT, { VALUE_ATTR_LITERAL }},
  { STMT, { STMT_NUM_ATTR_LITERAL } },
  { READ, { STMT_NUM_ATTR_LITERAL, VAR_NAME_ATTR_LITERAL }},
  { PRINT, { STMT_NUM_ATTR_LITERAL, VAR_NAME_ATTR_LITERAL }},
  { CALL, { STMT_NUM_ATTR_LITERAL, PROC_NAME_ATTR_LITERAL }},
  { WHILE, { STMT_NUM_ATTR_LITERAL }},
  { IF, { STMT_NUM_ATTR_LITERAL }},
  { VARIABLE, { VAR_NAME_ATTR_LITERAL}},
  { PROCEDURE, { PROC_NAME_ATTR_LITERAL }},
  { UNKNOWN, { STMT_NUM_ATTR_LITERAL, VAR_NAME_ATTR_LITERAL, PROC_NAME_ATTR_LITERAL, VALUE_ATTR_LITERAL }},
};
}  // namespace spa
