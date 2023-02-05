#include <iostream>
#include <sstream>
#include <utility>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "QpsValue.h"
#include "QpsResultTable.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace UnitTesting {
  TEST_CLASS(TestQpsResultTable) {
public:
  TEST_METHOD(TestDimension) {
    spa::QpsResultTable table;
    table.addHeader("s");
    table.addHeader("v");
    std::pair<int, int> emptyDim = table.getDimension();
    Assert::AreEqual(emptyDim.first, 2);
    Assert::AreEqual(emptyDim.second, 0);
    table.addHeader("p");
    table.addRow({ spa::QpsValue(12), spa::QpsValue("x"), spa::QpsValue("add")});
    std::pair<int, int> filledDim = table.getDimension();
    Assert::AreEqual(filledDim.first, 3);
    Assert::AreEqual(filledDim.second, 1);
  }

  TEST_METHOD(TestGetColumn) {
    spa::QpsResultTable table;
    table.addHeader(spa::PqlArgument(spa::SYNONYM, "v", { spa::VARIABLE }));
    table.addHeader(spa::PqlArgument(spa::LINE_NO, "12", {}));
    table.addRow({ spa::QpsValue("x"), spa::QpsValue(12) });
    table.addRow({ spa::QpsValue("y"), spa::QpsValue(12) });
    auto columnVals = table.getColumn("v");
    Assert::AreEqual(columnVals.size(), size_t(2));
    Assert::IsTrue(columnVals.find(spa::QpsValue("x")) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue("y")) != columnVals.end());
    columnVals = table.getColumn("s");
    Assert::AreEqual(columnVals.size(), size_t(0));
  }

  TEST_METHOD(TestInnerJoin) {
    spa::QpsResultTable resultTable;
    spa::QpsResultTable table1;
    table1.addHeader(spa::PqlArgument(spa::SYNONYM, "a", { spa::ASSIGN }));
    table1.addHeader(spa::PqlArgument(spa::SYNONYM, "v", { spa::VARIABLE }));
    table1.addRow({ spa::QpsValue(12), spa::QpsValue("x") });
    table1.addRow({ spa::QpsValue(13), spa::QpsValue("y")});
    table1.addRow({ spa::QpsValue(14), spa::QpsValue("z") });
    table1.addRow({ spa::QpsValue(15), spa::QpsValue("w") });

    spa::QpsResultTable table2;
    table2.addHeader(spa::PqlArgument(spa::SYNONYM, "a", { spa::ASSIGN }));
    table2.addRow({ spa::QpsValue(12) });
    table2.addRow({ spa::QpsValue(14) });
    resultTable = table1.innerJoin(table2);

    auto dim = resultTable.getDimension();
    Assert::AreEqual(dim.first, 3);
    Assert::AreEqual(dim.second, 2);
    auto columnVals = resultTable.getColumn("a");
    Assert::AreEqual(columnVals.size(), size_t(2));
    Assert::IsTrue(columnVals.find(spa::QpsValue(12)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(14)) != columnVals.end());
    columnVals = resultTable.getColumn("v");
    Assert::AreEqual(columnVals.size(), size_t(2));
    Assert::IsTrue(columnVals.find(spa::QpsValue("x")) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue("z")) != columnVals.end());

    spa::QpsResultTable table3;
    table3.addHeader(spa::PqlArgument(spa::SYNONYM, "v", { spa::ASSIGN }));
    table3.addRow({ spa::QpsValue("x") });
    resultTable = resultTable.innerJoin(table3);

    dim = resultTable.getDimension();
    Assert::AreEqual(dim.first, 4);
    Assert::AreEqual(dim.second, 1);
    columnVals = resultTable.getColumn("a");
    Assert::AreEqual(columnVals.size(), size_t(1));
    Assert::IsTrue(columnVals.find(spa::QpsValue(12)) != columnVals.end());
    columnVals = resultTable.getColumn("v");
    Assert::AreEqual(columnVals.size(), size_t(1));
    Assert::IsTrue(columnVals.find(spa::QpsValue("x")) != columnVals.end());
  }
  };
}  // namespace UnitTesting
