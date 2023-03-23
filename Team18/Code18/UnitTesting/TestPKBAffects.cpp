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
  TEST_CLASS(TestPKBAffects) {
    std::unordered_map<int, std::unordered_set<int>> affectsTable = {
      {1, {3, 4}},
      {2, {3}},
      {3, {4}}
    };

    std::unordered_map<int, spa::StatementType> statementTypeTable = {
      {1, spa::StatementType::ASSIGN},
      {2, spa::StatementType::ASSIGN},
      {3, spa::StatementType::ASSIGN},
      {4, spa::StatementType::ASSIGN}
    };
public:
  TEST_METHOD(TestAddAffects) {
    spa::RelationshipStorage relationshipStorage;
    relationshipStorage.setAffectsTable(affectsTable);
    relationshipStorage.setStatementTypeTable(statementTypeTable);

    Assert::IsTrue(relationshipStorage.addAffects("10", "11"));
    Assert::IsFalse(relationshipStorage.addAffects("10", "11"));
    Assert::IsTrue(relationshipStorage.addAffects("10", "12"));
  }

  TEST_METHOD(TestGetAffectsLineLine) {
    spa::RelationshipStorage relationshipStorage;
    relationshipStorage.setAffectsTable(affectsTable);
    relationshipStorage.setStatementTypeTable(statementTypeTable);

    spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "1", {}));
    spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "3", {}));
    spa::QueryResult queryResult = relationshipStorage.getAffectsLineLine(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
    Assert::IsTrue(queryResult.getIsTrue());

    secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "4", {}));
    queryResult = relationshipStorage.getAffectsLineLine(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
    Assert::IsTrue(queryResult.getIsTrue());

    secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "10", {}));
    queryResult = relationshipStorage.getAffectsLineLine(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
    Assert::IsFalse(queryResult.getIsTrue());
  }

  TEST_METHOD(TestGetAffectsLineStatement) {
    spa::RelationshipStorage relationshipStorage;
    relationshipStorage.setAffectsTable(affectsTable);
    relationshipStorage.setStatementTypeTable(statementTypeTable);
    std::vector<std::pair<int, int>> expected = { {1, 3}, {1, 4} };

    spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "1", {}));
    spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "s",
                                                                   spa::DesignEntityType::STMT));
    spa::QueryResult queryResult = relationshipStorage.getAffectsLineStatement(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

    secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "a", spa::DesignEntityType::ASSIGN));
    queryResult = relationshipStorage.getAffectsLineStatement(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

    expected = { {2, 3} };
    firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "2", {}));
    secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "a",
                                                  spa::DesignEntityType::ASSIGN));
    queryResult = relationshipStorage.getAffectsLineStatement(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

    secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "w", spa::DesignEntityType::WHILE));
    queryResult = relationshipStorage.getAffectsLineStatement(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(queryResult.getLineNumberLineNumberPairs().empty());
  }

  TEST_METHOD(TestGetAffectsStatementLine) {
    spa::RelationshipStorage relationshipStorage;
    relationshipStorage.setAffectsTable(affectsTable);
    relationshipStorage.setStatementTypeTable(statementTypeTable);
    std::vector<std::pair<int, int>> expected = { {1, 3}, {2, 3} };

    spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "a",
                                                                  spa::DesignEntityType::ASSIGN));
    spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "3", {}));
    spa::QueryResult queryResult = relationshipStorage.getAffectsStatementLine(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

    firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "s", spa::DesignEntityType::STMT));
    queryResult = relationshipStorage.getAffectsStatementLine(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

    firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "if", spa::DesignEntityType::IF));
    queryResult = relationshipStorage.getAffectsStatementLine(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(queryResult.getLineNumberLineNumberPairs().empty());
  }

  TEST_METHOD(TestGetAffectsLineUnderscore) {
    spa::RelationshipStorage relationshipStorage;
    relationshipStorage.setAffectsTable(affectsTable);
    relationshipStorage.setStatementTypeTable(statementTypeTable);

    spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "2", {}));
    spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
    spa::QueryResult queryResult = relationshipStorage.getAffectsLineUnderscore(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
    Assert::IsTrue(queryResult.getIsTrue());

    firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "9", {}));
    queryResult = relationshipStorage.getAffectsLineUnderscore(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
    Assert::IsFalse(queryResult.getIsTrue());
  }

  TEST_METHOD(TestGetAffectsUnderscoreLine) {
    spa::RelationshipStorage relationshipStorage;
    relationshipStorage.setAffectsTable(affectsTable);
    relationshipStorage.setStatementTypeTable(statementTypeTable);

    spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
    spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "2", {}));
    spa::QueryResult queryResult = relationshipStorage.getAffectsUnderscoreLine(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
    Assert::IsTrue(queryResult.getIsTrue());

    secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "9", {}));
    queryResult = relationshipStorage.getAffectsUnderscoreLine(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
    Assert::IsFalse(queryResult.getIsTrue());
  }

  TEST_METHOD(TestGetAffectsStatementStatement) {
    spa::RelationshipStorage relationshipStorage;
    relationshipStorage.setAffectsTable(affectsTable);
    relationshipStorage.setStatementTypeTable(statementTypeTable);
    std::vector<std::pair<int, int>> expected = { {1, 3}, {1, 4}, {2, 3}, {3, 4} } };

    spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "s",
                                                                  spa::DesignEntityType::STMT));
    spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "a",
                                                                   spa::DesignEntityType::ASSIGN));
    spa::QueryResult queryResult = relationshipStorage.getAffectsStatementStatement(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

    firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "if", spa::DesignEntityType::IF));
    queryResult = relationshipStorage.getAffectsStatementStatement(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(queryResult.getLineNumberLineNumberPairs().empty());
  }

  TEST_METHOD(TestGetAffectsStatementUnderscore) {
    spa::RelationshipStorage relationshipStorage;
    relationshipStorage.setAffectsTable(affectsTable);
    relationshipStorage.setStatementTypeTable(statementTypeTable);
    std::vector<std::pair<int, int>> expected = { {1, 3}, {1, 4}, {2, 3}, {3, 4} }
  };

    spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "a",
                                                                  spa::DesignEntityType::ASSIGN));
    spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
    spa::QueryResult queryResult = relationshipStorage.getAffectsStatementUnderscore(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

    firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "s", spa::DesignEntityType::STMT));
    queryResult = relationshipStorage.getAffectsStatementUnderscore(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

    firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "p", spa::DesignEntityType::PRINT));
    queryResult = relationshipStorage.getAffectsStatementUnderscore(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(queryResult.getLineNumberLineNumberPairs().empty());
  }

  TEST_METHOD(TestGetAffectsUnderscoreStatement) {
    spa::RelationshipStorage relationshipStorage;
    relationshipStorage.setAffectsTable(affectsTable);
    relationshipStorage.setStatementTypeTable(statementTypeTable);
    std::vector<std::pair<int, int>> expected = { {1, 3}, {1, 4}, {2, 3}, {3, 4} } };

    spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
    spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "a",
                                                                   spa::DesignEntityType::ASSIGN));
    spa::QueryResult queryResult = relationshipStorage.getAffectsUnderscoreStatement(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());
    =
    secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "s", spa::DesignEntityType::STMT));
    queryResult = relationshipStorage.getAffectsUnderscoreStatement(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

    secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "re", spa::DesignEntityType::READ));
    queryResult = relationshipStorage.getAffectsUnderscoreStatement(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(queryResult.getLineNumberLineNumberPairs().empty());
  }

  TEST_METHOD(TestGetAffectsUnderscoreUnderscore) {
    spa::RelationshipStorage relationshipStorage;
    relationshipStorage.setAffectsTable(affectsTable);
    relationshipStorage.setStatementTypeTable(statementTypeTable);

    spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
    spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
    spa::QueryResult queryResult = relationshipStorage.getAffectsUnderscoreUnderscore(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
    Assert::IsTrue(queryResult.getIsTrue());

    relationshipStorage.setAffectsTable({});
    queryResult = relationshipStorage.getAffectsUnderscoreUnderscore(firstArg, secondArg);
    Assert::IsFalse(queryResult.getIsTrue());
  }

  TEST_METHOD(TestGetAffectsGeneralStatementStatement) {
    spa::RelationshipStorage relationshipStorage;
    relationshipStorage.setAffectsTable(affectsTable);
    relationshipStorage.setStatementTypeTable(statementTypeTable);
    std::vector<std::pair<int, int>> expected = { {1, 3}, {1, 4}, {2, 3}, {3, 4} } };

    spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "a,
                                                                  spa::DesignEntityType::ASSIGN));
    spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "s",
                                                                   spa::DesignEntityType::STMT));
    spa::QueryResult queryResult = relationshipStorage.getAffectsStatementStatement(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());
    
    firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "s1", spa::DesignEntityType::STMT));
    secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "s2", spa::DesignEntityType::STMT));
    queryResult = relationshipStorage.getAffectsStatementStatement(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

    firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "re", spa::DesignEntityType::READ));
    queryResult = relationshipStorage.getAffectsStatementStatement(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(queryResult.getLineNumberLineNumberPairs().empty());
  }
  };
}  // namespace UnitTesting
