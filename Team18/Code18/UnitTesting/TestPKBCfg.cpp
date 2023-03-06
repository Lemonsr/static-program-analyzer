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

    Assert::IsTrue(relationshipStorage.addCfgNode(3, {}));
    Assert::IsFalse(relationshipStorage.addCfgNode(3, {}));
    Assert::IsTrue(relationshipStorage.addCfgNode(4, {}));
  };

  TEST_METHOD(TestGetCfgNode) {
    spa::RelationshipStorage relationshipStorage;
    relationshipStorage.setCfgTable(cfgTable);
    spa::CFGNode expected = ;

    spa::QueryResult queryResult = relationshipStorage.getCfgNode(1);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getCfgNode());

    queryResult = relationshipStorage.getCallsContainerParent("D");

    expected = { {1, 4, 6} };
    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getLineNumbers());

    queryResult = relationshipStorage.getCallsContainerParent("F");

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(queryResult.getLineNumbers().empty());
  }
  };
}  // namespace UnitTesting
