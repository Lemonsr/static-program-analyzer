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
    spa::CFGNode* cfgNodeOne = new spa::CFGNode(1);
    spa::CFGNode* cfgNodeTwo = new spa::CFGNode(2);
    std::unordered_map<int, spa::CFGNode> cfgTable = {
      {1, *cfgNodeOne},
      {2, *cfgNodeTwo}
    };

public:
  TEST_METHOD(TestAddCfgNode) {
    spa::RelationshipStorage relationshipStorage;
    relationshipStorage.setCfgTable(cfgTable);
    spa::CFGNode* cfgNodeTestOne = new spa::CFGNode(3);
    spa::CFGNode* cfgNodeTestTwo = new spa::CFGNode(4);
    Assert::IsTrue(relationshipStorage.addCfgNode(3, *cfgNodeTestOne));
    Assert::IsFalse(relationshipStorage.addCfgNode(3, *cfgNodeTestOne));
    Assert::IsTrue(relationshipStorage.addCfgNode(4, *cfgNodeTestTwo));
  }

  TEST_METHOD(TestGetCfgNode) {
    spa::RelationshipStorage relationshipStorage;
    relationshipStorage.setCfgTable(cfgTable);
    std::vector<spa::CFGNode> expected = { *cfgNodeOne };

    spa::QueryResult queryResult = relationshipStorage.getCfgNode(1);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getCfgNodes());

    queryResult = relationshipStorage.getCfgNode(2);

    expected = { *cfgNodeTwo };
    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getCfgNodes());

    queryResult = relationshipStorage.getCfgNode(3);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(queryResult.getCfgNodes().empty());
  }
  };
}  // namespace UnitTesting
