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
  TEST_CLASS(TestPKBUses) {
    std::unordered_map<int, std::unordered_set<std::string>> usesTable = {
      {1, {"a", "b", "c"}},
      {2, {"d", "e", "f"}},
      {3, {"g", "h", "i"}},
      {4, {"j", "k", "l"}},
      {5, {"m"}},
      {6, {"n"}},
      {7, {"a", "b", "c"}},
      {8, {"a", "b"}},
      {9, {"c"}},
      {10, {"d"}},
    };

    std::unordered_map<int, spa::StatementType> statementTypeTable = {
      {1, spa::StatementType::ASSIGN},
      {2, spa::StatementType::ASSIGN},
      {3, spa::StatementType::CALL},
      {4, spa::StatementType::CALL},
      {5, spa::StatementType::PRINT},
      {6, spa::StatementType::PRINT},
      {7, spa::StatementType::IF},
      {8, spa::StatementType::ASSIGN},
      {9, spa::StatementType::PRINT},
      {10, spa::StatementType::WHILE}
    };

    public:
      TEST_METHOD(TestAddUses) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setUsesTable(usesTable);
        relationshipStorage.setStatementTypeTable(statementTypeTable);

        Assert::IsTrue(relationshipStorage.addUses("8", "u"));
        Assert::IsFalse(relationshipStorage.addUses("8", "u"));
      }

      TEST_METHOD(TestGetUsesLineVarName) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setUsesTable(usesTable);
        relationshipStorage.setStatementTypeTable(statementTypeTable);

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "1", {}));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::VARIABLE_NAME, "a", {}));
        spa::QueryResult queryResult = relationshipStorage.getUsesLineVarName(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
        Assert::IsTrue(queryResult.getIsTrue());

        secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::VARIABLE_NAME, "d", {}));
        queryResult = relationshipStorage.getModifiesLineVarName(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
        Assert::IsFalse(queryResult.getIsTrue());
      }

      TEST_METHOD(TestGetUsesLineUnderscore) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setUsesTable(usesTable);
        relationshipStorage.setStatementTypeTable(statementTypeTable);

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "1", {}));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
        spa::QueryResult queryResult = relationshipStorage.getUsesLineUnderscore(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
        Assert::IsTrue(queryResult.getIsTrue());

        firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "20", {}));
        queryResult = relationshipStorage.getUsesLineUnderscore(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::BOOL);
        Assert::IsFalse(queryResult.getIsTrue());
      }

      TEST_METHOD(TestGetUsesLineVar) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setUsesTable(usesTable);
        relationshipStorage.setStatementTypeTable(statementTypeTable);
        std::vector<std::pair<int, std::string>> expected = { {1, "a"}, {1, "b"}, {1, "c"}};


        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "1", {}));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "v",
                                                                       spa::DesignEntityType::VARIABLE));
        spa::QueryResult queryResult = relationshipStorage.getUsesLineVar(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(expected == queryResult.getLineNumberVariablePairs());

        firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::LINE_NO, "20", {}));
        queryResult = relationshipStorage.getUsesLineVar(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(queryResult.getLineNumberVariablePairs().empty());
      }

      TEST_METHOD(TestGetUsesStmtVarName) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setUsesTable(usesTable);
        relationshipStorage.setStatementTypeTable(statementTypeTable);
        std::vector<std::pair<int, std::string>> expected = { {1, "a"}, {1, "b"}, {1, "c"}, {7, "a"}, {7, "b"},
                                                              {7, "c"}, {8, "a"}, {8, "b"} };

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "s",
          spa::DesignEntityType::STMT));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::VARIABLE_NAME, "a", {}));
        spa::QueryResult queryResult = relationshipStorage.getUsesStmtVarName(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(expected == queryResult.getLineNumberVariablePairs());

        secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::VARIABLE_NAME, "z", {}));
        queryResult = relationshipStorage.getUsesStmtVarName(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(queryResult.getLineNumberVariablePairs().empty());

        expected = { {10, "d"} };
        firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "w", spa::DesignEntityType::WHILE));
        secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::VARIABLE_NAME, "d", {}));
        queryResult = relationshipStorage.getUsesStmtVarName(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(expected == queryResult.getLineNumberVariablePairs());
      }

      TEST_METHOD(TestGetUsesStmtUnderscore) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setUsesTable(usesTable);
        relationshipStorage.setStatementTypeTable(statementTypeTable);
        std::vector<std::pair<int, std::string>> expected;
        for (auto& itr = usesTable.begin(); itr != usesTable.end(); itr++) {
          int lineNumber = itr->first;
          std::unordered_set<std::string> varNames = itr->second;
          for (auto& itr2 = varNames.begin(); itr2 != varNames.end(); itr2++) {
            expected.push_back({ lineNumber, *itr2 });
          }
        }

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "s",
          spa::DesignEntityType::STMT));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
        spa::QueryResult queryResult = relationshipStorage.getUsesStmtUnderscore(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(expected == queryResult.getLineNumberVariablePairs());

        expected = { {5, "m"}, {6, "n"}, {9, "c"}};
        firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "p", spa::DesignEntityType::PRINT));
        queryResult = relationshipStorage.getUsesStmtUnderscore(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(expected == queryResult.getLineNumberVariablePairs());
      }

      TEST_METHOD(TestGetUsesStmtVar) {
        spa::RelationshipStorage relationshipStorage;
        relationshipStorage.setUsesTable(usesTable);
        relationshipStorage.setStatementTypeTable(statementTypeTable);
        std::vector<std::pair<int, std::string>> expected;
        for (auto& itr = usesTable.begin(); itr != usesTable.end(); itr++) {
          int lineNumber = itr->first;
          std::unordered_set<std::string> varNames = itr->second;
          for (auto& itr2 = varNames.begin(); itr2 != varNames.end(); itr2++) {
            expected.push_back({ lineNumber, *itr2 });
          }
        }

        spa::PKBQueryArg firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "s",
          spa::DesignEntityType::STMT));
        spa::PKBQueryArg secondArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::WILDCARD, "_", {}));
        spa::QueryResult queryResult = relationshipStorage.getUsesStmtVar(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(expected == queryResult.getLineNumberVariablePairs());

        expected = { {7, "a"}, {7, "b"}, {7, "c"}};
        firstArg = spa::PKBQueryArg(spa::PqlArgument(spa::ArgumentType::SYNONYM, "i", spa::DesignEntityType::IF));
        queryResult = relationshipStorage.getUsesStmtVar(firstArg, secondArg);

        Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
        Assert::IsTrue(expected == queryResult.getLineNumberVariablePairs());
      }
  };
}  // namespace UnitTesting
