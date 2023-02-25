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
  TEST_CLASS(TestPKBCalls) {
    std::unordered_map<std::string, std::unordered_set<std::string>> callsTable = {
      {"p", {"q", "r"}},
      {"q", {"r", "s"}},
      {"r", {"s"}},
    };

    public:
      TEST_METHOD(TestAddCalls) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setCallsTable(callsTable);

        Assert::IsTrue(relationshipStorage.addCalls("p", "s"));
        Assert::IsFalse(relationshipStorage.addCalls("p", "s"));
      };

      TEST_METHOD(TestGetCallsNameName) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setCallsTable(callsTable);

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::PROCEDURE_NAME, "p", {}));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::PROCEDURE_NAME, "q", {}));
        spa::QueryResult queryResult = relationshipStorage.getCallsNameName(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
        Assert::IsTrue(queryResult.getIsTrue());

        firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::PROCEDURE_NAME, "a", {}));
        queryResult = relationshipStorage.getCallsNameName(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
        Assert::IsFalse(queryResult.getIsTrue());
      }

      TEST_METHOD(TestGetCallsNameUnderscore) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setCallsTable(callsTable);

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::PROCEDURE_NAME, "p", {}));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
        spa::QueryResult queryResult = relationshipStorage.getCallsNameUnderscore(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
        Assert::IsTrue(queryResult.getIsTrue());

        firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::PROCEDURE_NAME, "a", {}));
        queryResult = relationshipStorage.getCallsNameUnderscore(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
        Assert::IsFalse(queryResult.getIsTrue());
      }

      TEST_METHOD(TestGetCallsNameProcedure) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setCallsTable(callsTable);
        std::vector<std::pair<std::string, std::string>> expected = { {"q", "r"}, {"q", "s"}};


        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::PROCEDURE_NAME, "q", {}));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM,
                                                                       "p", { spa::DesignEntityType::PROCEDURE }));
        spa::QueryResult queryResult = relationshipStorage.getCallsNameProcedure(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(queryResult.getNameNamePairs() == expected);

        firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::PROCEDURE_NAME, "a", {}));
        queryResult = relationshipStorage.getCallsNameProcedure(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(queryResult.getNameNamePairs().empty());
      }

      TEST_METHOD(TestGetCallsUnderscoreName) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setCallsTable(callsTable);

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::PROCEDURE_NAME, "s", {}));
        spa::QueryResult queryResult = relationshipStorage.getCallsUnderscoreName(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
        Assert::IsTrue(queryResult.getIsTrue());

        secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::PROCEDURE_NAME, "a", {}));
        queryResult = relationshipStorage.getCallsUnderscoreName(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
        Assert::IsFalse(queryResult.getIsTrue());
      }

      TEST_METHOD(TestGetCallsUnderscoreUnderscore) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setCallsTable(callsTable);

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
        spa::QueryResult queryResult = relationshipStorage.getCallsUnderscoreUnderscore(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
        Assert::IsTrue(queryResult.getIsTrue());

        relationshipStorage.setCallsTable({});
        queryResult = relationshipStorage.getCallsUnderscoreUnderscore(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
        Assert::IsFalse(queryResult.getIsTrue());
      }

      TEST_METHOD(TestGetCallsUnderscoreProcedure) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setCallsTable(callsTable);
        std::vector<std::pair<std::string, std::string>> expected = {
          {"p", "q"},
          {"p", "r"},
          {"q", "r"},
          {"q", "s"},
          {"r", "s"},
        };

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM,
                                                      "p", { spa::DesignEntityType::PROCEDURE }));
        spa::QueryResult queryResult = relationshipStorage.getCallsUnderscoreProcedure(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(queryResult.getNameNamePairs() == expected);

        relationshipStorage.setCallsTable({});
        queryResult = relationshipStorage.getCallsUnderscoreProcedure(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(queryResult.getNameNamePairs().empty());
      }

      TEST_METHOD(TestGetCallsProcedureName) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setCallsTable(callsTable);
        std::vector<std::pair<std::string, std::string>> expected = {
          {"q", "s"},
          {"r", "s"},
        };

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM,
                                                     "p", { spa::DesignEntityType::PROCEDURE }));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::PROCEDURE_NAME, "s", {}));

        spa::QueryResult queryResult = relationshipStorage.getCallsProcedureName(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(queryResult.getNameNamePairs() == expected);

        secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::PROCEDURE_NAME, "t", {}));
        queryResult = relationshipStorage.getCallsProcedureName(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(queryResult.getNameNamePairs().empty());
      }

      TEST_METHOD(TestGetCallsProcedureUnderscore) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setCallsTable(callsTable);
        std::vector<std::pair<std::string, std::string>> expected = {
          {"p", "q"},
          {"p", "r"},
          {"q", "r"},
          {"q", "s"},
          {"r", "s"},
        };

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM,
                                                                      "p", { spa::DesignEntityType::PROCEDURE }));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));

        spa::QueryResult queryResult = relationshipStorage.getCallsProcedureUnderscore(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(queryResult.getNameNamePairs() == expected);

        relationshipStorage.setCallsTable({});
        queryResult = relationshipStorage.getCallsProcedureUnderscore(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(queryResult.getNameNamePairs().empty());
      }

      TEST_METHOD(TestGetCallsProcedureProcedure) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setCallsTable(callsTable);
        std::vector<std::pair<std::string, std::string>> expected = {
          {"p", "q"},
          {"p", "r"},
          {"q", "r"},
          {"q", "s"},
          {"r", "s"},
        };

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM,
                                                     "p", { spa::DesignEntityType::PROCEDURE }));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));

        spa::QueryResult queryResult = relationshipStorage.getCallsProcedureProcedure(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(queryResult.getNameNamePairs() == expected);

        relationshipStorage.setCallsTable({});
        queryResult = relationshipStorage.getCallsProcedureProcedure(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(queryResult.getNameNamePairs().empty());
      }
  };
}  // namespace UnitTesting
