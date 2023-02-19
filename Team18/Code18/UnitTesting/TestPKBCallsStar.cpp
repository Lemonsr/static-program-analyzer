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
  TEST_CLASS(TestPKBCallsStar) {
    std::unordered_map<std::string, std::unordered_set<std::string>> callsStarTable = {
      {"p", {"q", "r", "s"}},
      {"q", {"r", "s"}},
      {"r", {"s"}},
    };

    public:
      TEST_METHOD(TestAddCallsStar) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setCallsStarTable(callsStarTable);

        Assert::IsTrue(relationshipStorage.addCallsStar("p", "t"));
        Assert::IsFalse(relationshipStorage.addCallsStar("p", "t"));
      };

      TEST_METHOD(TestGetCallsStarNameName) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setCallsStarTable(callsStarTable);

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::PROCEDURE_NAME, "p", {}));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::PROCEDURE_NAME, "q", {}));
        spa::QueryResult queryResult = relationshipStorage.getCallsStarNameName(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
        Assert::IsTrue(queryResult.getIsTrue());

        firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::PROCEDURE_NAME, "a", {}));
        queryResult = relationshipStorage.getCallsStarNameName(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
        Assert::IsFalse(queryResult.getIsTrue());
      }

      TEST_METHOD(TestGetCallsStarNameUnderscore) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setCallsStarTable(callsStarTable);

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::PROCEDURE_NAME, "p", {}));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
        spa::QueryResult queryResult = relationshipStorage.getCallsStarNameUnderscore(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
        Assert::IsTrue(queryResult.getIsTrue());

        firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::PROCEDURE_NAME, "a", {}));
        queryResult = relationshipStorage.getCallsStarNameUnderscore(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
        Assert::IsFalse(queryResult.getIsTrue());
      }

      TEST_METHOD(TestGetCallsStarNameProcedure) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setCallsStarTable(callsStarTable);
        std::vector<std::pair<std::string, std::string>> expected = { {"q", "r"}, {"q", "s"}};


        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::PROCEDURE_NAME, "q", {}));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM,
                                                                       "p", { spa::DesignEntityType::PROCEDURE }));
        spa::QueryResult queryResult = relationshipStorage.getCallsStarNameProcedure(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(queryResult.getProcNameProcNamePairs() == expected);

        firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::PROCEDURE_NAME, "a", {}));
        queryResult = relationshipStorage.getCallsStarNameProcedure(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(queryResult.getProcNameProcNamePairs().empty());
      }

      TEST_METHOD(TestGetCallsStarUnderscoreName) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setCallsStarTable(callsStarTable);

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::PROCEDURE_NAME, "s", {}));
        spa::QueryResult queryResult = relationshipStorage.getCallsStarUnderscoreName(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
        Assert::IsTrue(queryResult.getIsTrue());

        secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::PROCEDURE_NAME, "a", {}));
        queryResult = relationshipStorage.getCallsStarUnderscoreName(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
        Assert::IsFalse(queryResult.getIsTrue());
      }

      TEST_METHOD(TestGetCallsStarUnderscoreUnderscore) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setCallsStarTable(callsStarTable);

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
        spa::QueryResult queryResult = relationshipStorage.getCallsStarUnderscoreUnderscore(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
        Assert::IsTrue(queryResult.getIsTrue());

        relationshipStorage.setCallsStarTable({});
        queryResult = relationshipStorage.getCallsStarUnderscoreUnderscore(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
        Assert::IsFalse(queryResult.getIsTrue());
      }

      TEST_METHOD(TestGetCallsStarUnderscoreProcedure) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setCallsStarTable(callsStarTable);
        std::vector<std::pair<std::string, std::string>> expected = {
          {"p", "q"},
          {"p", "r"},
          {"p", "s"},
          {"q", "r"},
          {"q", "s"},
          {"r", "s"},
        };

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM,
                                                      "p", { spa::DesignEntityType::PROCEDURE }));
        spa::QueryResult queryResult = relationshipStorage.getCallsStarUnderscoreProcedure(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(queryResult.getProcNameProcNamePairs() == expected);

        relationshipStorage.setCallsStarTable({});
        queryResult = relationshipStorage.getCallsStarUnderscoreProcedure(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(queryResult.getProcNameProcNamePairs().empty());
      }

      TEST_METHOD(TestGetCallsStarProcedureName) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setCallsStarTable(callsStarTable);
        std::vector<std::pair<std::string, std::string>> expected = {
          {"p", "s"},
          {"q", "s"},
          {"r", "s"},
        };

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM,
                                                     "p", { spa::DesignEntityType::PROCEDURE }));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::PROCEDURE_NAME, "s", {}));

        spa::QueryResult queryResult = relationshipStorage.getCallsStarProcedureName(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(queryResult.getProcNameProcNamePairs() == expected);

        secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::PROCEDURE_NAME, "t", {}));
        queryResult = relationshipStorage.getCallsStarProcedureName(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(queryResult.getProcNameProcNamePairs().empty());
      }

      TEST_METHOD(TestGetCallsStarProcedureUnderscore) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setCallsStarTable(callsStarTable);
        std::vector<std::pair<std::string, std::string>> expected = {
          {"p", "q"},
          {"p", "r"},
          {"p", "s"},
          {"q", "r"},
          {"q", "s"},
          {"r", "s"},
        };

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM,
                                                                      "p", { spa::DesignEntityType::PROCEDURE }));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));

        spa::QueryResult queryResult = relationshipStorage.getCallsStarProcedureUnderscore(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(queryResult.getProcNameProcNamePairs() == expected);

        relationshipStorage.setCallsStarTable({});
        queryResult = relationshipStorage.getCallsStarProcedureUnderscore(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(queryResult.getProcNameProcNamePairs().empty());
      }

      TEST_METHOD(TestGetCallsStarProcedureProcedure) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setCallsStarTable(callsStarTable);
        std::vector<std::pair<std::string, std::string>> expected = {
          {"p", "q"},
          {"p", "r"},
          {"p", "s"},
          {"q", "r"},
          {"q", "s"},
          {"r", "s"},
        };

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM,
                                                     "p", { spa::DesignEntityType::PROCEDURE }));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));

        spa::QueryResult queryResult = relationshipStorage.getCallsStarProcedureProcedure(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(queryResult.getProcNameProcNamePairs() == expected);

        relationshipStorage.setCallsStarTable({});
        queryResult = relationshipStorage.getCallsStarProcedureProcedure(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(queryResult.getProcNameProcNamePairs().empty());
      }
  };
}  // namespace UnitTesting
