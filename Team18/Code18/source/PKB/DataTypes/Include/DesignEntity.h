#pragma once

namespace spa {
enum DesignEntityType {
  UNKNOWN,
  PROCEDURE,
  STMT,
  READ,
  PRINT,
  ASSIGN,
  CALL,
  WHILE,
  IF,
  VARIABLE,
  CONSTANT
};
}  // namespace spa
