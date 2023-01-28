#include "Expr.h"

Expr::Expr(Expr* leftExpr, spa::Token token, Expr* rightExpr): leftExpr(leftExpr), token(token), rightExpr(rightExpr) {}

const Expr* Expr::getLeftExpr() {
	return leftExpr;
}

const Expr* Expr::getRightExpr() {
	return rightExpr;
}


spa::Token Expr::getToken() {
	return token;
}
