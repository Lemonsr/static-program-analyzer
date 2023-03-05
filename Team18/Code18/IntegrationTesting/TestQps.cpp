#include <memory>
#include <string>

#include "stdafx.h"
#include "CppUnitTest.h"

#include "QPS.h"
#include "PKB.h"
#include "PqlParser.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace IntegrationTesting {
TEST_CLASS(TestQPS) {
 public:
  TEST_METHOD(TestValidQuery) {
    spa::QPS qps;
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    std::string query = "variable v; Select v";
    spa::QpsResult result = qps.evaluate(query, *pkbManager);
    Assert::IsFalse(result.getErrorMessage().has_value());
  }

  TEST_METHOD(TestSyntacticallyInvalidQuery) {
    spa::QPS qps;
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    std::string query = "variable v; Select v;";
    spa::QpsResult result = qps.evaluate(query, *pkbManager);
    Assert::IsTrue(result.getErrorMessage().has_value());
    Assert::AreEqual(result.getErrorMessage().value(), std::string("SyntaxError"));
  }

  TEST_METHOD(TestInvalidArguments) {
    spa::QPS qps;
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    std::string query = "variable v; Select v such that Modifies(_, v)";
    spa::QpsResult result = qps.evaluate(query, *pkbManager);
    Assert::IsTrue(result.getErrorMessage().has_value());
    Assert::AreEqual(result.getErrorMessage().value(), std::string("SemanticError"));
  }

  TEST_METHOD(TestUnknownSynonym) {
    spa::QPS qps;
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    std::string query = "variable v; Select v such that Modifies(s, v)";
    spa::QpsResult result = qps.evaluate(query, *pkbManager);
    Assert::IsTrue(result.getErrorMessage().has_value());
    Assert::AreEqual(result.getErrorMessage().value(), std::string("SemanticError"));
  }
};
}  // namespace IntegrationTesting
