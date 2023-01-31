#include <iostream>
#include <sstream>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "Tokenizer.h"
#include "validator.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace UnitTesting {
    TEST_CLASS(TestValidator) {
public:
    TEST_METHOD(TestIsValidStmtToken) {
        spa::Token token = spa::Token(spa::TOKEN_READ, "read");
        spa::Validator validator = spa::Validator();
        Assert::IsTrue(validator.isValidStmtToken(token));
    }
    TEST_METHOD(TestIsValidCondExprToken) {
        spa::Token token = spa::Token(spa::TOKEN_BOOL_NOT, "!=");
        spa::Validator validator = spa::Validator();
        Assert::IsTrue(validator.isValidCondExprToken(token));
    }
    TEST_METHOD(TestIsValidRelExprToken) {
        spa::Token token = spa::Token(spa::TOKEN_COND_EQ, "==");
        spa::Validator validator = spa::Validator();
        Assert::IsTrue(validator.isValidRelExprToken(token));
    }
    TEST_METHOD(TestIsValidTermToken) {
        spa::Token token = spa::Token(spa::TOKEN_DIVIDE, "/");
        spa::Validator validator = spa::Validator();
        Assert::IsTrue(validator.isValidTermToken(token));
    }
    }; 
} // namespace UnitTesting
