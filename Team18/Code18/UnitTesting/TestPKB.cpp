#include <iostream>
#include <sstream>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "PKB.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace UnitTesting {
TEST_CLASS(TestPKB) {
 public:
  TEST_METHOD(TestPKBAddAssign) {
    spa::PKB pkb;

    pkb.addAssign(6, "x", "1 2 +");
    spa::AssignTable assignTable = pkb.getAssignTable();

    bool isKeyExist = assignTable.find(6) != assignTable.end();
    Assert::IsTrue(isKeyExist);
    bool isMatchLHS = assignTable.at(6).first == "x";
    Assert::IsTrue(isMatchLHS);
    bool isMatchRHS = assignTable.at(6).second == "1 2 +";
    Assert::IsTrue(isMatchRHS);
  }
};
}  // namespace UnitTesting
