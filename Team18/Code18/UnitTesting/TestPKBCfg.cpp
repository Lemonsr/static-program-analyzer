#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <utility>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "CFGStorage.h"
#include "CFGNode.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace UnitTesting {
  TEST_CLASS(TestPKBCfg) {
    spa::CFGNode cfgNodeOne;
    spa::CFGNode cfgNodeTwo;
    std::unordered_map<int, spa::CFGNode> cfgNodeTable;
    
public:
  TestPKBCfg() : cfgNodeOne(1), cfgNodeTwo(2) {
    cfgNodeTable = {
      {1, cfgNodeOne},
      {2, cfgNodeTwo}
    };
  }

  TEST_METHOD(TestAddCfgNode) {
    spa::CFGStorage cfgStorage;
    cfgStorage.setCfgNodeTable(cfgNodeTable);
    spa::CFGNode cfgNodeTestOne(3);
    spa::CFGNode cfgNodeTestTwo(4);
    Assert::IsTrue(cfgStorage.addCfgNode(3, cfgNodeTestOne));
    Assert::IsFalse(cfgStorage.addCfgNode(3, cfgNodeTestOne));
    Assert::IsTrue(cfgStorage.addCfgNode(4, cfgNodeTestTwo));
  }

  TEST_METHOD(TestUpdateCfgNode) {
    spa::CFGStorage cfgStorage;
    cfgStorage.setCfgNodeTable(cfgNodeTable);
    spa::CFGNode cfgNodeTestOne(3);
    spa::CFGNode cfgNodeTestTwo(4);
    spa::CFGNode cfgNodeTestThree(5);
    Assert::IsTrue(cfgStorage.updateCfgNode(1, cfgNodeTestOne));
    Assert::IsFalse(cfgStorage.updateCfgNode(3, cfgNodeTestTwo));
    Assert::IsTrue(cfgStorage.updateCfgNode(1, cfgNodeTestThree));
  }

  TEST_METHOD(TestDeleteCfgNode) {
    spa::CFGStorage cfgStorage;
    cfgStorage.setCfgNodeTable(cfgNodeTable);
    spa::CFGNode cfgNodeTestOne(3);
    Assert::IsTrue(cfgStorage.deleteCfgNode(1));
    Assert::IsFalse(cfgStorage.deleteCfgNode(3));
    Assert::IsFalse(cfgStorage.deleteCfgNode(1));
    Assert::IsTrue(cfgStorage.addCfgNode(1, cfgNodeTestOne));
  }

  TEST_METHOD(TestGetCfgNode) {
    spa::CFGStorage cfgStorage;
    cfgStorage.setCfgNodeTable(cfgNodeTable);
    std::vector<spa::CFGNode> expected = { cfgNodeOne };

    spa::QueryResult queryResult = cfgStorage.getCfgNode(1);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getCfgNodes());

    queryResult = cfgStorage.getCfgNode(2);

    expected = { cfgNodeTwo };
    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getCfgNodes());

    queryResult = cfgStorage.getCfgNode(3);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(queryResult.getCfgNodes().empty());
  }
  };
}  // namespace UnitTesting
