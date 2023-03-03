#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <utility>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "RelationshipStorage.h"
#include "CFGNode.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace UnitTesting {
  TEST_CLASS(TestPKBCfg) {
    std::unordered_map<int, spa::CFGNode> cfgTable = {
      {1, {}},
      {2, {}}
    };

public:
  TEST_METHOD(TestAddCfg) {
    spa::RelationshipStorage relationshipStorage;
    relationshipStorage.setCfgTable(cfgTable);

    Assert::IsTrue(relationshipStorage.addCfgNode(3, {}));
    Assert::IsFalse(relationshipStorage.addCfgNode(3, {}));
    Assert::IsTrue(relationshipStorage.addCfgNode(4, {}));
  };
  };
}  // namespace UnitTesting
