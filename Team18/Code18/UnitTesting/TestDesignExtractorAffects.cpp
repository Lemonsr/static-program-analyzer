#include <iostream>
#include <vector>

#include "stdafx.h"
#include "CppUnitTest.h"

#include "DesignExtractor.h"
#include "PKB.h"
#include "CFGNode.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace UnitTesting {
  TEST_CLASS(TestDesignExtractorAffects) {
private:
  std::unordered_set<int> getLineNoSet(const std::unordered_map<int, std::unordered_set<int>>& table, int lineNo) {
    auto it = table.find(lineNo);
    if (it == table.end()) {
      return {};
    }
    return it->second;
  }
public:
  TEST_METHOD(TestAffectsSimple) {
    /*
     procedure dummy {
       x = 1;
       v = 1;
       read v;
       z = x + v;
       r = z;
     }
   */
    spa::PKB pkb;
    std::vector<std::shared_ptr<spa::ProcedureStatement>> dummy;
    pkb.addCfgNode(1, spa::CFGNode(1, { "x" }, {}, spa::StatementType::ASSIGN));
    pkb.addCfgNode(2, spa::CFGNode(2, { "v" }, {}, spa::StatementType::ASSIGN));
    pkb.addCfgNode(3, spa::CFGNode(3, { "v" }, {}, spa::StatementType::READ));
    pkb.addCfgNode(4, spa::CFGNode(4, { "z" }, { "x", "v" }, spa::StatementType::ASSIGN));
    pkb.addCfgNode(5, spa::CFGNode(5, { "r" }, { "z" }, spa::StatementType::ASSIGN));
    pkb.addEdge(1, 2);
    pkb.addEdge(2, 3);
    pkb.addEdge(3, 4);
    pkb.addEdge(4, 5);
    pkb.addCfgEndNode(5);
    spa::DesignExtractor de(pkb, dummy);
    de.populateAffects();
    spa::QueryResult result = pkb.getAffectsTable();
    auto& affectsTable = *(result.getIntToSetIntTable());
    Assert::IsTrue(getLineNoSet(affectsTable, 1) == std::unordered_set<int> { 4 });
    Assert::IsTrue(getLineNoSet(affectsTable, 2).empty());
    Assert::IsTrue(getLineNoSet(affectsTable, 3).empty());
    Assert::IsTrue(getLineNoSet(affectsTable, 4) == std::unordered_set<int> { 5 });
    Assert::IsTrue(getLineNoSet(affectsTable, 5).empty());
  }

  TEST_METHOD(TestAffectsWhile) {
    /*
      procedure dummy {
        x = 1;
        v = 1;
        while (v == 1) {
          v = v;
        }
        z = x + v;
        r = z + v;
      }
   */
    spa::PKB pkb;
    std::vector<std::shared_ptr<spa::ProcedureStatement>> dummy;
    pkb.addCfgNode(1, spa::CFGNode(1, { "x" }, {}, spa::StatementType::ASSIGN));
    pkb.addCfgNode(2, spa::CFGNode(2, { "v" }, {}, spa::StatementType::ASSIGN));
    pkb.addCfgNode(3, spa::CFGNode(3, { "v" }, { "v" }, spa::StatementType::WHILE));
    pkb.addCfgNode(4, spa::CFGNode(4, { "v" }, { "v" }, spa::StatementType::ASSIGN));
    pkb.addCfgNode(5, spa::CFGNode(5, { "z" }, { "x", "v" }, spa::StatementType::ASSIGN));
    pkb.addCfgNode(6, spa::CFGNode(6, { "r" }, { "z", "v"}, spa::StatementType::ASSIGN));
    pkb.addEdge(1, 2);
    pkb.addEdge(2, 3);
    pkb.addEdge(3, 4);
    pkb.addEdge(4, 3);
    pkb.addEdge(3, 5);
    pkb.addEdge(5, 6);
    pkb.addCfgEndNode(6);
    spa::DesignExtractor de(pkb, dummy);
    de.populateAffects();
    spa::QueryResult result = pkb.getAffectsTable();
    auto& affectsTable = *(result.getIntToSetIntTable());
    Assert::IsTrue(getLineNoSet(affectsTable, 1) == std::unordered_set<int> { 5 });
    Assert::IsTrue(getLineNoSet(affectsTable, 2) == std::unordered_set<int> { 4, 5, 6 });
    Assert::IsTrue(getLineNoSet(affectsTable, 3).empty());
    Assert::IsTrue(getLineNoSet(affectsTable, 4) == std::unordered_set<int> { 4, 5, 6 });
    Assert::IsTrue(getLineNoSet(affectsTable, 5) == std::unordered_set<int> { 6 });
    Assert::IsTrue(getLineNoSet(affectsTable, 6).empty());
  }

  TEST_METHOD(TestAffectsIf) {
    /*
      procedure dummy {
        x = 1;
        v = 1;
        if (v == 1) then {
          y = v;
        } else {
          v = v;
        }
        z = x + v;
        r = z + y;
      }
    */
    spa::PKB pkb;
    std::vector<std::shared_ptr<spa::ProcedureStatement>> dummy;
    pkb.addCfgNode(1, spa::CFGNode(1, { "x" }, {}, spa::StatementType::ASSIGN));
    pkb.addCfgNode(2, spa::CFGNode(2, { "v" }, {}, spa::StatementType::ASSIGN));
    pkb.addCfgNode(3, spa::CFGNode(3, { "v", "y" }, {"v"}, spa::StatementType::IF));
    pkb.addCfgNode(4, spa::CFGNode(4, { "y" }, { "v" }, spa::StatementType::ASSIGN));
    pkb.addCfgNode(5, spa::CFGNode(5, { "v" }, { "v" }, spa::StatementType::ASSIGN));
    pkb.addCfgNode(6, spa::CFGNode(6, { "z" }, { "x", "v" }, spa::StatementType::ASSIGN));
    pkb.addCfgNode(7, spa::CFGNode(7, { "r" }, { "z", "y"}, spa::StatementType::ASSIGN));
    pkb.addEdge(1, 2);
    pkb.addEdge(2, 3);
    pkb.addEdge(3, 4);
    pkb.addEdge(3, 5);
    pkb.addEdge(4, 6);
    pkb.addEdge(5, 6);
    pkb.addEdge(6, 7);
    pkb.addCfgEndNode(7);
    spa::DesignExtractor de(pkb, dummy);
    de.populateAffects();
    spa::QueryResult result = pkb.getAffectsTable();
    auto& affectsTable = *(result.getIntToSetIntTable());
    Assert::IsTrue(getLineNoSet(affectsTable, 1) == std::unordered_set<int> { 6 });
    Assert::IsTrue(getLineNoSet(affectsTable, 2) == std::unordered_set<int> { 4, 5, 6 });
    Assert::IsTrue(getLineNoSet(affectsTable, 3).empty());
    Assert::IsTrue(getLineNoSet(affectsTable, 4) == std::unordered_set<int> { 7 });
    Assert::IsTrue(getLineNoSet(affectsTable, 5) == std::unordered_set<int> { 6 });
    Assert::IsTrue(getLineNoSet(affectsTable, 6) == std::unordered_set<int> { 7 });
    Assert::IsTrue(getLineNoSet(affectsTable, 7).empty());
  }
  };
}  // namespace UnitTesting
