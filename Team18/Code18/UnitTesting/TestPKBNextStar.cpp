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
  TEST_CLASS(TestPKBNextStar) {
    std::unordered_map<int, std::unordered_set<int>> nextStarTable = {
      {1, {2, 3, 4, 5}},
      {2, {2, 3, 4, 5}},
      {3, {2, 3, 4, 5}},
      {4, {5}}
    };

    std::unordered_map<int, spa::StatementType> statementTypeTable = {
      {1, spa::StatementType::READ},
      {2, spa::StatementType::WHILE},
      {3, spa::StatementType::CALL},
      {4, spa::StatementType::READ},
      {5, spa::StatementType::PRINT}
    };
public:
  TEST_METHOD(TestAddNextStar) {
    spa::RelationshipStorage relationshipStorage;
    relationshipStorage.setNextStarTable(nextStarTable);
    relationshipStorage.setStatementTypeTable(statementTypeTable);

    Assert::IsTrue(relationshipStorage.addNextStar("6", "7"));
    Assert::IsFalse(relationshipStorage.addNextStar("6", "7"));
    Assert::IsTrue(relationshipStorage.addNextStar("6", "8"));
  }

  TEST_METHOD(TestGetNextStarLineLine) {
    spa::RelationshipStorage relationshipStorage;
    relationshipStorage.setNextStarTable(nextStarTable);
    relationshipStorage.setStatementTypeTable(statementTypeTable);

    spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "3", {}));
    spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "4", {}));
    spa::QueryResult queryResult = relationshipStorage.getNextStarLineLine(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
    Assert::IsTrue(queryResult.getIsTrue());

    secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "5", {}));
    queryResult = relationshipStorage.getNextStarLineLine(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
    Assert::IsTrue(queryResult.getIsTrue());

    secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "10", {}));
    queryResult = relationshipStorage.getNextStarLineLine(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
    Assert::IsFalse(queryResult.getIsTrue());
  }

  TEST_METHOD(TestGetNextStarLineStatement) {
    spa::RelationshipStorage relationshipStorage;
    relationshipStorage.setNextStarTable(nextStarTable);
    relationshipStorage.setStatementTypeTable(statementTypeTable);
    std::vector<std::pair<int, int>> expected = { {3, 2}, {3, 3}, {3, 4}, {3, 5} };

    spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "3", {}));
    spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "s",
      spa::DesignEntityType::STMT));
    spa::QueryResult queryResult = relationshipStorage.getNextStarLineStatement(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

    expected = { {3, 5} };
    secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "p", spa::DesignEntityType::PRINT));
    queryResult = relationshipStorage.getNextStarLineStatement(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

    expected = { {3, 2} };
    firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "3", {}));
    secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "w",
      spa::DesignEntityType::WHILE));
    queryResult = relationshipStorage.getNextStarLineStatement(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

    firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "4", {}));
    queryResult = relationshipStorage.getNextStarLineStatement(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(queryResult.getLineNumberLineNumberPairs().empty());
  }

  TEST_METHOD(TestGetNextStarStatementLine) {
    spa::RelationshipStorage relationshipStorage;
    relationshipStorage.setNextStarTable(nextStarTable);
    relationshipStorage.setStatementTypeTable(statementTypeTable);
    std::vector<std::pair<int, int>> expected = { {2, 2} };

    spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "w",
      spa::DesignEntityType::WHILE));
    spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "2", {}));
    spa::QueryResult queryResult = relationshipStorage.getNextStarStatementLine(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

    expected = { {1, 2}, {2, 2}, {3, 2} };
    firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "s", spa::DesignEntityType::STMT));
    queryResult = relationshipStorage.getNextStarStatementLine(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

    firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "if", spa::DesignEntityType::IF));
    queryResult = relationshipStorage.getNextStarStatementLine(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(queryResult.getLineNumberLineNumberPairs().empty());
  }

  TEST_METHOD(TestGetNextStarLineUnderscore) {
    spa::RelationshipStorage relationshipStorage;
    relationshipStorage.setNextStarTable(nextStarTable);
    relationshipStorage.setStatementTypeTable(statementTypeTable);

    spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "2", {}));
    spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
    spa::QueryResult queryResult = relationshipStorage.getNextStarLineUnderscore(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
    Assert::IsTrue(queryResult.getIsTrue());

    firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "5", {}));
    queryResult = relationshipStorage.getNextStarLineUnderscore(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
    Assert::IsFalse(queryResult.getIsTrue());
  }

  TEST_METHOD(TestGetNextStarUnderscoreLine) {
    spa::RelationshipStorage relationshipStorage;
    relationshipStorage.setNextStarTable(nextStarTable);
    relationshipStorage.setStatementTypeTable(statementTypeTable);

    spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
    spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "2", {}));
    spa::QueryResult queryResult = relationshipStorage.getNextStarUnderscoreLine(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
    Assert::IsTrue(queryResult.getIsTrue());

    secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "1", {}));
    queryResult = relationshipStorage.getNextStarUnderscoreLine(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
    Assert::IsFalse(queryResult.getIsTrue());
  }

  TEST_METHOD(TestGetNextStarStatementStatement) {
    spa::RelationshipStorage relationshipStorage;
    relationshipStorage.setNextStarTable(nextStarTable);
    relationshipStorage.setStatementTypeTable(statementTypeTable);
    std::vector<std::pair<int, int>> expected = { {1, 5}, {2, 5}, {3, 5}, {4, 5} };

    spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "s",
      spa::DesignEntityType::STMT));
    spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "p",
      spa::DesignEntityType::PRINT));
    spa::QueryResult queryResult = relationshipStorage.getNextStarStatementStatement(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

    expected = { {1, 2}, {2, 2}, {3, 2} };
    secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "w", spa::DesignEntityType::WHILE));
    queryResult = relationshipStorage.getNextStarStatementStatement(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

    firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "if", spa::DesignEntityType::IF));
    queryResult = relationshipStorage.getNextStarStatementStatement(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(queryResult.getLineNumberLineNumberPairs().empty());
  }

  TEST_METHOD(TestGetNextStarStatementUnderscore) {
    spa::RelationshipStorage relationshipStorage;
    relationshipStorage.setNextStarTable(nextStarTable);
    relationshipStorage.setStatementTypeTable(statementTypeTable);
    std::vector<std::pair<int, int>> expected = { {1, 2}, {1, 3}, {1, 4}, {1, 5}, {4, 5} };

    spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "re",
                                                                  spa::DesignEntityType::READ));
    spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
    spa::QueryResult queryResult = relationshipStorage.getNextStarStatementUnderscore(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

    expected = { {1, 2}, {1, 3}, {1, 4}, {1, 5}, {2, 2}, {2, 3}, {2, 4}, {2, 5}, {3, 2}, {3, 3}, {3, 4}, {3, 5},
                 {4, 5} };
    firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "s", spa::DesignEntityType::STMT));
    queryResult = relationshipStorage.getNextStarStatementUnderscore(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

    firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "a", spa::DesignEntityType::ASSIGN));
    queryResult = relationshipStorage.getNextStarStatementUnderscore(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(queryResult.getLineNumberLineNumberPairs().empty());
  }

  TEST_METHOD(TestGetNextStarUnderscoreStatement) {
    spa::RelationshipStorage relationshipStorage;
    relationshipStorage.setNextStarTable(nextStarTable);
    relationshipStorage.setStatementTypeTable(statementTypeTable);
    std::vector<std::pair<int, int>> expected = { {1, 5}, {2, 5}, {3, 5}, {4, 5} };

    spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
    spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "p",
                                                                   spa::DesignEntityType::PRINT));
    spa::QueryResult queryResult = relationshipStorage.getNextStarUnderscoreStatement(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

    expected = { {1, 2}, {1, 3}, {1, 4}, {1, 5}, {2, 2}, {2, 3}, {2, 4}, {2, 5}, {3, 2}, {3, 3}, {3, 4}, {3, 5},
                 {4, 5} };
    secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "s", spa::DesignEntityType::STMT));
    queryResult = relationshipStorage.getNextStarUnderscoreStatement(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

    secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "if", spa::DesignEntityType::IF));
    queryResult = relationshipStorage.getNextStarUnderscoreStatement(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(queryResult.getLineNumberLineNumberPairs().empty());
  }

  TEST_METHOD(TestGetNextStarUnderscoreUnderscore) {
    spa::RelationshipStorage relationshipStorage;
    relationshipStorage.setNextStarTable(nextStarTable);
    relationshipStorage.setStatementTypeTable(statementTypeTable);

    spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
    spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
    spa::QueryResult queryResult = relationshipStorage.getNextStarUnderscoreUnderscore(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
    Assert::IsTrue(queryResult.getIsTrue());

    relationshipStorage.setNextStarTable({});
    queryResult = relationshipStorage.getNextStarUnderscoreUnderscore(firstArg, secondArg);
    Assert::IsFalse(queryResult.getIsTrue());
  }

  TEST_METHOD(TestGetNextStarGeneralStatementStatement) {
    spa::RelationshipStorage relationshipStorage;
    relationshipStorage.setNextStarTable(nextStarTable);
    relationshipStorage.setStatementTypeTable(statementTypeTable);
    std::vector<std::pair<int, int>> expected = { {3, 2}, {3, 3}, {3, 4}, {3, 5} };

    spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "c",
      spa::DesignEntityType::CALL));
    spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "s",
      spa::DesignEntityType::STMT));
    spa::QueryResult queryResult = relationshipStorage.getNextStarStatementStatement(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

    expected = { {1, 2}, {1, 3}, {1, 4}, {1, 5}, {2, 2}, {2, 3}, {2, 4}, {2, 5}, {3, 2}, {3, 3}, {3, 4}, {3, 5},
                 {4, 5} };
    firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "s1", spa::DesignEntityType::STMT));
    secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "s2", spa::DesignEntityType::STMT));
    queryResult = relationshipStorage.getNextStarStatementStatement(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

    expected = { {1, 2}, {1, 3}, {1, 4}, {1, 5}, {4, 5} };
    firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "re", spa::DesignEntityType::READ));
    queryResult = relationshipStorage.getNextStarStatementStatement(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

    firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "a", spa::DesignEntityType::ASSIGN));
    queryResult = relationshipStorage.getNextStarStatementStatement(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(queryResult.getLineNumberLineNumberPairs().empty());
  }
  };
}  // namespace UnitTesting
