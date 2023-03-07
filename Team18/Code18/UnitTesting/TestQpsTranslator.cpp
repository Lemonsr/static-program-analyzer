#include "stdafx.h"
#include "CppUnitTest.h"
#include "QpsResultTable.h"
#include "QpsTranslator.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace UnitTesting {
  TEST_CLASS(TestQpsTranslator) {
    spa::QpsResultTable resultTable;
public:
  TestQpsTranslator() {
    resultTable.addHeader("p");
    resultTable.addHeader("p.procName");
    resultTable.addHeader("s");
    resultTable.addHeader("s.stmt#");
    resultTable.addHeader("v");
    resultTable.addHeader("v.varName");
    resultTable.addRow({ spa::QpsValue("func1"), spa::QpsValue("func1"), spa::QpsValue(1), spa::QpsValue(1), spa::QpsValue("a"), spa::QpsValue("a") });
    resultTable.addRow({ spa::QpsValue("func2"), spa::QpsValue("func2"), spa::QpsValue(2), spa::QpsValue(2), spa::QpsValue("b"), spa::QpsValue("b") });
    resultTable.addRow({ spa::QpsValue("func3"), spa::QpsValue("func3"), spa::QpsValue(3), spa::QpsValue(3), spa::QpsValue("c"), spa::QpsValue("c") });
    resultTable.addRow({ spa::QpsValue("func4"), spa::QpsValue("func4"), spa::QpsValue(4), spa::QpsValue(4), spa::QpsValue("d"), spa::QpsValue("d") });
    resultTable.addRow({ spa::QpsValue("func5"), spa::QpsValue("func5"), spa::QpsValue(5), spa::QpsValue(5), spa::QpsValue("e"), spa::QpsValue("e") });
  }
  TEST_METHOD(TestSimpleSelect) {
    spa::QpsTranslator translator(resultTable);
    std::vector<std::string> selectColumns;
    selectColumns.push_back("s");
    std::list<std::string> expected = { "1", "2", "3", "4", "5" };
    std::list<std::string> result = translator.translate(spa::SelectClauseType::SELECT_TUPLE, selectColumns);
    Assert::IsTrue(result == expected);

    selectColumns.clear();
    selectColumns.push_back("p.procName");
    expected = { "func1", "func2", "func3", "func4", "func5" };
    result = translator.translate(spa::SelectClauseType::SELECT_TUPLE, selectColumns);
    Assert::IsTrue(result == expected);
  }

  TEST_METHOD(TestTupleDiffColumn) {
    spa::QpsTranslator translator(resultTable);
    std::vector<std::string> selectColumns;
    selectColumns.push_back("p");
    selectColumns.push_back("v.varName");
    selectColumns.push_back("s");
    std::list<std::string> expected = { "func1 a 1", "func2 b 2", "func3 c 3", "func4 d 4", "func5 e 5" };
    std::list<std::string> result = translator.translate(spa::SelectClauseType::SELECT_TUPLE, selectColumns);
    Assert::IsTrue(result == expected);
  }

  TEST_METHOD(TestTupleSameColumn) {
    spa::QpsTranslator translator(resultTable);
    std::vector<std::string> selectColumns;
    selectColumns.push_back("s");
    selectColumns.push_back("s");
    std::list<std::string> expected = { "1 1", "2 2", "3 3", "4 4", "5 5" };
    std::list<std::string> result = translator.translate(spa::SelectClauseType::SELECT_TUPLE, selectColumns);
    Assert::IsTrue(result == expected);
  }

  TEST_METHOD(TestBooleanTrue) {
    spa::QpsTranslator translator(resultTable);
    std::vector<std::string> selectColumns;
    std::list<std::string> expected = { "TRUE" };
    std::list<std::string> result = translator.translate(spa::SelectClauseType::SELECT_BOOLEAN, selectColumns);
    Assert::IsTrue(result == expected);
  }

  TEST_METHOD(TestBooleanFalse) {
    spa::QpsResultTable emptyTable;
    spa::QpsTranslator translator(emptyTable);
    std::vector<std::string> selectColumns;
    std::list<std::string> expected = { "FALSE" };
    std::list<std::string> result = translator.translate(spa::SelectClauseType::SELECT_BOOLEAN, selectColumns);
    Assert::IsTrue(result == expected);
  }
  };
}  // namespace UnitTesting