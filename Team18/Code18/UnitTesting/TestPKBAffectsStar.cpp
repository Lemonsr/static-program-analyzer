#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <utility>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "RelationshipStorage.h"
#include "PqlArgument.h"
#include "Argument.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace UnitTesting {
  TEST_CLASS(TestPKBAffectsStar) {
    std::unordered_map<int, std::unordered_set<int>> affectsStarTable = {
      {1, {3, 4}},
      {2, {2, 3, 4}},
      {3, {3, 4}},
      {4, {4}}
    };

    std::unordered_map<int, spa::StatementType> statementTypeTable = {
      {1, spa::StatementType::ASSIGN},
      {2, spa::StatementType::ASSIGN},
      {3, spa::StatementType::ASSIGN},
      {4, spa::StatementType::ASSIGN}
    };
public:
  
  };
}  // namespace UnitTesting
