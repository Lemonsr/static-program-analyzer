#include "stdafx.h"
#include <iostream>
#include <sstream>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "PKB.h"
#include "PKBManager.h"
#include "SP.h"
#include "SpValidator.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace UnitTesting {
    TEST_CLASS(TestSpValidator) {
	// procedure proc { x = z + v ;}
    TEST_METHOD(TestSpValidatorGrammarValidProcedure) {
		spa::Stream<spa::Token> tokens;
		tokens.pushBack({ spa::TOKEN_PROCEDURE, "procedure"});
		tokens.pushBack({ spa::TOKEN_NAME, "proc" });
		tokens.pushBack({ spa::TOKEN_OPEN_BRACE, "{" });
		tokens.pushBack({ spa::TOKEN_NAME, "x" });
		tokens.pushBack({ spa::TOKEN_EQUAL, "=" });
		tokens.pushBack({ spa::TOKEN_NAME, "z" });
		tokens.pushBack({ spa::TOKEN_PLUS, "+" });
		tokens.pushBack({ spa::TOKEN_NAME, "v" });
		tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
		tokens.pushBack({ spa::TOKEN_CLOSE_BRACE, "}" });
        spa::SpValidator validator(tokens);
		Assert::IsTrue(validator.validateGrammar());
	}

	TEST_METHOD(TestSpValidatorGrammarInvalidProcedure) {
		spa::Stream<spa::Token> tokens;
		tokens.pushBack({ spa::TOKEN_PROCEDURE, "procedure" });
		tokens.pushBack({ spa::TOKEN_OPEN_BRACE, "{" });
		tokens.pushBack({ spa::TOKEN_NAME, "x" });
		tokens.pushBack({ spa::TOKEN_EQUAL, "=" });
		tokens.pushBack({ spa::TOKEN_NAME, "z" });
		tokens.pushBack({ spa::TOKEN_NAME, "+" });
		tokens.pushBack({ spa::TOKEN_NAME, "v" });
		spa::SpValidator validator(tokens);
		Assert::ExpectException<std::exception>([&] { validator.validateGrammar(); });
	}


	//TEST_METHOD(TestSpValidatorGrammarInvalid) {
	//	spa::Stream<spa::Token> tokens;
	//	tokens.pushBack({ spa::TOKEN_PROCEDURE, "procedure" });
	//	tokens.pushBack({ spa::TOKEN_OPEN_BRACES, "{" });
	//	tokens.pushBack({ spa::TOKEN_NAME, "x" });
	//	tokens.pushBack({ spa::TOKEN_EQUAL, "=" });
	//	tokens.pushBack({ spa::TOKEN_INTEGER, "12" });
	//	tokens.pushBack({ spa::TOKEN_MODULO, "%" });
	//	tokens.pushBack({ spa::TOKEN_NAME, "v" });
	//	tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
	//	Stream<tokens> convertedTokens = convertToken();
	//	SpValidator validator(convertedTokens);
	//	Assert::AreEqual(validator.validateGrammar(), false);
	//}
    };
}





	//// procedure { x = z + v;}
	//tokens.pushBack({ spa::TOKEN_PROCEDURE, "procedure" });
	//tokens.pushBack({ spa::TOKEN_OPEN_BRACES, "{" });
	//tokens.pushBack({ spa::TOKEN_NAME, "x" });
	//tokens.pushBack({ spa::TOKEN_EQUAL, "=" });
	//tokens.pushBack({ spa::TOKEN_NAME, "z" });
	//tokens.pushBack({ spa::TOKEN_NAME, "+" });
	//tokens.pushBack({ spa::TOKEN_NAME, "v" });
	//tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
	//tokens.pushBack({ spa::TOKEN_CLOSE_BRACES, "}" });


	//// procedure { x = z + ;}
	//tokens.pushBack({ spa::TOKEN_PROCEDURE, "procedure" });
	//tokens.pushBack({ spa::TOKEN_OPEN_BRACES, "{" });
	//tokens.pushBack({ spa::TOKEN_NAME, "x" });
	//tokens.pushBack({ spa::TOKEN_EQUAL, "=" });
	//tokens.pushBack({ spa::TOKEN_NAME, "z" });
	//tokens.pushBack({ spa::TOKEN_NAME, "+" });
	//tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
	//tokens.pushBack({ spa::TOKEN_CLOSE_BRACES, "}" });

	//// procedure { x = z + }
	//tokens.pushBack({ spa::TOKEN_PROCEDURE, "procedure" });
	//tokens.pushBack({ spa::TOKEN_OPEN_BRACES, "{" });
	//tokens.pushBack({ spa::TOKEN_NAME, "x" });
	//tokens.pushBack({ spa::TOKEN_EQUAL, "=" });
	//tokens.pushBack({ spa::TOKEN_NAME, "z" });
	//tokens.pushBack({ spa::TOKEN_NAME, "+" });
	//tokens.pushBack({ spa::TOKEN_CLOSE_BRACES, "}" });


	//// procedure { x = z}
	//tokens.pushBack({ spa::TOKEN_PROCEDURE, "procedure" });
	//tokens.pushBack({ spa::TOKEN_OPEN_BRACES, "{" });
	//tokens.pushBack({ spa::TOKEN_NAME, "x" });
	//tokens.pushBack({ spa::TOKEN_EQUAL, "=" });
	//tokens.pushBack({ spa::TOKEN_NAME, "z" });
	//tokens.pushBack({ spa::TOKEN_CLOSE_BRACES, "}" });

	//// procedure { x = z;}
	//tokens.pushBack({ spa::TOKEN_PROCEDURE, "procedure" });
	//tokens.pushBack({ spa::TOKEN_OPEN_BRACES, "{" });
	//tokens.pushBack({ spa::TOKEN_NAME, "x" });
	//tokens.pushBack({ spa::TOKEN_EQUAL, "=" });
	//tokens.pushBack({ spa::TOKEN_NAME, "z" });
	//tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
	//tokens.pushBack({ spa::TOKEN_CLOSE_BRACES, "}" });



	//// procedure { x = 1}
	//tokens.pushBack({ spa::TOKEN_PROCEDURE, "procedure" });
	//tokens.pushBack({ spa::TOKEN_OPEN_BRACES, "{" });
	//tokens.pushBack({ spa::TOKEN_NAME, "x" });
	//tokens.pushBack({ spa::TOKEN_EQUAL, "=" });
	//tokens.pushBack({ spa::TOKEN_INTEGER, "1" });
	//tokens.pushBack({ spa::TOKEN_CLOSE_BRACES, "}" });


	//// procedure { x = ;}
	//tokens.pushBack({ spa::TOKEN_PROCEDURE, "procedure" });
	//tokens.pushBack({ spa::TOKEN_OPEN_BRACES, "{" });
	//tokens.pushBack({ spa::TOKEN_NAME, "x" });
	//tokens.pushBack({ spa::TOKEN_EQUAL, "=" });
	//tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
	//tokens.pushBack({ spa::TOKEN_CLOSE_BRACES, "}" });

	//// procedure { x }
	//tokens.pushBack({ spa::TOKEN_PROCEDURE, "procedure" });
	//tokens.pushBack({ spa::TOKEN_OPEN_BRACES, "{" });
	//tokens.pushBack({ spa::TOKEN_NAME, "x" });
	//tokens.pushBack({ spa::TOKEN_CLOSE_BRACES, "}" });

	//// procedure { x = z + 1;}
	//tokens.pushBack({ spa::TOKEN_PROCEDURE, "procedure" });
	//tokens.pushBack({ spa::TOKEN_OPEN_BRACES, "{" });
	//tokens.pushBack({ spa::TOKEN_NAME, "x" });
	//tokens.pushBack({ spa::TOKEN_EQUAL, "=" });
	//tokens.pushBack({ spa::TOKEN_NAME, "z" });
	//tokens.pushBack({ spa::TOKEN_NAME, "+" });
	//tokens.pushBack({ spa::TOKEN_INTEGER, "1" });
	//tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
	//tokens.pushBack({ spa::TOKEN_CLOSE_BRACES, "}" });

	//// procedure {}
	//tokens.pushBack({ spa::TOKEN_PROCEDURE, "procedure" });
	//tokens.pushBack({ spa::TOKEN_OPEN_BRACES, "{" });
	//tokens.pushBack({ spa::TOKEN_CLOSE_BRACES, "}" });

	//// procedure {
	//tokens.pushBack({ spa::TOKEN_PROCEDURE, "procedure" });
	//tokens.pushBack({ spa::TOKEN_OPEN_BRACES, "{" });


	//// procedure {;}
	//tokens.pushBack({ spa::TOKEN_PROCEDURE, "procedure" });
	//tokens.pushBack({ spa::TOKEN_OPEN_BRACES, "{" });
	//tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
	//tokens.pushBack({ spa::TOKEN_CLOSE_BRACES, "}" });

	//// procedure }
	//tokens.pushBack({ spa::TOKEN_PROCEDURE, "procedure" });
	//tokens.pushBack({ spa::TOKEN_OPEN_BRACES, "{" });
	//tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
	//tokens.pushBack({ spa::TOKEN_CLOSE_BRACES, "}" });

	//// procedure { x = \" 8 \"}   ???
	//tokens.pushBack({ spa::TOKEN_PROCEDURE, "procedure" });
	//tokens.pushBack({ spa::TOKEN_OPEN_BRACES, "{" });
	//tokens.pushBack({ spa::TOKEN_NAME, "x" });
	//tokens.pushBack({ spa::TOKEN_EQUAL, "=" });
	//tokens.pushBack({ spa::TOKEN_DOUBLE_QUOTES, "\"" });
	//tokens.pushBack({ spa::TOKEN_INTEGER, "6" });
	//tokens.pushBack({ spa::TOKEN_DOUBLE_QUOTES, "\"" });
	//tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
	//tokens.pushBack({ spa::TOKEN_CLOSE_BRACES, "}" });

	//// procedure { x + y = 9;}
	//tokens.pushBack({ spa::TOKEN_PROCEDURE, "procedure" });
	//tokens.pushBack({ spa::TOKEN_OPEN_BRACES, "{" });
	//tokens.pushBack({ spa::TOKEN_NAME, "x" });
	//tokens.pushBack({ spa::TOKEN_PLUS, "+" });
	//tokens.pushBack({ spa::TOKEN_NAME, "y" });
	//tokens.pushBack({ spa::TOKEN_EQUAL, "=" });
	//tokens.pushBack({ spa::TOKEN_INTEGER, "9" });
	//tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
	//tokens.pushBack({ spa::TOKEN_CLOSE_BRACES, "}" });


	//// procedure { x + y % 9;}
	//tokens.pushBack({ spa::TOKEN_PROCEDURE, "procedure" });
	//tokens.pushBack({ spa::TOKEN_OPEN_BRACES, "{" });
	//tokens.pushBack({ spa::TOKEN_NAME, "x" });
	//tokens.pushBack({ spa::TOKEN_PLUS, "+" });
	//tokens.pushBack({ spa::TOKEN_NAME, "y" });
	//tokens.pushBack({ spa::TOKEN_MODULO, "%" });
	//tokens.pushBack({ spa::TOKEN_INTEGER, "9" });
	//tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
	//tokens.pushBack({ spa::TOKEN_CLOSE_BRACES, "}" });

	//// procedure { x + y = z / 9;}
	//tokens.pushBack({ spa::TOKEN_PROCEDURE, "procedure" });
	//tokens.pushBack({ spa::TOKEN_OPEN_BRACES, "{" });
	//tokens.pushBack({ spa::TOKEN_NAME, "x" });
	//tokens.pushBack({ spa::TOKEN_PLUS, "+" });
	//tokens.pushBack({ spa::TOKEN_NAME, "y" });
	//tokens.pushBack({ spa::TOKEN_EQUAL, "=" });
	//tokens.pushBack({ spa::TOKEN_NAME, "z" });
	//tokens.pushBack({ spa::TOKEN_DIVIDE, "/" });
	//tokens.pushBack({ spa::TOKEN_INTEGER, "9" });
	//tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
	//tokens.pushBack({ spa::TOKEN_CLOSE_BRACES, "}" });

	//// procedure { xy = 9 / ;}
	//tokens.pushBack({ spa::TOKEN_PROCEDURE, "procedure" });
	//tokens.pushBack({ spa::TOKEN_OPEN_BRACES, "{" });
	//tokens.pushBack({ spa::TOKEN_NAME, "x" });
	//tokens.pushBack({ spa::TOKEN_NAME, "y" });
	//tokens.pushBack({ spa::TOKEN_EQUAL, "=" });
	//tokens.pushBack({ spa::TOKEN_NAME, "9" });
	//tokens.pushBack({ spa::TOKEN_DIVIDE, "/" });
	//tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
	//tokens.pushBack({ spa::TOKEN_CLOSE_BRACES, "}" });

	//// proc {}

	//// proc{ ; }


