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
  TEST_CLASS(TestPKBModifiesProc) {
    std::unordered_map<std::string, std::unordered_set<std::string>> modifiesProcTable = {
      {"ProcA", {"a"}},
      {"ProcB", {"c", "d", "e"}},
      {"ProcC", {"d"}},
      {"ProcD", {"e", "f"}},
      {"ProcE", {"f"}}
    };

public:
  TEST_METHOD(TestAddModifiesProc) {
    spa::RelationshipStorage relationshipStorage;
    relationshipStorage.setModifiesProcTable(modifiesProcTable);

    Assert::IsTrue(relationshipStorage.addModifiesProc("ProcF", "z"));
    Assert::IsFalse(relationshipStorage.addModifiesProc("ProcF", "z"));
    Assert::IsTrue(relationshipStorage.addModifiesProc("ProcF", "y"));
  }

  TEST_METHOD(TestGetModifiesProcNameVarName) {
    spa::RelationshipStorage relationshipStorage;
    relationshipStorage.setModifiesProcTable(modifiesProcTable);

    spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::PROCEDURE_NAME, "ProcA", {}));
    spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::VARIABLE_NAME, "a", {}));
    spa::QueryResult queryResult = relationshipStorage.getModifiesProcNameVarName(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
    Assert::IsTrue(queryResult.getIsTrue());

    secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::VARIABLE_NAME, "b", {}));
    queryResult = relationshipStorage.getModifiesProcNameVarName(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
    Assert::IsFalse(queryResult.getIsTrue());
  }

  TEST_METHOD(TestGetModifiesProcNameUnderscore) {
    spa::RelationshipStorage relationshipStorage;
    relationshipStorage.setModifiesProcTable(modifiesProcTable);

    spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::PROCEDURE_NAME, "ProcB", {}));
    spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
    spa::QueryResult queryResult = relationshipStorage.getModifiesProcNameUnderscore(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
    Assert::IsTrue(queryResult.getIsTrue());

    firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::PROCEDURE_NAME, "ProcK", {}));
    queryResult = relationshipStorage.getModifiesProcNameUnderscore(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
    Assert::IsFalse(queryResult.getIsTrue());
  }

  TEST_METHOD(TestGetModifiesProcNameVar) {
    spa::RelationshipStorage relationshipStorage;
    relationshipStorage.setModifiesProcTable(modifiesProcTable);
    std::vector<std::pair<std::string, std::string>> expected = { {"ProcA", "a"} };

    spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::PROCEDURE_NAME, "ProcA", {}));
    spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "v",
                                                                   spa::DesignEntityType::VARIABLE));
    spa::QueryResult queryResult = relationshipStorage.getModifiesProcNameVar(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getNameNamePairs());

    firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::PROCEDURE_NAME, "ProcK", {}));
    queryResult = relationshipStorage.getModifiesProcNameVar(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(queryResult.getNameNamePairs().empty());
  }

  TEST_METHOD(TestGetModifiesProcedureVarName) {
    spa::RelationshipStorage relationshipStorage;
    relationshipStorage.setModifiesProcTable(modifiesProcTable);
    std::vector<std::pair<std::string, std::string>> expected = { {"ProcB", "d"}, {"ProcC", "d"} };

    spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "p",
                                                                  spa::DesignEntityType::PROCEDURE));
    spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::VARIABLE_NAME, "d", {}));
    spa::QueryResult queryResult = relationshipStorage.getModifiesProcedureVarName(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getNameNamePairs());

    secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::VARIABLE_NAME, "z", {}));
    queryResult = relationshipStorage.getModifiesProcedureVarName(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(queryResult.getNameNamePairs().empty());
  }

  TEST_METHOD(TestGetModifiesProcedureUnderscore) {
    spa::RelationshipStorage relationshipStorage;
    relationshipStorage.setModifiesProcTable(modifiesProcTable);
    std::vector<std::pair<std::string, std::string>> expected;
    for (auto& itr = modifiesProcTable.begin(); itr != modifiesProcTable.end(); itr++) {
      std::string procName = itr->first;
      std::unordered_set<std::string> varNames = itr->second;
      for (auto& itr2 = varNames.begin(); itr2 != varNames.end(); itr2++) {
        expected.push_back({ procName, *itr2 });
      }
    }

    spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "p",
                                                                  spa::DesignEntityType::PROCEDURE));
    spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
    spa::QueryResult queryResult = relationshipStorage.getModifiesProcedureUnderscore(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getNameNamePairs());
  }

  TEST_METHOD(TestGetModifiesProcedureVar) {
    spa::RelationshipStorage relationshipStorage;
    relationshipStorage.setModifiesProcTable(modifiesProcTable);
    std::vector<std::pair<std::string, std::string>> expected;
    for (auto& itr = modifiesProcTable.begin(); itr != modifiesProcTable.end(); itr++) {
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
    spa::QueryResult queryResult = relationshipStorage.getModifiesProcedureVar(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getNameNamePairs());
  }
  };
}  // namespace UnitTesting
