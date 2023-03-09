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
    spa::CFGNode cfgNodeOne;
    spa::CFGNode cfgNodeTwo;
    std::unordered_map<int, spa::CFGNode> cfgTable;

public:
  TestPKBCfg() : cfgNodeOne(1), cfgNodeTwo(2) {
    cfgTable = {
      {1, cfgNodeOne},
      {2, cfgNodeTwo}
    };
  }

  TEST_METHOD(TestAddCfgNode) {
    spa::RelationshipStorage relationshipStorage;
    relationshipStorage.setCfgTable(cfgTable);
    spa::CFGNode cfgNodeTestOne(3);
    spa::CFGNode cfgNodeTestTwo(4);
    Assert::IsTrue(relationshipStorage.addCfgNode(3, cfgNodeTestOne));
    Assert::IsFalse(relationshipStorage.addCfgNode(3, cfgNodeTestOne));
    Assert::IsTrue(relationshipStorage.addCfgNode(4, cfgNodeTestTwo));
  }

  TEST_METHOD(TestUpdateCfgNode) {
    spa::RelationshipStorage relationshipStorage;
    relationshipStorage.setCfgTable(cfgTable);
    spa::CFGNode cfgNodeTestOne(3);
    spa::CFGNode cfgNodeTestTwo(4);
    spa::CFGNode cfgNodeTestThree(5);
    Assert::IsTrue(relationshipStorage.updateCfgNode(1, cfgNodeTestOne));
    Assert::IsFalse(relationshipStorage.updateCfgNode(3, cfgNodeTestTwo));
    Assert::IsTrue(relationshipStorage.updateCfgNode(1, cfgNodeTestThree));
  }

  TEST_METHOD(TestDeleteCfgNode) {
    spa::RelationshipStorage relationshipStorage;
    relationshipStorage.setCfgTable(cfgTable);
    spa::CFGNode cfgNodeTestOne(3);
    Assert::IsTrue(relationshipStorage.deleteCfgNode(1));
    Assert::IsFalse(relationshipStorage.deleteCfgNode(3));
    Assert::IsFalse(relationshipStorage.deleteCfgNode(1));
    Assert::IsTrue(relationshipStorage.addCfgNode(1, cfgNodeTestOne));
  }


  TEST_METHOD(TestGetCfgNode) {
    spa::RelationshipStorage relationshipStorage;
    relationshipStorage.setCfgTable(cfgTable);
    std::vector<spa::CFGNode> expected = { cfgNodeOne };

    spa::QueryResult queryResult = relationshipStorage.getCfgNode(1);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getCfgNodes());

    queryResult = relationshipStorage.getCfgNode(2);

    expected = { cfgNodeTwo };
    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getCfgNodes());

    queryResult = relationshipStorage.getCfgNode(3);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(queryResult.getCfgNodes().empty());
  }
  };
}  // namespace UnitTesting
