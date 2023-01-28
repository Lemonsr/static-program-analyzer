#pragma once
#include "Token.h"

class Expr
{
private:
	Expr* leftExpr;
	spa::Token token;
	Expr* rightExpr;

public:
	Expr(Expr* leftExpr, spa::Token token, Expr* rightExpr);

	const Expr* getLeftExpr();
	const Expr* getRightExpr();
	spa::Token getToken();
};
