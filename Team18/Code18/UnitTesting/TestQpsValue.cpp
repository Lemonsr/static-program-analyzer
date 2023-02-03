#include <iostream>
#include <string>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "QpsValue.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace UnitTesting {
  TEST_CLASS(TestQpsValue) {
public:
  TEST_METHOD(TestStringType) {
    spa::QpsValue val("hello");
    Assert::IsTrue(val.getType() == spa::QpsValueType::STRING);
    Assert::AreEqual(val.getString(), std::string("hello"));
  }

  TEST_METHOD(TestIntegerType) {
    spa::QpsValue val(12);
    Assert::IsTrue(val.getType() == spa::QpsValueType::INTEGER);
    Assert::AreEqual(val.getInteger(), 12);
  }

  TEST_METHOD(TestEquals) {
    spa::QpsValue str1("hello");
    spa::QpsValue str2("hello");
    Assert::IsTrue(str1 == str2);
    spa::QpsValue num1(1);
    spa::QpsValue num2(1);
    Assert::IsTrue(num1 == num2);
  }

  TEST_METHOD(TestNotEquals) {
    spa::QpsValue str1("hello");
    spa::QpsValue num1(12);
    Assert::IsTrue(str1 != num1);
    spa::QpsValue str2("bye");
    Assert::IsTrue(str1 != str2);
    spa::QpsValue num2(1);
    Assert::IsTrue(num1 != num2);
  }
  };
}  // namespace UnitTesting
