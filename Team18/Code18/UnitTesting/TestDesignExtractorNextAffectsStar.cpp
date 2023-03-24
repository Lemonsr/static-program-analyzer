#include <iostream>
#include <vector>

#include "stdafx.h"
#include "CppUnitTest.h"

#include "DesignExtractor.h"
#include "PKB.h"
#include "CFGNode.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace UnitTesting {
  TEST_CLASS(TestDesignExtractorNextAffectsStar) {
public:
  TEST_METHOD(TestNextAffectsStar) {
    spa::PKB pkb;
    std::vector<std::shared_ptr<spa::ProcedureStatement>> dummy;
    std::unordered_map<int, std::unordered_set<int>> input = {
      { 1, { 2, 3 }},
      { 2, { 5, 6 }},
      { 3, { 7, 8 }},
    };
    spa::DesignExtractor de(pkb, dummy);
    de.populateAffects();
    auto result = de.extractNextAffectsStar(input);
    Assert::IsTrue(result[1] == std::unordered_set<int> { 2, 3, 5, 6, 7, 8 });
    Assert::IsTrue(result[2] == std::unordered_set<int> { 5, 6 });
    Assert::IsTrue(result[3] == std::unordered_set<int> { 7, 8 });
  }
  };
}  // namespace UnitTesting
