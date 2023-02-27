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
  TEST_CLASS(TestPKBUsesProc) {
    std::unordered_map<std::string, std::unordered_set<std::string>> usesProcTable = {
      {"ProcA", {"a"}},
      {"ProcB", {"c", "d", "e"}},
      {"ProcC", {"d"}},
      {"ProcD", {"e", "f"}},
      {"ProcE", {"f"}}
    };

public:
  TEST_METHOD(TestAddUsesProc) {
    spa::RelationshipStorage relationshipStorage;
    relationshipStorage.setUsesProcTable(usesProcTable);

    Assert::IsTrue(relationshipStorage.addUsesProc("ProcF", "z"));
    Assert::IsFalse(relationshipStorage.addUsesProc("ProcF", "z"));
    Assert::IsTrue(relationshipStorage.addUsesProc("ProcF", "y"));
  }

  TEST_METHOD(TestGetUsesProcNameVarName) {
    spa::RelationshipStorage relationshipStorage;
    relationshipStorage.setUsesProcTable(usesProcTable);

    spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LITERAL_STRING, "ProcA", {}));
    spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LITERAL_STRING, "a", {}));
    spa::QueryResult queryResult = relationshipStorage.getUsesProcNameVarName(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
    Assert::IsTrue(queryResult.getIsTrue());

    secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LITERAL_STRING, "b", {}));
    queryResult = relationshipStorage.getUsesProcNameVarName(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
    Assert::IsFalse(queryResult.getIsTrue());
  }

  TEST_METHOD(TestGetUsesProcNameUnderscore) {
    spa::RelationshipStorage relationshipStorage;
    relationshipStorage.setUsesProcTable(usesProcTable);

    spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LITERAL_STRING, "ProcB", {}));
    spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
    spa::QueryResult queryResult = relationshipStorage.getUsesProcNameUnderscore(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
    Assert::IsTrue(queryResult.getIsTrue());

    firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LITERAL_STRING, "ProcK", {}));
    queryResult = relationshipStorage.getUsesProcNameUnderscore(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
    Assert::IsFalse(queryResult.getIsTrue());
  }

  TEST_METHOD(TestGetUsesProcNameVar) {
    spa::RelationshipStorage relationshipStorage;
    relationshipStorage.setUsesProcTable(usesProcTable);
    std::vector<std::pair<std::string, std::string>> expected = { {"ProcA", "a"} };

    spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LITERAL_STRING, "ProcA", {}));
    spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "v",
                                                                   spa::DesignEntityType::VARIABLE));
    spa::QueryResult queryResult = relationshipStorage.getUsesProcNameVar(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getNameNamePairs());

    firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LITERAL_STRING, "ProcK", {}));
    queryResult = relationshipStorage.getUsesProcNameVar(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(queryResult.getNameNamePairs().empty());
  }

  TEST_METHOD(TestGetUsesProcedureVarName) {
    spa::RelationshipStorage relationshipStorage;
    relationshipStorage.setUsesProcTable(usesProcTable);
    std::vector<std::pair<std::string, std::string>> expected = { {"ProcB", "d"}, {"ProcC", "d"} };

    spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "p",
                                                                  spa::DesignEntityType::PROCEDURE));
    spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LITERAL_STRING, "d", {}));
    spa::QueryResult queryResult = relationshipStorage.getUsesProcedureVarName(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getNameNamePairs());

    secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LITERAL_STRING, "z", {}));
    queryResult = relationshipStorage.getUsesProcedureVarName(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(queryResult.getNameNamePairs().empty());
  }

  TEST_METHOD(TestGetUsesProcedureUnderscore) {
    spa::RelationshipStorage relationshipStorage;
    relationshipStorage.setUsesProcTable(usesProcTable);
    std::vector<std::pair<std::string, std::string>> expected;
    for (auto& itr = usesProcTable.begin(); itr != usesProcTable.end(); itr++) {
      std::string procName = itr->first;
      std::unordered_set<std::string> varNames = itr->second;
      for (auto& itr2 = varNames.begin(); itr2 != varNames.end(); itr2++) {
        expected.push_back({ procName, *itr2 });
      }
    }

    spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "p",
                                                                  spa::DesignEntityType::PROCEDURE));
    spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
    spa::QueryResult queryResult = relationshipStorage.getUsesProcedureUnderscore(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getNameNamePairs());

    relationshipStorage.setUsesProcTable({});
    queryResult = relationshipStorage.getUsesProcedureUnderscore(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(queryResult.getNameNamePairs().empty());
  }

  TEST_METHOD(TestGetUsesProcedureVar) {
    spa::RelationshipStorage relationshipStorage;
    relationshipStorage.setUsesProcTable(usesProcTable);
    std::vector<std::pair<std::string, std::string>> expected;
    for (auto& itr = usesProcTable.begin(); itr != usesProcTable.end(); itr++) {
      std::string procName = itr->first;
      std::unordered_set<std::string> varNames = itr->second;
      for (auto& itr2 = varNames.begin(); itr2 != varNames.end(); itr2++) {
        expected.push_back({ procName, *itr2 });
      }
    }

    spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "p",
                                                                  spa::DesignEntityType::STMT));
    spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "v",
                                                                   spa::DesignEntityType::VARIABLE));
    spa::QueryResult queryResult = relationshipStorage.getUsesProcedureVar(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getNameNamePairs());

    relationshipStorage.setUsesProcTable({});
    queryResult = relationshipStorage.getUsesProcedureUnderscore(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(queryResult.getNameNamePairs().empty());
  }
  };
}  // namespace UnitTesting
