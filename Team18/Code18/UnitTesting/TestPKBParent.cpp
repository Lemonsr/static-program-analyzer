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
  TEST_CLASS(TestPKBParent) {
    std::unordered_map<int, std::unordered_set<int>> parentTable = {
      {1, {2}},
      {4, {5, 6, 7}},
      {7, {8}}
    };

    std::unordered_map<int, spa::StatementType> statementTypeTable = {
      {1, spa::StatementType::IF},
      {2, spa::StatementType::ASSIGN},
      {3, spa::StatementType::ASSIGN},
      {4, spa::StatementType::WHILE},
      {5, spa::StatementType::CALL},
      {6, spa::StatementType::ASSIGN},
      {7, spa::StatementType::IF},
      {8, spa::StatementType::PRINT},
      {9, spa::StatementType::ASSIGN}
    };
   public:
      TEST_METHOD(TestAddParent) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setParentTable(parentTable);
        relationshipStorage.setStatementTypeTable(statementTypeTable);

        Assert::IsTrue(relationshipStorage.addParent("10", "11"));
        Assert::IsFalse(relationshipStorage.addParent("10", "11"));
        Assert::IsTrue(relationshipStorage.addParent("10", "12"));
      }

      TEST_METHOD(TestGetParentLineLine) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setParentTable(parentTable);
        relationshipStorage.setStatementTypeTable(statementTypeTable);

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "1", {}));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "2", {}));
        spa::QueryResult queryResult = relationshipStorage.getParentLineLine(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
        Assert::IsTrue(queryResult.getIsTrue());

        secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "3", {}));
        queryResult = relationshipStorage.getParentLineLine(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
        Assert::IsFalse(queryResult.getIsTrue());
      }

      TEST_METHOD(TestGetParentLineStatement) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setParentTable(parentTable);
        relationshipStorage.setStatementTypeTable(statementTypeTable);
        std::vector<std::pair<int, int>> expected = { {1, 2} };

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "1", {}));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "a",
                                                                       spa::DesignEntityType::ASSIGN));
        spa::QueryResult queryResult = relationshipStorage.getParentLineStatement(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

        firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "9", {}));
        queryResult = relationshipStorage.getParentLineStatement(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(queryResult.getLineNumberLineNumberPairs().empty());
      }

      TEST_METHOD(TestGetParentStatementLine) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setParentTable(parentTable);
        relationshipStorage.setStatementTypeTable(statementTypeTable);
        std::vector<std::pair<int, int>> expected = { {4, 5} };

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "w",
                                                                      spa::DesignEntityType::WHILE));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "5", {}));
        spa::QueryResult queryResult = relationshipStorage.getParentStatementLine(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

        secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "9", {}));
        queryResult = relationshipStorage.getParentStatementLine(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(queryResult.getLineNumberLineNumberPairs().empty());
      }

      TEST_METHOD(TestGetParentLineUnderscore) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setParentTable(parentTable);
        relationshipStorage.setStatementTypeTable(statementTypeTable);

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "1", {}));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
        spa::QueryResult queryResult = relationshipStorage.getParentLineUnderscore(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
        Assert::IsTrue(queryResult.getIsTrue());

        firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "9", {}));
        queryResult = relationshipStorage.getParentLineUnderscore(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
        Assert::IsFalse(queryResult.getIsTrue());
      }

      TEST_METHOD(TestGetParentUnderscoreLine) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setParentTable(parentTable);
        relationshipStorage.setStatementTypeTable(statementTypeTable);

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "2", {}));
        spa::QueryResult queryResult = relationshipStorage.getParentUnderscoreLine(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
        Assert::IsTrue(queryResult.getIsTrue());

        secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "4", {}));
        queryResult = relationshipStorage.getParentUnderscoreLine(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
        Assert::IsFalse(queryResult.getIsTrue());
      }

      TEST_METHOD(TestGetParentStatementStatement) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setParentTable(parentTable);
        relationshipStorage.setStatementTypeTable(statementTypeTable);
        std::vector<std::pair<int, int>> expected = { {1, 2} };

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "if",
                                                                      spa::DesignEntityType::IF));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "a",
                                                                       spa::DesignEntityType::ASSIGN));
        spa::QueryResult queryResult = relationshipStorage.getParentStatementStatement(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

        expected = { {4, 6} };
        firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "w", spa::DesignEntityType::WHILE));
        queryResult = relationshipStorage.getParentStatementStatement(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

        firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "a", spa::DesignEntityType::ASSIGN));
        queryResult = relationshipStorage.getParentStatementStatement(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(queryResult.getLineNumberLineNumberPairs().empty());
      }

      TEST_METHOD(TestGetParentStatementUnderscore) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setParentTable(parentTable);
        relationshipStorage.setStatementTypeTable(statementTypeTable);
        std::vector<std::pair<int, int>> expected = { {1, 2}, {7, 8} };

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "if",
                                                                      spa::DesignEntityType::IF));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
        spa::QueryResult queryResult = relationshipStorage.getParentStatementUnderscore(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

        firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "p", spa::DesignEntityType::PRINT));
        queryResult = relationshipStorage.getParentStatementUnderscore(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(queryResult.getLineNumberLineNumberPairs().empty());
      }

      TEST_METHOD(TestGetParentUnderscoreStatement) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setParentTable(parentTable);
        relationshipStorage.setStatementTypeTable(statementTypeTable);
        std::vector<std::pair<int, int>> expected = { {7, 8} };

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "p",
                                                                       spa::DesignEntityType::PRINT));
        spa::QueryResult queryResult = relationshipStorage.getParentUnderscoreStatement(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

        secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "w", spa::DesignEntityType::WHILE));
        queryResult = relationshipStorage.getParentUnderscoreStatement(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(queryResult.getLineNumberLineNumberPairs().empty());
      }

      TEST_METHOD(TestGetParentUnderscoreUnderscore) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setParentTable(parentTable);
        relationshipStorage.setStatementTypeTable(statementTypeTable);

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
        spa::QueryResult queryResult = relationshipStorage.getParentUnderscoreUnderscore(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
        Assert::IsTrue(queryResult.getIsTrue());

        relationshipStorage.setParentTable({});
        queryResult = relationshipStorage.getParentUnderscoreUnderscore(firstArg, secondArg);
        Assert::IsFalse(queryResult.getIsTrue());
      }
  };
}  // namespace UnitTesting
