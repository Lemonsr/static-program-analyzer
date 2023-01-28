#pragma once
#include <vector>

#include "Statement.h"
#include "Token.h"

class Procedure
{
private:
	spa::Token token;
	std::vector<Statement*> statementLst;

public:
	Procedure(spa::Token token, std::vector<Statement*> statementLst);
	spa::Token getToken();
	std::vector<Statement*> getStatementLst();
};


