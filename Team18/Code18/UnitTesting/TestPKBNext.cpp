#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <utility>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "CFGStorage.h"
#include "RelationshipStorage.h"
#include "PqlArgument.h"
#include "Argument.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace UnitTesting {
  TEST_CLASS(TestPKBNext) {
    std::unordered_map<int, std::unordered_set<int>> nextTable = {
      {1, {2}},
      {2, {3}},
      {3, {4, 5}},
      {4, {7}},
      {5, {6}},
      {6, {7}},
      {7, {8, 9}},
      {8, {7}}
    };

    std::unordered_map<int, spa::StatementType> statementTypeTable = {
      {1, spa::StatementType::READ},
      {2, spa::StatementType::CALL},
      {3, spa::StatementType::IF},
      {4, spa::StatementType::CALL},
      {5, spa::StatementType::PRINT},
      {6, spa::StatementType::CALL},
      {7, spa::StatementType::WHILE},
      {8, spa::StatementType::PRINT},
      {9, spa::StatementType::ASSIGN}
    };
public:
  TEST_METHOD(TestAddNext) {
    spa::CFGStorage cfgStorage;
    spa::RelationshipStorage relationshipStorage;
    cfgStorage.setNextTable(nextTable);
    relationshipStorage.setStatementTypeTable(statementTypeTable);

    Assert::IsTrue(cfgStorage.addNext("10", "11"));
    Assert::IsFalse(cfgStorage.addNext("10", "11"));
    Assert::IsTrue(cfgStorage.addNext("10", "12"));
  }

  TEST_METHOD(TestGetNextLineLine) {
    spa::CFGStorage cfgStorage;
    spa::RelationshipStorage relationshipStorage;
    cfgStorage.setNextTable(nextTable);
    relationshipStorage.setStatementTypeTable(statementTypeTable);

    spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "3", {}));
    spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "4", {}));
    spa::QueryResult queryResult = cfgStorage.getNextLineLine(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
    Assert::IsTrue(queryResult.getIsTrue());

    secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "5", {}));
    queryResult = cfgStorage.getNextLineLine(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
    Assert::IsTrue(queryResult.getIsTrue());

    secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "10", {}));
    queryResult = cfgStorage.getNextLineLine(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
    Assert::IsFalse(queryResult.getIsTrue());
  }

  TEST_METHOD(TestGetNextLineStatement) {
    spa::CFGStorage cfgStorage;
    spa::RelationshipStorage relationshipStorage;
    cfgStorage.setNextTable(nextTable);
    relationshipStorage.setStatementTypeTable(statementTypeTable);
    std::vector<std::pair<int, int>> expected = { {7, 8}, {7, 9} };

    spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "7", {}));
    spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "s",
                                                                   spa::DesignEntityType::STMT));
    spa::QueryResult queryResult = cfgStorage.getNextLineStatement(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

    expected = { {7, 8} };
    secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "p", spa::DesignEntityType::PRINT));
    queryResult = cfgStorage.getNextLineStatement(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

    expected = { {4, 7} };
    firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "4", {}));
    secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "w",
                                                  spa::DesignEntityType::WHILE));
    queryResult = cfgStorage.getNextLineStatement(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

    firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "1", {}));
    queryResult = cfgStorage.getNextLineStatement(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(queryResult.getLineNumberLineNumberPairs().empty());
  }

  TEST_METHOD(TestGetNextStatementLine) {
    spa::CFGStorage cfgStorage;
    spa::RelationshipStorage relationshipStorage;
    cfgStorage.setNextTable(nextTable);
    relationshipStorage.setStatementTypeTable(statementTypeTable);
    std::vector<std::pair<int, int>> expected = { {7, 9} };

    spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "w",
      spa::DesignEntityType::WHILE));
    spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "9", {}));
    spa::QueryResult queryResult = cfgStorage.getNextStatementLine(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

    expected = { {7, 9} };
    firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "s", spa::DesignEntityType::STMT));
    queryResult = cfgStorage.getNextStatementLine(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

    firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "if", spa::DesignEntityType::IF));
    queryResult = cfgStorage.getNextStatementLine(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(queryResult.getLineNumberLineNumberPairs().empty());
  }

  TEST_METHOD(TestGetNextLineUnderscore) {
    spa::CFGStorage cfgStorage;
    spa::RelationshipStorage relationshipStorage;
    cfgStorage.setNextTable(nextTable);
    relationshipStorage.setStatementTypeTable(statementTypeTable);

    spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "2", {}));
    spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
    spa::QueryResult queryResult = cfgStorage.getNextLineUnderscore(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
    Assert::IsTrue(queryResult.getIsTrue());

    firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "9", {}));
    queryResult = cfgStorage.getNextLineUnderscore(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
    Assert::IsFalse(queryResult.getIsTrue());
  }

  TEST_METHOD(TestGetNextUnderscoreLine) {
    spa::CFGStorage cfgStorage;
    spa::RelationshipStorage relationshipStorage;
    cfgStorage.setNextTable(nextTable);
    relationshipStorage.setStatementTypeTable(statementTypeTable);

    spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
    spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "2", {}));
    spa::QueryResult queryResult = cfgStorage.getNextUnderscoreLine(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
    Assert::IsTrue(queryResult.getIsTrue());

    secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "1", {}));
    queryResult = cfgStorage.getNextUnderscoreLine(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
    Assert::IsFalse(queryResult.getIsTrue());
  }

  TEST_METHOD(TestGetNextStatementStatement) {
    spa::CFGStorage cfgStorage;
    spa::RelationshipStorage relationshipStorage;
    cfgStorage.setNextTable(nextTable);
    relationshipStorage.setStatementTypeTable(statementTypeTable);
    std::vector<std::pair<int, int>> expected = { {3, 5}, {7, 8} };

    spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "s",
                                                                  spa::DesignEntityType::STMT));
    spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "p",
                                                                   spa::DesignEntityType::PRINT));
    spa::QueryResult queryResult = cfgStorage.getNextStatementStatement(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

    expected = { {4, 7}, {6, 7}, {8, 7} };
    secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "w", spa::DesignEntityType::WHILE));
    queryResult = cfgStorage.getNextStatementStatement(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

    firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "if", spa::DesignEntityType::IF));
    queryResult = cfgStorage.getNextStatementStatement(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(queryResult.getLineNumberLineNumberPairs().empty());
  }

  TEST_METHOD(TestGetNextStatementUnderscore) {
    spa::CFGStorage cfgStorage;
    spa::RelationshipStorage relationshipStorage;
    cfgStorage.setNextTable(nextTable);
    relationshipStorage.setStatementTypeTable(statementTypeTable);
    std::vector<std::pair<int, int>> expected = { {3, 4}, {3, 5} };

    spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "if",
                                                                  spa::DesignEntityType::IF));
    spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
    spa::QueryResult queryResult = cfgStorage.getNextStatementUnderscore(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

    expected = { {1, 2}, {2, 3}, {3, 4}, {3, 5}, {4, 7}, {5, 6}, {6, 7}, {7, 8}, {7, 9}, {8, 7} };
    firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "s", spa::DesignEntityType::STMT));
    queryResult = cfgStorage.getNextStatementUnderscore(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

    firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "a", spa::DesignEntityType::ASSIGN));
    queryResult = cfgStorage.getNextStatementUnderscore(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(queryResult.getLineNumberLineNumberPairs().empty());
  }

  TEST_METHOD(TestGetNextUnderscoreStatement) {
    spa::CFGStorage cfgStorage;
    spa::RelationshipStorage relationshipStorage;
    cfgStorage.setNextTable(nextTable);
    relationshipStorage.setStatementTypeTable(statementTypeTable);
    std::vector<std::pair<int, int>> expected = { {3, 5}, {7, 8} };

    spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
    spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "p",
      spa::DesignEntityType::PRINT));
    spa::QueryResult queryResult = cfgStorage.getNextUnderscoreStatement(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

    expected = { {1, 2}, {2, 3}, {3, 4}, {3, 5}, {4, 7}, {5, 6}, {6, 7}, {7, 8}, {7, 9}, {8, 7} };
    secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "s", spa::DesignEntityType::STMT));
    queryResult = cfgStorage.getNextUnderscoreStatement(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

    secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "re", spa::DesignEntityType::READ));
    queryResult = cfgStorage.getNextUnderscoreStatement(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(queryResult.getLineNumberLineNumberPairs().empty());
  }

  TEST_METHOD(TestGetNextUnderscoreUnderscore) {
    spa::CFGStorage cfgStorage;
    spa::RelationshipStorage relationshipStorage;
    cfgStorage.setNextTable(nextTable);
    relationshipStorage.setStatementTypeTable(statementTypeTable);

    spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
    spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
    spa::QueryResult queryResult = cfgStorage.getNextUnderscoreUnderscore(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
    Assert::IsTrue(queryResult.getIsTrue());

    cfgStorage.setNextTable({});
    queryResult = cfgStorage.getNextUnderscoreUnderscore(firstArg, secondArg);
    Assert::IsFalse(queryResult.getIsTrue());
  }

  TEST_METHOD(TestGetNextGeneralStatementStatement) {
    spa::CFGStorage cfgStorage;
    spa::RelationshipStorage relationshipStorage;
    cfgStorage.setNextTable(nextTable);
    relationshipStorage.setStatementTypeTable(statementTypeTable);
    std::vector<std::pair<int, int>> expected = { {2, 3}, {4, 7}, {6, 7} };

    spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "c",
                                                                  spa::DesignEntityType::CALL));
    spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "s",
                                                                   spa::DesignEntityType::STMT));
    spa::QueryResult queryResult = cfgStorage.getNextStatementStatement(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

    expected = { {1, 2}, {2, 3}, {3, 4}, {3, 5}, {4, 7}, {5, 6}, {6, 7}, {7, 8}, {7, 9}, {8, 7} };
    firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "s1", spa::DesignEntityType::STMT));
    secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "s2", spa::DesignEntityType::STMT));
    queryResult = cfgStorage.getNextStatementStatement(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

    expected = { {1, 2} };
    firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "re", spa::DesignEntityType::READ));
    queryResult = cfgStorage.getNextStatementStatement(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(expected == queryResult.getLineNumberLineNumberPairs());

    firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "a", spa::DesignEntityType::ASSIGN));
    queryResult = cfgStorage.getNextStatementStatement(firstArg, secondArg);

    Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
    Assert::IsTrue(queryResult.getLineNumberLineNumberPairs().empty());
  }
  };
}  // namespace UnitTesting
