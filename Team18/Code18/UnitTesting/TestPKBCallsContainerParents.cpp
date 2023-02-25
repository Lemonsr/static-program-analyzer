#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <utility>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "RelationshipStorage.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace UnitTesting {
  TEST_CLASS(TestPKBCallsContainerParents) {
    std::unordered_map<std::string, std::unordered_set<int>> callsContainerParentsTable = {
      {"A", {1}},
      {"B", {1, 4}},
      {"C", {1, 4, 6}},
      {"D", {1, 4, 6}}
    };

public:
  TEST_METHOD(TestAddCallsContainerParents) {
    spa::RelationshipStorage relationshipStorage;
    relationshipStorage.setCallsContainerParentsTable(callsContainerParentsTable);

    Assert::IsTrue(relationshipStorage.addCallsContainerParent("E", "1"));
    Assert::IsFalse(relationshipStorage.addCallsContainerParent("E", "1"));
  };

  TEST_METHOD(TestGetCallsContainerParent) {
    spa::RelationshipStorage relationshipStorage;
    relationshipStorage.setCallsContainerParentsTable(callsContainerParentsTable);
    std::vector<int> expected = { {1} };

    spa::QueryResult queryResult = relationshipStorage.getCallsContainerParent("A");

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getLineNumbers());

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
