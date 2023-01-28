#pragma once
#include <vector>
#include "Statement.h"

class ContainerStatement : public Statement
{
protected:
	std::vector<Statement*> statementList;
public:
	std::vector<Statement*> getStatementList();
};

class IfContainerStatement : public ContainerStatement
{
public:
	IfContainerStatement(std::string parentProcedureVal, int statementLineNum, std::vector<Statement*> statementList);
};

class WhileContainerStatement : public ContainerStatement
{
public:
	WhileContainerStatement(std::string parentProcedureVal, int statementLineNum, std::vector<Statement*> statementList);
};

class InnerBlockStatement : public ContainerStatement
{
public:
	InnerBlockStatement(std::string parentProcedureVal, int statementLineNum, std::vector<Statement*> statementList);
};

