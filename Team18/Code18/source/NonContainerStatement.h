#pragma once
#include <unordered_set>

#include "Expr.h"
#include "Statement.h"
#include "Token.h"

class NonContainerStatement : public Statement
{
protected:
	std::unordered_set<int> whileStmtAncestors;
	std::unordered_set<int> ifStmtAncestors;
};

class OneVarNonContainerStatement : public NonContainerStatement
{
protected:
	std::string variableName;
public:
	std::string getVariableName();
};

class ReadStatement : public OneVarNonContainerStatement
{
public:
	ReadStatement(std::string parentProcedureVal, std::string variableName, std::unordered_set<int> whileStmtAncestors, std::unordered_set<int> ifStmtAncestors, int statementLineNum);
};

class PrintStatement : public OneVarNonContainerStatement
{
public:
	PrintStatement(std::string parentProcedureVal, std::string variableName, std::unordered_set<int> whileStmtAncestors, std::unordered_set<int> ifStmtAncestors, int statementLineNum);
};

class CallStatement : public OneVarNonContainerStatement
{
public:
	CallStatement(std::string parentProcedureVal, std::string variableName, std::unordered_set<int> whileStmtAncestors, std::unordered_set<int> ifStmtAncestors, int statementLineNum);
};

class MultiVarNonContainerStatement : public NonContainerStatement
{
protected:
	std::string assignVar;
	std::string rightPostfixExpr;
	Expr* expr;
};

class AssignStatement : public MultiVarNonContainerStatement
{
public:
	AssignStatement(std::string parentProcedureVal, Expr* expr, std::unordered_set<int> whileStmtAncestors, std::unordered_set<int> ifStmtAncestors, int statementLineNum);
};

class ifConditionStatement : public MultiVarNonContainerStatement
{
public:
	ifConditionStatement(std::string parentProcedureVal, Expr* expr, std::unordered_set<int> whileStmtAncestors, std::unordered_set<int> ifStmtAncestors, int statementLineNum);
};

class whileConditionStatement : public MultiVarNonContainerStatement
{
public:
	whileConditionStatement(std::string parentProcedureVal, Expr* expr, std::unordered_set<int> whileStmtAncestors, std::unordered_set<int> ifStmtAncestors, int statementLineNum);
};

