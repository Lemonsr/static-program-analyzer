#include "PKBQueryArg.h"
#include "PKBQueryTypes.h"
#include "PqlArgument.h"

#include <optional>
#include <utility>
#include <string>
#include <unordered_map>

std::unordered_map<spa::DesignEntityType, spa::PKBQueryArgType> queryArgMap {
  { spa::STMT, spa::PKBQueryArgType::STATEMENT },
  { spa::READ, spa::PKBQueryArgType::READ },
  { spa::PRINT, spa::PKBQueryArgType::PRINT },
  { spa::ASSIGN, spa::PKBQueryArgType::ASSIGN },
  { spa::CALL, spa::PKBQueryArgType::CALL },
  { spa::WHILE, spa::PKBQueryArgType::WHILE },
  { spa::IF, spa::PKBQueryArgType::IF }
};

spa::PKBQueryArg::PKBQueryArg(PqlArgument& pqlArg) {
  switch (pqlArg.getType()) {
  case SYNONYM: {
    auto designEntity = pqlArg.getDesignEntity().value();
    if (designEntity == PROCEDURE) {
      type = PKBQueryArgType::PROCEDURE;
      procedureOpt = {};
    } else if (designEntity == VARIABLE) {
      type = PKBQueryArgType::VARIABLE;
      variableOpt = {};
    } else if (designEntity == CONSTANT) {
      type = PKBQueryArgType::CONSTANT;
      constantOpt = {};
    } else {
      type = queryArgMap[designEntity];
      statementOpt = { statementTypeMap[designEntity] };
    }
    break;
  }
  case WILDCARD: {
    type = PKBQueryArgType::UNDERSCORE;
    underscoreOpt = {};
    break;
  }
  case VARIABLE_NAME: {
    type = PKBQueryArgType::NAME;
    nameOpt = { pqlArg.getValue() };
    break;
  }
  case LINE_NO: {
    type = PKBQueryArgType::LINE_NUMBER;
    lineNumberOpt = { static_cast<size_t>(std::stoi(pqlArg.getValue())) };
  }
  }
}

const spa::PKBQueryArgType& spa::PKBQueryArg::getType() {
  return type;
}

const spa::Constant& spa::PKBQueryArg::getConstant() {
  return constantOpt.value();
}

const spa::Variable& spa::PKBQueryArg::getVariable() {
  return variableOpt.value();
}

const spa::Statement& spa::PKBQueryArg::getStatement() {
  return statementOpt.value();
}

const spa::Procedure& spa::PKBQueryArg::getProcedure() {
  return procedureOpt.value();
}

const spa::LineNumber& spa::PKBQueryArg::getLineNumber() {
  return lineNumberOpt.value();
}

const spa::Name& spa::PKBQueryArg::getName() {
  return nameOpt.value();
}

const spa::Underscore& spa::PKBQueryArg::getUnderscore() {
  return underscoreOpt.value();
}
