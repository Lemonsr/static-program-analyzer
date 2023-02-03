#include "Procedure.h"

//Constructor
spa::Procedure::Procedure(spa::Token token,
                          std::vector<Statement*> statementLst): token(token),
                                                                 statementLst(statementLst) {}

spa::Token spa::Procedure::getToken() {
  return token;
}

std::vector<spa::Statement*> spa::Procedure::getStatementLst() {
  return statementLst;
}
