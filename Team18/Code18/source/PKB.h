#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "PKBTables.h"

typedef int16_t PROC;

namespace spa {
class TNode;

class PKB {
 public:
  static VarTable* varTable;
  static int setProcToAST(PROC p, TNode* r);
  static TNode* getRootAST(PROC p);
};
}  // namespace spa
