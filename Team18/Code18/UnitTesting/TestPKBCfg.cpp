#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <utility>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "CFGStorage.h"
#include "RelationshipStorage.h"
#include "CFGNode.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace UnitTesting {
TEST_CLASS(TestPKBCfg) {
  spa::CFGNode cfgNodeOne;
  spa::CFGNode cfgNodeTwo;
  spa::CFGNode cfgNodeThree;
  std::unordered_map<int, spa::CFGNode> cfgNodeTable;

public:
  TestPKBCfg() : cfgNodeOne(1, spa::StatementType::READ), cfgNodeTwo(2, spa::StatementType::READ),
                 cfgNodeThree(3, spa::StatementType::READ) {
    cfgNodeTable = {
      {1, cfgNodeOne},
      {2, cfgNodeTwo},
      {3, cfgNodeThree}
    };
  }

  TEST_METHOD(TestGetCfgNode) {
    spa::CFGStorage cfgStorage;
    cfgStorage.setCfgNodeTable(cfgNodeTable);

    spa::QueryResult queryResult = cfgStorage.getCfgNode(1);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(queryResult.getCfgNodes().size() == size_t(1));
    spa::CFGNode result = *queryResult.getCfgNodes()[0];
    Assert::IsTrue(cfgNodeOne == result);

    queryResult = cfgStorage.getCfgNode(2);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(queryResult.getCfgNodes().size() == size_t(1));
    result = *queryResult.getCfgNodes()[0];
    Assert::IsTrue(cfgNodeTwo == result);

    queryResult = cfgStorage.getCfgNode(4);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(queryResult.getCfgNodes().empty());
  }

  TEST_METHOD(TestAddCfgNode) {
    spa::CFGStorage cfgStorage;
    cfgStorage.setCfgNodeTable(cfgNodeTable);
    spa::CFGNode cfgNodeTestOne(4, spa::StatementType::READ);
    spa::CFGNode cfgNodeTestTwo(5, spa::StatementType::READ);

    Assert::IsTrue(cfgStorage.addCfgNode(4, cfgNodeTestOne));
    Assert::IsFalse(cfgStorage.addCfgNode(4, cfgNodeTestOne));
    Assert::IsTrue(cfgStorage.addCfgNode(5, cfgNodeTestTwo));
  }

  TEST_METHOD(TestAddCfgEndNode) {
    spa::CFGStorage cfgStorage;
    cfgStorage.setCfgNodeTable(cfgNodeTable);

    Assert::IsTrue(cfgStorage.addCfgEndNode(4));
    Assert::IsFalse(cfgStorage.addCfgEndNode(4));
    Assert::IsTrue(cfgStorage.addCfgEndNode(5));
  }

  TEST_METHOD(TestAddEdge) {
    spa::CFGStorage cfgStorage;
    spa::RelationshipStorage relationshipStorage;
    cfgStorage.setCfgNodeTable(cfgNodeTable);

    Assert::IsTrue(cfgStorage.addEdge(1, 2, relationshipStorage));
    Assert::IsTrue(cfgStorage.addEdge(2, 3, relationshipStorage));
    Assert::IsFalse(cfgStorage.addEdge(1, 2, relationshipStorage));

    spa::QueryResult nodeOneResult = cfgStorage.getCfgNode(1);
    spa::QueryResult nodeTwoResult = cfgStorage.getCfgNode(2);
    spa::QueryResult nodeThreeResult = cfgStorage.getCfgNode(3);

    spa::CFGNode* nodeOne = nodeOneResult.getCfgNodes()[0];
    spa::CFGNode* nodeTwo = nodeTwoResult.getCfgNodes()[0];
    spa::CFGNode* nodeThree = nodeThreeResult.getCfgNodes()[0];

    Assert::IsTrue(nodeOne->getIncomingEdges().empty());

    std::unordered_set<spa::CFGNode*> nodeOneOutgoing = nodeOne->getOutgoingEdges();
    Assert::IsTrue(nodeOneOutgoing.find(nodeTwo) != nodeOneOutgoing.end());

    std::unordered_set<spa::CFGNode*> nodeTwoIncoming = nodeTwo->getIncomingEdges();
    Assert::IsTrue(nodeTwoIncoming.find(nodeOne) != nodeTwoIncoming.end());

    std::unordered_set<spa::CFGNode*> nodeTwoOutgoing = nodeTwo->getOutgoingEdges();
    Assert::IsTrue(nodeTwoOutgoing.find(nodeThree) != nodeTwoOutgoing.end());

    std::unordered_set<spa::CFGNode*> nodeThreeIncoming = nodeThree->getIncomingEdges();
    Assert::IsTrue(nodeThreeIncoming.find(nodeTwo) != nodeThreeIncoming.end());

    Assert::IsTrue(nodeThree->getOutgoingEdges().empty());
  }

  TEST_METHOD(TestAddModifiedVariable) {
    spa::CFGStorage cfgStorage;
    spa::RelationshipStorage relationshipStorage;
    cfgStorage.setCfgNodeTable(cfgNodeTable);

    std::string varOne = "x";
    std::string varTwo = "y";
    std::string varThree = "z";

    cfgStorage.addModifiedVariable(1, varOne);
    cfgStorage.addModifiedVariable(1, varTwo);
    cfgStorage.addModifiedVariable(2, varThree);

    std::unordered_set<std::string> expected = {varOne, varTwo};

    spa::QueryResult queryResult = cfgStorage.getCfgNode(1);
    spa::CFGNode* resultNode = queryResult.getCfgNodes()[0];
    Assert::IsTrue(expected == resultNode->getModifiedVariables());

    expected = {varThree};

    queryResult = cfgStorage.getCfgNode(2);
    resultNode = queryResult.getCfgNodes()[0];
    Assert::IsTrue(expected == resultNode->getModifiedVariables());

    queryResult = cfgStorage.getCfgNode(3);
    resultNode = queryResult.getCfgNodes()[0];
    Assert::IsTrue(resultNode->getModifiedVariables().empty());
  }

  TEST_METHOD(TestRemoveDummyNode) {
    spa::CFGStorage cfgStorage;
    spa::RelationshipStorage relationshipStorage;
    cfgStorage.setCfgNodeTable(cfgNodeTable);

    spa::CFGNode cfgNodeTest = spa::CFGNode();
    cfgStorage.addCfgNode(-1, cfgNodeTest);

    spa::QueryResult queryResult = cfgStorage.getCfgNode(-1);
    std::vector<spa::CFGNode*> resultNodes = queryResult.getCfgNodes();
    Assert::IsFalse(resultNodes.empty());

    Assert::IsTrue(cfgStorage.removeDummyNode());
    Assert::IsFalse(cfgStorage.removeDummyNode());

    queryResult = cfgStorage.getCfgNode(-1);
    resultNodes = queryResult.getCfgNodes();
    Assert::IsTrue(resultNodes.empty());
  }
};
}  // namespace UnitTesting
