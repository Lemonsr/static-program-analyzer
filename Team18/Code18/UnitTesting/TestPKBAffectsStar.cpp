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
  TEST_CLASS(TestPKBAffectsStar) {
    std::unordered_map<int, std::unordered_set<int>> affectsStarTable = {
      {1, {3, 4}},
      {2, {3, 4}},
      {3, {4}}
    };

    std::unordered_map<int, spa::StatementType> statementTypeTable = {
      {1, spa::StatementType::ASSIGN},
      {2, spa::StatementType::ASSIGN},
      {3, spa::StatementType::ASSIGN},
      {4, spa::StatementType::ASSIGN}
    };

  TEST_METHOD(TestGetAffectsStarLineLine) {
    spa::RelationshipStorage relationshipStorage;
    relationshipStorage.setAffectsStarTable(affectsStarTable);
    relationshipStorage.setStatementTypeTable(statementTypeTable);

    spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "1", {}));
    spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "3", {}));
    spa::QueryResult queryResult = relationshipStorage.getAffectsStarLineLine(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
    Assert::IsTrue(queryResult.getIsTrue());

    secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "4", {}));
    queryResult = relationshipStorage.getAffectsStarLineLine(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
    Assert::IsTrue(queryResult.getIsTrue());

    secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "10", {}));
    queryResult = relationshipStorage.getAffectsStarLineLine(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
    Assert::IsFalse(queryResult.getIsTrue());
  }

  TEST_METHOD(TestGetAffectsStarLineStatement) {
    spa::RelationshipStorage relationshipStorage;
    relationshipStorage.setAffectsStarTable(affectsStarTable);
    relationshipStorage.setStatementTypeTable(statementTypeTable);
    std::vector<std::pair<int, int>> expected = { {1, 3}, {1, 4} };

    spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "1", {}));
    spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "s",
                                                                   spa::DesignEntityType::STMT));
    spa::QueryResult queryResult = relationshipStorage.getAffectsStarLineStatement(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

    secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "a", spa::DesignEntityType::ASSIGN));
    queryResult = relationshipStorage.getAffectsStarLineStatement(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

    expected = { {2, 3}, {2, 4} };
    firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "2", {}));
    secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "a",
                                                  spa::DesignEntityType::ASSIGN));
    queryResult = relationshipStorage.getAffectsStarLineStatement(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

    secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "w", spa::DesignEntityType::WHILE));
    queryResult = relationshipStorage.getAffectsStarLineStatement(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(queryResult.getLineNumberLineNumberPairs().empty());
  }

  TEST_METHOD(TestGetAffectsStarStatementLine) {
    spa::RelationshipStorage relationshipStorage;
    relationshipStorage.setAffectsStarTable(affectsStarTable);
    relationshipStorage.setStatementTypeTable(statementTypeTable);
    std::vector<std::pair<int, int>> expected = { {1, 3}, {2, 3} };

    spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "a",
                                                                  spa::DesignEntityType::ASSIGN));
    spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "3", {}));
    spa::QueryResult queryResult = relationshipStorage.getAffectsStarStatementLine(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

    firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "s", spa::DesignEntityType::STMT));
    queryResult = relationshipStorage.getAffectsStarStatementLine(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

    firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "if", spa::DesignEntityType::IF));
    queryResult = relationshipStorage.getAffectsStarStatementLine(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(queryResult.getLineNumberLineNumberPairs().empty());
  }

  TEST_METHOD(TestGetAffectsStarLineUnderscore) {
    spa::RelationshipStorage relationshipStorage;
    relationshipStorage.setAffectsStarTable(affectsStarTable);
    relationshipStorage.setStatementTypeTable(statementTypeTable);

    spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "2", {}));
    spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
    spa::QueryResult queryResult = relationshipStorage.getAffectsStarLineUnderscore(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
    Assert::IsTrue(queryResult.getIsTrue());

    firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "9", {}));
    queryResult = relationshipStorage.getAffectsStarLineUnderscore(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
    Assert::IsFalse(queryResult.getIsTrue());
  }

  TEST_METHOD(TestGetAffectsStarUnderscoreLine) {
    spa::RelationshipStorage relationshipStorage;
    relationshipStorage.setAffectsStarTable(affectsStarTable);
    relationshipStorage.setStatementTypeTable(statementTypeTable);

    spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
    spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "3", {}));
    spa::QueryResult queryResult = relationshipStorage.getAffectsStarUnderscoreLine(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
    Assert::IsTrue(queryResult.getIsTrue());

    secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "9", {}));
    queryResult = relationshipStorage.getAffectsStarUnderscoreLine(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
    Assert::IsFalse(queryResult.getIsTrue());
  }

  TEST_METHOD(TestGetAffectsStarStatementStatement) {
    spa::RelationshipStorage relationshipStorage;
    relationshipStorage.setAffectsStarTable(affectsStarTable);
    relationshipStorage.setStatementTypeTable(statementTypeTable);
    std::vector<std::pair<int, int>> expected = { {1, 3}, {1, 4}, {2, 3}, {2, 4}, {3, 4} };

    spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "s",
                                                                  spa::DesignEntityType::STMT));
    spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "a",
                                                                   spa::DesignEntityType::ASSIGN));
    spa::QueryResult queryResult = relationshipStorage.getAffectsStarStatementStatement(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

    firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "if", spa::DesignEntityType::IF));
    queryResult = relationshipStorage.getAffectsStarStatementStatement(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(queryResult.getLineNumberLineNumberPairs().empty());
  }

  TEST_METHOD(TestGetAffectsStarStatementUnderscore) {
    spa::RelationshipStorage relationshipStorage;
    relationshipStorage.setAffectsStarTable(affectsStarTable);
    relationshipStorage.setStatementTypeTable(statementTypeTable);
    std::vector<std::pair<int, int>> expected = { {1, 3}, {1, 4}, {2, 3}, {2, 4}, {3, 4} };

    spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "a",
                                                                  spa::DesignEntityType::ASSIGN));
    spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
    spa::QueryResult queryResult = relationshipStorage.getAffectsStarStatementUnderscore(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

    firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "s", spa::DesignEntityType::STMT));
    queryResult = relationshipStorage.getAffectsStarStatementUnderscore(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

    firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "p", spa::DesignEntityType::PRINT));
    queryResult = relationshipStorage.getAffectsStarStatementUnderscore(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(queryResult.getLineNumberLineNumberPairs().empty());
  }

  TEST_METHOD(TestGetAffectsStarUnderscoreStatement) {
    spa::RelationshipStorage relationshipStorage;
    relationshipStorage.setAffectsStarTable(affectsStarTable);
    relationshipStorage.setStatementTypeTable(statementTypeTable);
    std::vector<std::pair<int, int>> expected = { {1, 3}, {1, 4}, {2, 3}, {2, 4}, {3, 4} };

    spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
    spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "a",
                                                                   spa::DesignEntityType::ASSIGN));
    spa::QueryResult queryResult = relationshipStorage.getAffectsStarUnderscoreStatement(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

    secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "s", spa::DesignEntityType::STMT));
    queryResult = relationshipStorage.getAffectsStarUnderscoreStatement(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

    secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "re", spa::DesignEntityType::READ));
    queryResult = relationshipStorage.getAffectsStarUnderscoreStatement(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(queryResult.getLineNumberLineNumberPairs().empty());
  }

  TEST_METHOD(TestGetAffectsStarUnderscoreUnderscore) {
    spa::RelationshipStorage relationshipStorage;
    relationshipStorage.setAffectsStarTable(affectsStarTable);
    relationshipStorage.setStatementTypeTable(statementTypeTable);

    spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
    spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
    spa::QueryResult queryResult = relationshipStorage.getAffectsStarUnderscoreUnderscore(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
    Assert::IsTrue(queryResult.getIsTrue());

    relationshipStorage.setAffectsStarTable({});
    queryResult = relationshipStorage.getAffectsStarUnderscoreUnderscore(firstArg, secondArg);
    Assert::IsFalse(queryResult.getIsTrue());
  }

  TEST_METHOD(TestGetAffectsStarGeneralStatementStatement) {
    spa::RelationshipStorage relationshipStorage;
    relationshipStorage.setAffectsStarTable(affectsStarTable);
    relationshipStorage.setStatementTypeTable(statementTypeTable);
    std::vector<std::pair<int, int>> expected = { {1, 3}, {1, 4}, {2, 3}, {2, 4}, {3, 4} };

    spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "a",
                                                                  spa::DesignEntityType::ASSIGN));
    spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "s",
                                                                   spa::DesignEntityType::STMT));
    spa::QueryResult queryResult = relationshipStorage.getAffectsStarStatementStatement(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

    firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "s1", spa::DesignEntityType::STMT));
    secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "s2", spa::DesignEntityType::STMT));
    queryResult = relationshipStorage.getAffectsStarStatementStatement(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

    firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "re", spa::DesignEntityType::READ));
    queryResult = relationshipStorage.getAffectsStarStatementStatement(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(queryResult.getLineNumberLineNumberPairs().empty());
  }
  };
}  // namespace UnitTesting
