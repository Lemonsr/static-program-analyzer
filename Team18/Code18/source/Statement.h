#pragma once
#include <string>

class Statement
{
protected:
	int statementLineNum;
	std::string parentProcedureVal;

public:
	int getStatementLineNum();
	const std::string getParentProcedureVal();
};
