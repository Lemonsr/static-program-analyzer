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
  TEST_CLASS(TestPKBFollows) {
    std::unordered_map<int, int> followsTable = {
      {1, 2},
      {2, 3},
      {3, 4},
      {4, 8},
      {5, 6},
      {6, 7},
      {8, 9}
    };

    std::unordered_map<int, spa::StatementType> statementTypeTable = {
      {1, spa::StatementType::READ},
      {2, spa::StatementType::READ},
      {3, spa::StatementType::ASSIGN},
      {4, spa::StatementType::WHILE},
      {5, spa::StatementType::CALL},
      {6, spa::StatementType::ASSIGN},
      {7, spa::StatementType::PRINT},
      {8, spa::StatementType::ASSIGN},
      {9, spa::StatementType::ASSIGN}
    };
    public:
      TEST_METHOD(TestAddFollows) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setFollowsTable(followsTable);
        relationshipStorage.setStatementTypeTable(statementTypeTable);

        Assert::IsTrue(relationshipStorage.addFollows("10", "11"));
        Assert::IsFalse(relationshipStorage.addFollows("10", "11"));
      }

      TEST_METHOD(TestGetFollowsLineLine) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setFollowsTable(followsTable);
        relationshipStorage.setStatementTypeTable(statementTypeTable);

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "1", {}));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "2", {}));
        spa::QueryResult queryResult = relationshipStorage.getFollowsLineLine(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
        Assert::IsTrue(queryResult.getIsTrue());

        secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "3", {}));
        queryResult = relationshipStorage.getFollowsLineLine(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
        Assert::IsFalse(queryResult.getIsTrue());
      }

      TEST_METHOD(TestGetFollowsLineStatement) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setFollowsTable(followsTable);
        relationshipStorage.setStatementTypeTable(statementTypeTable);
        std::vector<std::pair<int, int>> expected = { {1, 2} };

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "1", {}));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "re",
          spa::DesignEntityType::READ));
        spa::QueryResult queryResult = relationshipStorage.getFollowsLineStatement(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

        firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "9", {}));
        queryResult = relationshipStorage.getFollowsLineStatement(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(queryResult.getLineNumberLineNumberPairs().empty());
      }

      TEST_METHOD(TestGetFollowsStatementLine) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setFollowsTable(followsTable);
        relationshipStorage.setStatementTypeTable(statementTypeTable);
        std::vector<std::pair<int, int>> expected = { {4, 8} };

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "w",
          spa::DesignEntityType::WHILE));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "8", {}));
        spa::QueryResult queryResult = relationshipStorage.getFollowsStatementLine(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

        secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "9", {}));
        queryResult = relationshipStorage.getFollowsStatementLine(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(queryResult.getLineNumberLineNumberPairs().empty());
      }

      TEST_METHOD(TestGetFollowsLineUnderscore) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setFollowsTable(followsTable);
        relationshipStorage.setStatementTypeTable(statementTypeTable);

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "1", {}));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
        spa::QueryResult queryResult = relationshipStorage.getFollowsLineUnderscore(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
        Assert::IsTrue(queryResult.getIsTrue());

        firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "9", {}));
        queryResult = relationshipStorage.getFollowsLineUnderscore(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
        Assert::IsFalse(queryResult.getIsTrue());
      }

      TEST_METHOD(TestGetFollowsUnderscoreLine) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setFollowsTable(followsTable);
        relationshipStorage.setStatementTypeTable(statementTypeTable);

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "2", {}));
        spa::QueryResult queryResult = relationshipStorage.getFollowsUnderscoreLine(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
        Assert::IsTrue(queryResult.getIsTrue());

        secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "5", {}));
        queryResult = relationshipStorage.getFollowsUnderscoreLine(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
        Assert::IsFalse(queryResult.getIsTrue());
      }

      TEST_METHOD(TestGetFollowsStatementStatement) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setFollowsTable(followsTable);
        relationshipStorage.setStatementTypeTable(statementTypeTable);
        std::vector<std::pair<int, int>> expected = { {2, 3}, {4, 8}, {5, 6}, {8, 9} };

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "s",
                                                                      spa::DesignEntityType::STMT));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "a",
                                                                       spa::DesignEntityType::ASSIGN));
        spa::QueryResult queryResult = relationshipStorage.getFollowsStatementStatement(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

        expected = { {2, 3} };
        firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "re", spa::DesignEntityType::READ));
        queryResult = relationshipStorage.getFollowsStatementStatement(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());
      }

      TEST_METHOD(TestGetFollowsStatementUnderscore) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setFollowsTable(followsTable);
        relationshipStorage.setStatementTypeTable(statementTypeTable);
        std::vector<std::pair<int, int>> expected = { {5, 6} };

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "c",
          spa::DesignEntityType::CALL));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
        spa::QueryResult queryResult = relationshipStorage.getFollowsStatementUnderscore(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

        firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "p", spa::DesignEntityType::PRINT));
        queryResult = relationshipStorage.getFollowsStatementUnderscore(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(queryResult.getLineNumberLineNumberPairs().empty());
      }

      TEST_METHOD(TestGetFollowsUnderscoreStatement) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setFollowsTable(followsTable);
        relationshipStorage.setStatementTypeTable(statementTypeTable);
        std::vector<std::pair<int, int>> expected = { {6, 7} };

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "p",
                                                                       spa::DesignEntityType::PRINT));
        spa::QueryResult queryResult = relationshipStorage.getFollowsUnderscoreStatement(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

        secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "c", spa::DesignEntityType::CALL));
        queryResult = relationshipStorage.getFollowsUnderscoreStatement(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(queryResult.getLineNumberLineNumberPairs().empty());
      }

      TEST_METHOD(TestGetFollowsUnderscoreUnderscore) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setFollowsTable(followsTable);
        relationshipStorage.setStatementTypeTable(statementTypeTable);

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
        spa::QueryResult queryResult = relationshipStorage.getFollowsUnderscoreUnderscore(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
        Assert::IsTrue(queryResult.getIsTrue());

        relationshipStorage.setFollowsTable({});
        queryResult = relationshipStorage.getFollowsUnderscoreUnderscore(firstArg, secondArg);
        Assert::IsFalse(queryResult.getIsTrue());
      }
  };
}  // namespace UnitTesting
