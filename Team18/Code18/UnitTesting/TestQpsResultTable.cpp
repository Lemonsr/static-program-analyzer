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

  TEST_METHOD(TestGetColumns) {
    spa::QpsResultTable table;
    table.addHeader(spa::PqlArgument(spa::SYNONYM, "v", { spa::VARIABLE }));
    table.addHeader(spa::PqlArgument(spa::LINE_NO, "12", {}));
    table.addHeader(spa::PqlArgument(spa::SYNONYM, "x", { spa::STMT }));
    table.addRow({ spa::QpsValue("a"), spa::QpsValue(12), spa::QpsValue(12) });
    table.addRow({ spa::QpsValue("b"), spa::QpsValue(12), spa::QpsValue(13) });
    table.addRow({ spa::QpsValue("c"), spa::QpsValue(12), spa::QpsValue(14) });
    table.addRow({ spa::QpsValue("d"), spa::QpsValue(12), spa::QpsValue(15) });
    table.addRow({ spa::QpsValue("x"), spa::QpsValue(12), spa::QpsValue(16) });
    table.addRow({ spa::QpsValue("y"), spa::QpsValue(12), spa::QpsValue(17) });
    table.addRow({ spa::QpsValue("w"), spa::QpsValue(12), spa::QpsValue(18) });
    table.addRow({ spa::QpsValue("z"), spa::QpsValue(12), spa::QpsValue(19) });
    auto columnVals = table.getColumns({"v", "x"});
    Assert::AreEqual(columnVals.size(), size_t(8));
    Assert::IsTrue(columnVals[0][0] == spa::QpsValue("a"));
    Assert::IsTrue(columnVals[0][1] == spa::QpsValue(12));
    Assert::IsTrue(columnVals[1][0] == spa::QpsValue("b"));
    Assert::IsTrue(columnVals[1][1] == spa::QpsValue(13));
    Assert::IsTrue(columnVals[2][0] == spa::QpsValue("c"));
    Assert::IsTrue(columnVals[2][1] == spa::QpsValue(14));
    Assert::IsTrue(columnVals[3][0] == spa::QpsValue("d"));
    Assert::IsTrue(columnVals[3][1] == spa::QpsValue(15));
    Assert::IsTrue(columnVals[4][0] == spa::QpsValue("x"));
    Assert::IsTrue(columnVals[4][1] == spa::QpsValue(16));
    Assert::IsTrue(columnVals[5][0] == spa::QpsValue("y"));
    Assert::IsTrue(columnVals[5][1] == spa::QpsValue(17));
    Assert::IsTrue(columnVals[6][0] == spa::QpsValue("w"));
    Assert::IsTrue(columnVals[6][1] == spa::QpsValue(18));
    Assert::IsTrue(columnVals[7][0] == spa::QpsValue("z"));
    Assert::IsTrue(columnVals[7][1] == spa::QpsValue(19));
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
    table2.addHeader(spa::PqlArgument(spa::SYNONYM, "v1", { spa::VARIABLE }));
    table2.addRow({ spa::QpsValue(12), spa::QpsValue("alpha")});
    table2.addRow({ spa::QpsValue(14), spa::QpsValue("beta") });
    table2.addRow({ spa::QpsValue(15), spa::QpsValue("gamma") });
    resultTable = table1.innerJoin(table2);

    auto dim = resultTable.getDimension();
    Assert::AreEqual(dim.first, 4);
    Assert::AreEqual(dim.second, 3);
    auto columnVals = resultTable.getColumn("a");
    Assert::AreEqual(columnVals.size(), size_t(3));
    Assert::IsTrue(columnVals.find(spa::QpsValue(12)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(14)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(15)) != columnVals.end());
    columnVals = resultTable.getColumn("v");
    Assert::AreEqual(columnVals.size(), size_t(3));
    Assert::IsTrue(columnVals.find(spa::QpsValue("x")) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue("z")) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue("w")) != columnVals.end());
    columnVals = resultTable.getColumn("v1");
    Assert::AreEqual(columnVals.size(), size_t(3));
    Assert::IsTrue(columnVals.find(spa::QpsValue("alpha")) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue("beta")) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue("gamma")) != columnVals.end());

    spa::QpsResultTable table3;
    table3.addHeader(spa::PqlArgument(spa::SYNONYM, "v1", { spa::VARIABLE }));
    table3.addHeader(spa::PqlArgument(spa::SYNONYM, "a", { spa::ASSIGN }));
    table3.addRow({ spa::QpsValue("beta"), spa::QpsValue(14) });
    resultTable = resultTable.innerJoin(table3);

    dim = resultTable.getDimension();
    Assert::AreEqual(dim.first, 6);
    Assert::AreEqual(dim.second, 1);
    columnVals = resultTable.getColumn("a");
    Assert::AreEqual(columnVals.size(), size_t(1));
    Assert::IsTrue(columnVals.find(spa::QpsValue(14)) != columnVals.end());
    columnVals = resultTable.getColumn("v");
    Assert::AreEqual(columnVals.size(), size_t(1));
    Assert::IsTrue(columnVals.find(spa::QpsValue("z")) != columnVals.end());
    columnVals = resultTable.getColumn("v1");
    Assert::AreEqual(columnVals.size(), size_t(1));
    Assert::IsTrue(columnVals.find(spa::QpsValue("beta")) != columnVals.end());

    spa::QpsResultTable table4;
    table4.addHeader("s.stmt#");
    table4.addRow({ spa::QpsValue(1) });
    table4.addRow({ spa::QpsValue(2) });
    table4.addRow({ spa::QpsValue(3) });
    table4.addRow({ spa::QpsValue(4) });
    table4.addRow({ spa::QpsValue(5) });
    table4.addRow({ spa::QpsValue(6) });
    resultTable = resultTable.innerJoin(table4);
    dim = resultTable.getDimension();
    Assert::AreEqual(dim.first, 7);
    Assert::AreEqual(dim.second, 6);
    columnVals = resultTable.getColumn("s.stmt#");
    Assert::AreEqual(columnVals.size(), size_t(6));
    Assert::IsTrue(columnVals.find(spa::QpsValue(1)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(2)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(3)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(4)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(5)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(6)) != columnVals.end());
  }

  TEST_METHOD(TestMatchColumns) {
    spa::QpsResultTable table;
    table.addHeader("a1.stmt#");
    table.addHeader("v");
    table.addHeader("a2.stmt#");
    table.addRow({ spa::QpsValue(1), spa::QpsValue("match"), spa::QpsValue(1) });
    table.addRow({ spa::QpsValue(1), spa::QpsValue("unmatch"), spa::QpsValue(10) });
    table.addRow({ spa::QpsValue(2), spa::QpsValue("match"), spa::QpsValue(2) });
    table.addRow({ spa::QpsValue(2), spa::QpsValue("unmatch"), spa::QpsValue(20) });
    table.addRow({ spa::QpsValue(3), spa::QpsValue("match"), spa::QpsValue(3) });
    table.addRow({ spa::QpsValue(3), spa::QpsValue("unmatch"), spa::QpsValue(30) });
    spa::QpsResultTable result = table.matchColumns("a1.stmt#", "a2.stmt#");
    auto dim = result.getDimension();
    Assert::AreEqual(dim.first, 3);
    Assert::AreEqual(dim.second, 3);
    auto& rows = result.getRows();
    for (int i = 0; i < 3; ++i) {
      int num = i + 1;
      Assert::IsTrue(rows[i][0] == rows[i][2]);
      Assert::IsTrue(rows[i][1] == spa::QpsValue("match"));
    }
  }
  };
}  // namespace UnitTesting
