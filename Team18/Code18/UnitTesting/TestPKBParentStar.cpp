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
  TEST_CLASS(TestPKBParentStar) {
    std::unordered_map<int, std::unordered_set<int>> parentStarTable = {
      {1, {2, 3}},
      {2, {3}},
      {5, {6, 7, 8, 9}},
      {8, {9}}
    };

    std::unordered_map<int, spa::StatementType> statementTypeTable = {
      {1, spa::StatementType::IF},
      {2, spa::StatementType::WHILE},
      {3, spa::StatementType::READ},
      {4, spa::StatementType::ASSIGN},
      {5, spa::StatementType::WHILE},
      {6, spa::StatementType::CALL},
      {7, spa::StatementType::CALL},
      {8, spa::StatementType::IF},
      {9, spa::StatementType::PRINT},
      {10, spa::StatementType::ASSIGN}
    };
    public:
      TEST_METHOD(TestAddParentStar) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setParentStarTable(parentStarTable);
        relationshipStorage.setStatementTypeTable(statementTypeTable);

        Assert::IsTrue(relationshipStorage.addParentStar("11", "12"));
        Assert::IsFalse(relationshipStorage.addParentStar("11", "12"));
        Assert::IsTrue(relationshipStorage.addParentStar("11", "13"));
      }

      TEST_METHOD(TestGetParentStarLineLine) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setParentStarTable(parentStarTable);
        relationshipStorage.setStatementTypeTable(statementTypeTable);

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "1", {}));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "2", {}));
        spa::QueryResult queryResult = relationshipStorage.getParentStarLineLine(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
        Assert::IsTrue(queryResult.getIsTrue());

        secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "3", {}));
        queryResult = relationshipStorage.getParentStarLineLine(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
        Assert::IsTrue(queryResult.getIsTrue());

        secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "4", {}));
        queryResult = relationshipStorage.getParentStarLineLine(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
        Assert::IsFalse(queryResult.getIsTrue());
      }

      TEST_METHOD(TestGetParentStarLineStatement) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setParentStarTable(parentStarTable);
        relationshipStorage.setStatementTypeTable(statementTypeTable);
        std::vector<std::pair<int, int>> expected = { {1, 2}, {1, 3} };

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "1", {}));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "s",
                                                                       spa::DesignEntityType::STMT));
        spa::QueryResult queryResult = relationshipStorage.getParentStarLineStatement(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

        expected = { {1, 3} };
        secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "re",
                                                      spa::DesignEntityType::READ));
        queryResult = relationshipStorage.getParentStarLineStatement(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

        expected = { {5, 8} };
        firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "5", {}));
        secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "if",
                                                      spa::DesignEntityType::IF));
        queryResult = relationshipStorage.getParentStarLineStatement(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());
      }

      TEST_METHOD(TestGetParentStarStatementLine) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setParentStarTable(parentStarTable);
        relationshipStorage.setStatementTypeTable(statementTypeTable);
        std::vector<std::pair<int, int>> expected = { {5, 9} };

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "w",
                                                                      spa::DesignEntityType::WHILE));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "9", {}));
        spa::QueryResult queryResult = relationshipStorage.getParentStarStatementLine(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());
        
        expected = { {5, 9}, {8, 9} };
        firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "s", spa::DesignEntityType::STMT));
        queryResult = relationshipStorage.getParentStarStatementLine(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());
      }

      TEST_METHOD(TestGetParentStarLineUnderscore) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setParentStarTable(parentStarTable);
        relationshipStorage.setStatementTypeTable(statementTypeTable);

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "2", {}));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
        spa::QueryResult queryResult = relationshipStorage.getParentStarLineUnderscore(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
        Assert::IsTrue(queryResult.getIsTrue());

        firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "9", {}));
        queryResult = relationshipStorage.getParentStarLineUnderscore(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
        Assert::IsFalse(queryResult.getIsTrue());
      }

      TEST_METHOD(TestGetParentStarUnderscoreLine) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setParentStarTable(parentStarTable);
        relationshipStorage.setStatementTypeTable(statementTypeTable);

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "2", {}));
        spa::QueryResult queryResult = relationshipStorage.getParentStarUnderscoreLine(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
        Assert::IsTrue(queryResult.getIsTrue());

        secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "4", {}));
        queryResult = relationshipStorage.getParentStarUnderscoreLine(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
        Assert::IsFalse(queryResult.getIsTrue());
      }

      TEST_METHOD(TestGetParentStarStatementStatement) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setParentStarTable(parentStarTable);
        relationshipStorage.setStatementTypeTable(statementTypeTable);
        std::vector<std::pair<int, int>> expected = { {8, 9} };

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "if",
                                                                      spa::DesignEntityType::IF));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "p",
                                                                       spa::DesignEntityType::PRINT));
        spa::QueryResult queryResult = relationshipStorage.getParentStarStatementStatement(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

        expected = { {1, 3} };
        secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "re", spa::DesignEntityType::READ));
        queryResult = relationshipStorage.getParentStarStatementStatement(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

        firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "a", spa::DesignEntityType::ASSIGN));
        queryResult = relationshipStorage.getParentStarStatementStatement(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(queryResult.getLineNumberLineNumberPairs().empty());
      }

      TEST_METHOD(TestGetParentStarStatementUnderscore) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setParentStarTable(parentStarTable);
        relationshipStorage.setStatementTypeTable(statementTypeTable);
        std::vector<std::pair<int, int>> expected = { {1, 2}, {1, 3}, {8, 9} };

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "if",
                                                                      spa::DesignEntityType::IF));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
        spa::QueryResult queryResult = relationshipStorage.getParentStarStatementUnderscore(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

        expected = { {1, 2}, {1, 3}, {2, 3}, {5, 6}, {5, 7}, {5, 8}, {5, 9}, {8, 9} };
        firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "s", spa::DesignEntityType::STMT));
        queryResult = relationshipStorage.getParentStarStatementUnderscore(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

        firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "p", spa::DesignEntityType::PRINT));
        queryResult = relationshipStorage.getParentStarStatementUnderscore(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(queryResult.getLineNumberLineNumberPairs().empty());
      }

      TEST_METHOD(TestGetParentStarUnderscoreStatement) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setParentStarTable(parentStarTable);
        relationshipStorage.setStatementTypeTable(statementTypeTable);
        std::vector<std::pair<int, int>> expected = { {5, 9}, {8, 9} };

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "p",
                                                                       spa::DesignEntityType::PRINT));
        spa::QueryResult queryResult = relationshipStorage.getParentStarUnderscoreStatement(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

        expected = { {1, 2}, {1, 3}, {2, 3}, {5, 6}, {5, 7}, {5, 8}, {5, 9}, {8, 9} };
        secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "s", spa::DesignEntityType::STMT));
        queryResult = relationshipStorage.getParentStarUnderscoreStatement(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

        secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "a", spa::DesignEntityType::ASSIGN));
        queryResult = relationshipStorage.getParentStarUnderscoreStatement(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(queryResult.getLineNumberLineNumberPairs().empty());
      }

      TEST_METHOD(TestGetParentStarUnderscoreUnderscore) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setParentStarTable(parentStarTable);
        relationshipStorage.setStatementTypeTable(statementTypeTable);

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
        spa::QueryResult queryResult = relationshipStorage.getParentStarUnderscoreUnderscore(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
        Assert::IsTrue(queryResult.getIsTrue());

        relationshipStorage.setParentStarTable({});
        queryResult = relationshipStorage.getParentStarUnderscoreUnderscore(firstArg, secondArg);
        Assert::IsFalse(queryResult.getIsTrue());
      }

      TEST_METHOD(TestGetParentStarGeneralStatementStatement) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setParentStarTable(parentStarTable);
        relationshipStorage.setStatementTypeTable(statementTypeTable);
        std::vector<std::pair<int, int>> expected = { {1, 2}, {1, 3}, {8, 9} };

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "if",
                                                                      spa::DesignEntityType::IF));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "s",
                                                                       spa::DesignEntityType::STMT));
        spa::QueryResult queryResult = relationshipStorage.getParentStarStatementStatement(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

        expected = { {2, 3}, {5, 6}, {5, 7}, {5, 8}, {5,9} };
        firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "w", spa::DesignEntityType::WHILE));
        queryResult = relationshipStorage.getParentStarStatementStatement(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

        firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "s1", spa::DesignEntityType::STMT));
        secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "s2", spa::DesignEntityType::STMT));
        queryResult = relationshipStorage.getParentStarStatementStatement(firstArg, secondArg);

        expected = { {1, 2}, {1, 3}, {2, 3}, {5, 6}, {5, 7}, {5, 8}, {5, 9}, {8, 9} };
        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

        firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "a", spa::DesignEntityType::ASSIGN));
        queryResult = relationshipStorage.getParentStarStatementStatement(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(queryResult.getLineNumberLineNumberPairs().empty());
      }
  };
}  // namespace UnitTesting
