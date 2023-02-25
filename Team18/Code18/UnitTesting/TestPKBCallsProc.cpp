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
  TEST_CLASS(TestPKBCallsProc) {
    std::unordered_map<int, std::string> callsProcTable = {
      {2, "A"},
      {3, "B"},
      {5, "B"},
      {7, "C"},
      {9, "D"}
    };

public:
  TEST_METHOD(TestAddCallsProc) {
    spa::RelationshipStorage relationshipStorage;
    relationshipStorage.setCallsProcTable(callsProcTable);

    Assert::IsTrue(relationshipStorage.addCallsProc(6, "E"));
    Assert::IsFalse(relationshipStorage.addCallsProc(6, "E"));
    Assert::IsFalse(relationshipStorage.addCallsProc(6, "F"));
  };

  TEST_METHOD(TestGetCallsProc) {
    spa::RelationshipStorage relationshipStorage;
    relationshipStorage.setCallsProcTable(callsProcTable);
    std::vector<std::pair<int, std::string>> expected = {
      {2, "A"},
      {3, "B"},
      {5, "B"},
      {7, "C"},
      {9, "D"}
    };

    spa::QueryResult queryResult = relationshipStorage.getCallsProc();

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getLineNumberNamePairs());

    relationshipStorage.setCallsProcTable({});

    queryResult = relationshipStorage.getCallsProc();

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(queryResult.getLineNumberNamePairs().empty());
  }
  };
}  // namespace UnitTesting
