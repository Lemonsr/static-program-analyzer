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
  TEST_CLASS(TestPKBModifies) {
    std::unordered_map<int, std::unordered_set<std::string>> modifiesTable = {
      {1, {"a"}},
      {2, {"b"}},
      {3, {"c"}},
      {4, {"d"}},
      {5, {"e"}},
      {6, {"a"}},
      {7, {"a", "b"}},
      {8, {"a"}},
      {9, {"b"}},
    };

    std::unordered_map<int, spa::StatementType> statementTypeTable = {
      {1, spa::StatementType::READ},
      {2, spa::StatementType::READ},
      {3, spa::StatementType::ASSIGN},
      {4, spa::StatementType::CALL},
      {5, spa::StatementType::CALL},
      {6, spa::StatementType::ASSIGN},
      {7, spa::StatementType::WHILE},
      {8, spa::StatementType::ASSIGN},
      {6, spa::StatementType::ASSIGN},
    };
    public:
      TEST_METHOD(TestAddModifies) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setModifiesTable(modifiesTable);
        relationshipStorage.setStatementTypeTable(statementTypeTable);

        Assert::IsTrue(relationshipStorage.addModifies("10", "z"));
        Assert::IsFalse(relationshipStorage.addModifies("10", "z"));
        Assert::IsTrue(relationshipStorage.addModifies("10", "y"));
      }

      TEST_METHOD(TestGetModifiesLineVarName) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setModifiesTable(modifiesTable);
        relationshipStorage.setStatementTypeTable(statementTypeTable);

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "1", {}));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LITERAL_STRING, "a", {}));
        spa::QueryResult queryResult = relationshipStorage.getModifiesLineVarName(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
        Assert::IsTrue(queryResult.getIsTrue());

        secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LITERAL_STRING, "b", {}));
        queryResult = relationshipStorage.getModifiesLineVarName(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
        Assert::IsFalse(queryResult.getIsTrue());
      }

      TEST_METHOD(TestGetModifiesLineUnderscore) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setModifiesTable(modifiesTable);
        relationshipStorage.setStatementTypeTable(statementTypeTable);

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "1", {}));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
        spa::QueryResult queryResult = relationshipStorage.getModifiesLineUnderscore(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
        Assert::IsTrue(queryResult.getIsTrue());

        firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "10", {}));
        queryResult = relationshipStorage.getModifiesLineUnderscore(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
        Assert::IsFalse(queryResult.getIsTrue());
      }

      TEST_METHOD(TestGetModifiesLineVar) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setModifiesTable(modifiesTable);
        relationshipStorage.setStatementTypeTable(statementTypeTable);
        std::vector<std::pair<int, std::string>> expected = { {1, "a"} };

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "1", {}));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "v",
                                                                       spa::DesignEntityType::VARIABLE));
        spa::QueryResult queryResult = relationshipStorage.getModifiesLineVar(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(expected == queryResult.getLineNumberNamePairs());

        firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "10", {}));
        queryResult = relationshipStorage.getModifiesLineVar(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(queryResult.getLineNumberNamePairs().empty());
      }

      TEST_METHOD(TestGetModifiesStmtVarName) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setModifiesTable(modifiesTable);
        relationshipStorage.setStatementTypeTable(statementTypeTable);
        std::vector<std::pair<int, std::string>> expected = { {1, "a"}, {6, "a"}, {7, "a"}, {8, "a"}};

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "s",
                                                                      spa::DesignEntityType::STMT));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LITERAL_STRING, "a", {}));
        spa::QueryResult queryResult = relationshipStorage.getModifiesStmtVarName(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(expected == queryResult.getLineNumberNamePairs());

        secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LITERAL_STRING, "z", {}));
        queryResult = relationshipStorage.getModifiesStmtVarName(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(queryResult.getLineNumberNamePairs().empty());

        expected = { {1, "a"}};
        firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "re", spa::DesignEntityType::READ));
        secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LITERAL_STRING, "a", {}));
        queryResult = relationshipStorage.getModifiesStmtVarName(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(expected == queryResult.getLineNumberNamePairs());
      }

      TEST_METHOD(TestGetModifiesStmtUnderscore) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setModifiesTable(modifiesTable);
        relationshipStorage.setStatementTypeTable(statementTypeTable);
        std::vector<std::pair<int, std::string>> expected;
        for (auto& itr = modifiesTable.begin(); itr != modifiesTable.end(); itr++) {
          int lineNumber = itr->first;
          std::unordered_set<std::string> varNames = itr->second;
          for (auto& itr2 = varNames.begin(); itr2 != varNames.end(); itr2++) {
            expected.push_back({ lineNumber, *itr2 });
          }
        }

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "s",
                                                                      spa::DesignEntityType::STMT));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
        spa::QueryResult queryResult = relationshipStorage.getModifiesStmtUnderscore(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(expected == queryResult.getLineNumberNamePairs());

        expected = { {3, "c"}, {6, "a"}, {8, "a"}};
        firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "a", spa::DesignEntityType::ASSIGN));
        queryResult = relationshipStorage.getModifiesStmtUnderscore(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(expected == queryResult.getLineNumberNamePairs());
      }

      TEST_METHOD(TestGetModifiesStmtVar) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setModifiesTable(modifiesTable);
        relationshipStorage.setStatementTypeTable(statementTypeTable);
        std::vector<std::pair<int, std::string>> expected;
        for (auto& itr = modifiesTable.begin(); itr != modifiesTable.end(); itr++) {
          int lineNumber = itr->first;
          std::unordered_set<std::string> varNames = itr->second;
          for (auto& itr2 = varNames.begin(); itr2 != varNames.end(); itr2++) {
            expected.push_back({ lineNumber, *itr2 });
          }
        }

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "s",
                                                                      spa::DesignEntityType::STMT));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "v",
                                                                       spa::DesignEntityType::VARIABLE));
        spa::QueryResult queryResult = relationshipStorage.getModifiesStmtVar(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(expected == queryResult.getLineNumberNamePairs());

        expected = { {4, "d"}, {5, "e"} };
        firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "c", spa::DesignEntityType::CALL));
        queryResult = relationshipStorage.getModifiesStmtVar(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(expected == queryResult.getLineNumberNamePairs());
      }

      TEST_METHOD(TestGetModifiesNested) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setModifiesTable(modifiesTable);
        relationshipStorage.setStatementTypeTable(statementTypeTable);
        std::vector<std::pair<int, std::string>> expected = { {7, "a"}, {7, "b"} };


        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "w",
          spa::DesignEntityType::WHILE));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
        spa::QueryResult queryResult = relationshipStorage.getModifiesStmtVar(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(expected == queryResult.getLineNumberNamePairs());
      }
  };
}  // namespace UnitTesting
