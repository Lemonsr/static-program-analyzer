#include <string>
#include <unordered_set>
#include <vector>
#include <utility>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "EntityStorage.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace UnitTesting {
  TEST_CLASS(TestPKBEntityStorage) {
    std::unordered_set<std::string> varTable = { "a", "b", "c" };
    std::unordered_set<std::string> constTable = { "1", "2", "3" };
    std::unordered_set<std::string> procTable = { "main", "func1", "func2" };

    TEST_METHOD(TestAddVar) {
      spa::EntityStorage entityStorage;
      entityStorage.setVarTable(varTable);

      Assert::IsTrue(entityStorage.addVar("d"));
      Assert::IsFalse(entityStorage.addVar("d"));
    }

    TEST_METHOD(TestAddConst) {
      spa::EntityStorage entityStorage;
      entityStorage.setConstTable(constTable);

      Assert::IsTrue(entityStorage.addConst("4"));
      Assert::IsFalse(entityStorage.addConst("4"));
    }

    TEST_METHOD(TestAddProc) {
      spa::EntityStorage entityStorage;
      entityStorage.setProcTable(procTable);

      Assert::IsTrue(entityStorage.addProc("func3"));
      Assert::IsFalse(entityStorage.addProc("func3"));
    }

    TEST_METHOD(TestGetVars) {
      spa::EntityStorage entityStorage;
      entityStorage.setVarTable(varTable);
      std::vector<std::string> expected(varTable.size());
      std::copy(varTable.begin(), varTable.end(), expected.begin());

      spa::QueryResult queryResult = entityStorage.getVars();
      Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
      Assert::IsTrue(queryResult.getNames() == expected);
    }

    TEST_METHOD(TestGetConsts) {
      spa::EntityStorage entityStorage;
      entityStorage.setConstTable(constTable);
      std::vector<std::string> expected(constTable.size());
      std::copy(constTable.begin(), constTable.end(), expected.begin());

      spa::QueryResult queryResult = entityStorage.getConsts();
      Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
      Assert::IsTrue(queryResult.getNames() == expected);
    }

    TEST_METHOD(TestGetProcs) {
      spa::EntityStorage entityStorage;
      entityStorage.setProcTable(procTable);
      std::vector<std::string> expected(procTable.size());
      std::copy(procTable.begin(), procTable.end(), expected.begin());

      spa::QueryResult queryResult = entityStorage.getProcs();
      Assert::IsTrue(queryResult.getQueryResultType() == spa::QueryResultType::TUPLE);
      Assert::IsTrue(queryResult.getNames() == expected);
    }
  };
}  // namespace UnitTesting
