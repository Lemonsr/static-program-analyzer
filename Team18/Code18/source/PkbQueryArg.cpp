#include "PkbQueryArg.h"

#include <optional>
#include <utility>
#include <string>
#include <unordered_map>

#include "PqlArgument.h"

std::unordered_map<spa::DesignEntityType, spa::PkbQueryArgType> queryArgMap{
  { spa::STMT, spa::PkbQueryArgType::STATEMENT },
  { spa::READ, spa::PkbQueryArgType::READ },
  { spa::PRINT, spa::PkbQueryArgType::PRINT },
  { spa::ASSIGN, spa::PkbQueryArgType::ASSIGN },
  { spa::CALL, spa::PkbQueryArgType::CALL },
  { spa::WHILE, spa::PkbQueryArgType::WHILE },
  { spa::IF, spa::PkbQueryArgType::IF }
};

std::unordered_map<spa::DesignEntityType,
                   std::optional<spa::StatementType>> statementTypeMap {
  { spa::STMT, {} },
  { spa::READ, {spa::StatementType::READ} },
  { spa::PRINT, {spa::StatementType::PRINT} },
  { spa::ASSIGN, {spa::StatementType::ASSIGN} },
  { spa::CALL, {spa::StatementType::CALL} },
  { spa::WHILE, {spa::StatementType::WHILE} },
  { spa::IF, {spa::StatementType::IF} }
};

spa::PkbQueryArg::PkbQueryArg(PqlArgument& pqlArg) {
  switch (pqlArg.getType()) {
  case SYNONYM: {
    auto designEntity = pqlArg.getDesignEntity().value();
    if (designEntity == PROCEDURE) {
      type = PkbQueryArgType::PROCEDURE;
      procedureOpt = {};
    } else if (designEntity == VARIABLE) {
      type = PkbQueryArgType::VARIABLE;
      variableOpt = {};
    } else if (designEntity == CONSTANT) {
      type = PkbQueryArgType::CONSTANT;
      constantOpt = {};
    } else {
      type = queryArgMap[designEntity];
      statementOpt = { statementTypeMap[designEntity] };
    }
    break;
  }
  case WILDCARD: {
    type = PkbQueryArgType::UNDERSCORE;
    underscoreOpt = {};
    break;
  }
  case VARIABLE_NAME: {
    type = PkbQueryArgType::NAME;
    nameOpt = { pqlArg.getValue() };
    break;
  }
  case LINE_NO: {
    type = PkbQueryArgType::LINE_NUMBER;
    lineNumberOpt = { static_cast<size_t>(std::stoi(pqlArg.getValue())) };
  }
  }
}

const spa::PkbQueryArgType& spa::PkbQueryArg::getType() {
  return type;
}

const spa::Constant& spa::PkbQueryArg::getConstant() {
  return constantOpt.value();
}

const spa::Variable& spa::PkbQueryArg::getVariable() {
  return variableOpt.value();
}

const spa::Statement& spa::PkbQueryArg::getStatement() {
  return statementOpt.value();
}

const spa::Procedure& spa::PkbQueryArg::getProcedure() {
  return procedureOpt.value();
}

const spa::LineNumber& spa::PkbQueryArg::getLineNumber() {
  return lineNumberOpt.value();
}

const spa::Name& spa::PkbQueryArg::getName() {
  return nameOpt.value();
}

const spa::Underscore& spa::PkbQueryArg::getUnderscore() {
  return underscoreOpt.value();
}
