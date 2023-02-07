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
  TEST_CLASS(TestPKBFollowsStar) {
    std::unordered_map<int, std::unordered_set<int>> followsStarTable = {
      {1, {2, 3, 4, 8, 9}},
      {2, {3, 4, 8, 9}},
      {3, {4, 8, 9}},
      {4, {8, 9}},
      {5, {6, 7}},
      {6, {7}},
      {8, {9}}
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
      TEST_METHOD(TestAddFollowsStar) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setFollowsStarTable(followsStarTable);
        relationshipStorage.setStatementTypeTable(statementTypeTable);

        Assert::IsTrue(relationshipStorage.addFollowsStar("10", "11"));
        Assert::IsFalse(relationshipStorage.addFollowsStar("10", "11"));
        Assert::IsTrue(relationshipStorage.addFollowsStar("10", "12"));
      }

      TEST_METHOD(TestGetFollowsStarLineLine) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setFollowsStarTable(followsStarTable);
        relationshipStorage.setStatementTypeTable(statementTypeTable);

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "1", {}));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "2", {}));
        spa::QueryResult queryResult = relationshipStorage.getFollowsStarLineLine(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
        Assert::IsTrue(queryResult.getIsTrue());

        secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "3", {}));
        queryResult = relationshipStorage.getFollowsStarLineLine(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
        Assert::IsTrue(queryResult.getIsTrue());

        secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "10", {}));
        queryResult = relationshipStorage.getFollowsStarLineLine(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
        Assert::IsFalse(queryResult.getIsTrue());
      }

      TEST_METHOD(TestGetFollowsStarLineStatement) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setFollowsStarTable(followsStarTable);
        relationshipStorage.setStatementTypeTable(statementTypeTable);
        std::vector<std::pair<int, int>> expected = { {1, 2}, {1, 3}, {1, 4}, {1, 8}, {1, 9} };

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "1", {}));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "s",
                                                                       spa::DesignEntityType::STMT));
        spa::QueryResult queryResult = relationshipStorage.getFollowsStarLineStatement(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

        expected = { {1, 2} };
        secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "re", spa::DesignEntityType::READ));
        queryResult = relationshipStorage.getFollowsStarLineStatement(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

        expected = { {5, 7} };
        firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "5", {}));
        secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "p",
                                                      spa::DesignEntityType::PRINT));
        queryResult = relationshipStorage.getFollowsStarLineStatement(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

        firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "9", {}));
        queryResult = relationshipStorage.getFollowsStarLineStatement(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(queryResult.getLineNumberLineNumberPairs().empty());
      }

      TEST_METHOD(TestGetFollowsStarStatementLine) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setFollowsStarTable(followsStarTable);
        relationshipStorage.setStatementTypeTable(statementTypeTable);
        std::vector<std::pair<int, int>> expected = { {4, 9} };

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "w",
                                                                      spa::DesignEntityType::WHILE));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "9", {}));
        spa::QueryResult queryResult = relationshipStorage.getFollowsStarStatementLine(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

        expected = { {1, 9}, {2, 9}, {3, 9}, {4, 9}, {8, 9} };
        firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "s", spa::DesignEntityType::STMT));
        queryResult = relationshipStorage.getFollowsStarStatementLine(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

        firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "if", spa::DesignEntityType::IF));
        queryResult = relationshipStorage.getFollowsStarStatementLine(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(queryResult.getLineNumberLineNumberPairs().empty());
      }

      TEST_METHOD(TestGetFollowsStarLineUnderscore) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setFollowsStarTable(followsStarTable);
        relationshipStorage.setStatementTypeTable(statementTypeTable);

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "2", {}));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
        spa::QueryResult queryResult = relationshipStorage.getFollowsStarLineUnderscore(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
        Assert::IsTrue(queryResult.getIsTrue());

        firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "7", {}));
        queryResult = relationshipStorage.getFollowsStarLineUnderscore(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
        Assert::IsFalse(queryResult.getIsTrue());
      }

      TEST_METHOD(TestGetFollowsStarUnderscoreLine) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setFollowsStarTable(followsStarTable);
        relationshipStorage.setStatementTypeTable(statementTypeTable);

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "2", {}));
        spa::QueryResult queryResult = relationshipStorage.getFollowsStarUnderscoreLine(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
        Assert::IsTrue(queryResult.getIsTrue());

        secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "5", {}));
        queryResult = relationshipStorage.getFollowsStarUnderscoreLine(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
        Assert::IsFalse(queryResult.getIsTrue());
      }

      TEST_METHOD(TestGetFollowsStarStatementStatement) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setFollowsStarTable(followsStarTable);
        relationshipStorage.setStatementTypeTable(statementTypeTable);
        std::vector<std::pair<int, int>> expected = { {6, 7} };

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "a",
          spa::DesignEntityType::ASSIGN));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "p",
          spa::DesignEntityType::PRINT));
        spa::QueryResult queryResult = relationshipStorage.getFollowsStarStatementStatement(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

        expected = { {3, 8}, {3, 9}, {8, 9} };
        secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "a", spa::DesignEntityType::ASSIGN));
        queryResult = relationshipStorage.getFollowsStarStatementStatement(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

        firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "if", spa::DesignEntityType::IF));
        queryResult = relationshipStorage.getFollowsStarStatementStatement(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(queryResult.getLineNumberLineNumberPairs().empty());
      }

      TEST_METHOD(TestGetFollowsStarStatementUnderscore) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setFollowsStarTable(followsStarTable);
        relationshipStorage.setStatementTypeTable(statementTypeTable);
        std::vector<std::pair<int, int>> expected = { {4, 8}, {4, 9} };

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "w",
                                                                      spa::DesignEntityType::WHILE));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
        spa::QueryResult queryResult = relationshipStorage.getFollowsStarStatementUnderscore(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

        expected = { {1, 2}, {1, 3}, {1, 4}, {1, 8}, {1, 9}, {2, 3}, {2, 4}, {2, 8}, {2, 9}, {3, 4}, {3, 8}, {3, 9},
                     {4, 8}, {4, 9}, {5, 6}, {5, 7}, {6, 7}, {8, 9} };
        firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "s", spa::DesignEntityType::STMT));
        queryResult = relationshipStorage.getFollowsStarStatementUnderscore(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

        firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "p", spa::DesignEntityType::PRINT));
        queryResult = relationshipStorage.getFollowsStarStatementUnderscore(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(queryResult.getLineNumberLineNumberPairs().empty());
      }

      TEST_METHOD(TestGetFollowsStarUnderscoreStatement) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setFollowsStarTable(followsStarTable);
        relationshipStorage.setStatementTypeTable(statementTypeTable);
        std::vector<std::pair<int, int>> expected = { {5, 7}, {6, 7} };

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "p",
          spa::DesignEntityType::PRINT));
        spa::QueryResult queryResult = relationshipStorage.getFollowsStarUnderscoreStatement(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

        expected = { {1, 2}, {1, 3}, {1, 4}, {1, 8}, {1, 9}, {2, 3}, {2, 4}, {2, 8}, {2, 9}, {3, 4}, {3, 8}, {3, 9},
                     {4, 8}, {4, 9}, {5, 6}, {5, 7}, {6, 7}, {8, 9} };
        secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "s", spa::DesignEntityType::STMT));
        queryResult = relationshipStorage.getFollowsStarUnderscoreStatement(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

        secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "c", spa::DesignEntityType::CALL));
        queryResult = relationshipStorage.getFollowsStarUnderscoreStatement(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(queryResult.getLineNumberLineNumberPairs().empty());
      }

      TEST_METHOD(TestGetFollowsStarUnderscoreUnderscore) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setFollowsStarTable(followsStarTable);
        relationshipStorage.setStatementTypeTable(statementTypeTable);

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
        spa::QueryResult queryResult = relationshipStorage.getFollowsStarUnderscoreUnderscore(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
        Assert::IsTrue(queryResult.getIsTrue());

        relationshipStorage.setFollowsStarTable({});
        queryResult = relationshipStorage.getFollowsStarUnderscoreUnderscore(firstArg, secondArg);
        Assert::IsFalse(queryResult.getIsTrue());
      }

      TEST_METHOD(TestGetFollowsStarGeneralStatementStatement) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setFollowsStarTable(followsStarTable);
        relationshipStorage.setStatementTypeTable(statementTypeTable);
        std::vector<std::pair<int, int>> expected = { {5, 6}, {5, 7} };

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "c",
                                                                      spa::DesignEntityType::CALL));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "s",
                                                                       spa::DesignEntityType::STMT));
        spa::QueryResult queryResult = relationshipStorage.getFollowsStarStatementStatement(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

        expected = { {1, 2}, {1, 3}, {1, 4}, {1, 8}, {1, 9}, {2, 3}, {2, 4}, {2, 8}, {2, 9}, {3, 4}, {3, 8}, {3, 9},
                     {4, 8}, {4, 9}, {5, 6}, {5, 7}, {6, 7}, {8, 9} };
        firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "s1", spa::DesignEntityType::STMT));
        secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "s2", spa::DesignEntityType::STMT));
        queryResult = relationshipStorage.getFollowsStarStatementStatement(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

        expected = { {3, 4}, {3, 8}, {3, 9}, {6, 7}, {8, 9} };
        firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "a", spa::DesignEntityType::ASSIGN));
        queryResult = relationshipStorage.getFollowsStarStatementStatement(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

        firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "if", spa::DesignEntityType::IF));
        queryResult = relationshipStorage.getFollowsStarStatementStatement(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(queryResult.getLineNumberLineNumberPairs().empty());
      }
  };
}  // namespace UnitTesting
