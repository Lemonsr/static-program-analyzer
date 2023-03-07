#include "PqlSemanticChecker.h"

#include <optional>

bool spa::PqlSemanticChecker::isSemanticallyValid(ParsedQuery& parsedQuery) {
  for (auto& p : parsedQuery.getDeclarationsCount()) {
    if (p.second > 1) {
      return false;
    }
  }
  for (auto& p : parsedQuery.getUsedDeclarations()) {
    if (p.second == UNKNOWN) {
      return false;
    }
  }
  for (auto& clause : parsedQuery.getSuchThatClauses()) {
    if (!isValid(clause)) {
      return false;
    }
  }
  for (auto& clause : parsedQuery.getPatternClauses()) {
    if (!isValid(clause)) {
      return false;
    }
  }
  for (auto& clause : parsedQuery.getWithClauses()) {
    if (!isValid(clause)) {
      return false;
    }
  }
  return true;
}

bool spa::PqlSemanticChecker::isValid(SuchThatClause& suchThatClause) {
  RelationshipType designAbstraction = suchThatClause.getDesignAbstraction();
  PqlArgument firstArg = suchThatClause.getFirstArg();
  PqlArgument secondArg = suchThatClause.getSecondArg();
  switch (designAbstraction) {
  case MODIFIES:
    return checkModifiesArguments(firstArg, secondArg);
  case USES:
    return checkUsesArguments(firstArg, secondArg);
  case PARENT:
  case PARENT_STAR:
    return checkParentArguments(firstArg, secondArg);
  case FOLLOWS:
  case FOLLOWS_STAR:
    return checkFollowsArguments(firstArg, secondArg);
  case CALLS:
  case CALLS_STAR:
    return checkCallsArguments(firstArg, secondArg);
  case NEXT:
    return checkNextArguments(firstArg, secondArg);
  }
  return false;
}

bool spa::PqlSemanticChecker::isValid(PatternClause& patternClause) {
  PqlArgument& firstArg = patternClause.getFirstArg();
  ArgumentType firstArgType = firstArg.getType();
  if (firstArgType == LINE_NO) {
    return false;
  }

  if (firstArgType == SYNONYM) {
    DesignEntityType deType = firstArg.getDesignEntity().value();
    if (deType != VARIABLE) {
      return false;
    }
  }

  DesignEntityType entityType = patternClause.getSynonymType();
  switch (entityType) {
    case ASSIGN:
      if (patternClause.getNumArgs() != 2) {
        return false;
      }
      break;
    case IF:
      if (patternClause.getNumArgs() != 3 || patternClause.getPatternType() != ANY) {
        return false;
      }
      break;
    case WHILE:
      if (patternClause.getNumArgs() != 2 || patternClause.getPatternType() != ANY) {
        return false;
      }
      break;
    default:
      return false;
  }
  return true;
}

bool spa::PqlSemanticChecker::isValid(WithClause& withClause) {
  WithArgument firstArg = withClause.getFirstArg();
  WithArgument secondArg = withClause.getSecondArg();
  QpsValueType firstArgType = getWithArgumentType(firstArg);
  QpsValueType secondArgType = getWithArgumentType(secondArg);

  return firstArgType == secondArgType;
}

bool spa::PqlSemanticChecker::checkModifiesArguments(PqlArgument& firstArg, PqlArgument& secondArg) {
  ArgumentType firstArgType = firstArg.getType();
  if (firstArgType == WILDCARD) {
    return false;
  }

  if (firstArgType == SYNONYM) {
    DesignEntityType deType = firstArg.getDesignEntity().value();
    if (deType == VARIABLE || deType == PRINT || deType == CONSTANT) {
      return false;
    }
  }

  ArgumentType secondArgType = secondArg.getType();
  if (secondArgType == LINE_NO) {
    return false;
  }

  if (secondArgType == SYNONYM && secondArg.getDesignEntity().value() != VARIABLE) {
    return false;
  }

  return true;
}

bool spa::PqlSemanticChecker::checkUsesArguments(PqlArgument& firstArg, PqlArgument& secondArg) {
  ArgumentType firstArgType = firstArg.getType();
  if (firstArgType == WILDCARD) {
    return false;
  }
  if (firstArgType == SYNONYM) {
    DesignEntityType deType = firstArg.getDesignEntity().value();
    if (deType == VARIABLE || deType == READ || deType == CONSTANT) {
      return false;
    }
  }

  ArgumentType secondArgType = secondArg.getType();
  if (secondArgType == LINE_NO) {
    return false;
  }

  if (secondArgType == SYNONYM && secondArg.getDesignEntity().value() != VARIABLE) {
    return false;
  }

  return true;
}

bool spa::PqlSemanticChecker::checkParentArguments(PqlArgument& firstArg, PqlArgument& secondArg) {
  for (PqlArgument arg : { firstArg, secondArg }) {
    ArgumentType argType = arg.getType();
    if (argType == LITERAL_STRING) {
      return false;
    }

    if (argType == SYNONYM) {
      DesignEntityType deType = arg.getDesignEntity().value();
      if (deType == PROCEDURE || deType == VARIABLE || deType == CONSTANT) {
        return false;
      }
    }
  }

  return true;
}

bool spa::PqlSemanticChecker::checkFollowsArguments(PqlArgument& firstArg, PqlArgument& secondArg) {
  for (PqlArgument arg : { firstArg, secondArg }) {
    ArgumentType argType = arg.getType();
    if (argType == LITERAL_STRING) {
      return false;
    }

    if (argType == SYNONYM) {
      DesignEntityType deType = arg.getDesignEntity().value();
      if (deType == PROCEDURE || deType == VARIABLE || deType == CONSTANT) {
        return false;
      }
    }
  }

  return true;
}

bool spa::PqlSemanticChecker::checkCallsArguments(PqlArgument& firstArg, PqlArgument& secondArg) {
  for (PqlArgument arg : { firstArg, secondArg }) {
    ArgumentType argType = arg.getType();
    if (argType == LINE_NO) {
      return false;
    }

    if (argType == SYNONYM) {
      DesignEntityType deType = arg.getDesignEntity().value();
      if (deType != PROCEDURE) {
        return false;
      }
    }
  }

  return true;
}

bool spa::PqlSemanticChecker::checkNextArguments(PqlArgument& firstArg, PqlArgument& secondArg) {
  for (PqlArgument arg : { firstArg, secondArg }) {
    ArgumentType argType = arg.getType();
    if (argType == LITERAL_STRING) {
      return false;
    }

    if (argType == SYNONYM) {
      DesignEntityType deType = arg.getDesignEntity().value();
      if (deType == PROCEDURE || deType == VARIABLE || deType == CONSTANT) {
        return false;
      }
    }
  }

  return true;
}

spa::QpsValueType spa::PqlSemanticChecker::getWithArgumentType(WithArgument& withArgument) {
  std::unordered_map<std::string, QpsValueType> attributeNameTypeMap = {
    {"stmt#", QpsValueType::INTEGER},
    {"value", QpsValueType::INTEGER},
    {"procName", QpsValueType::STRING},
    {"varName", QpsValueType::STRING},
  };

  if (withArgument.getType() == WithArgumentType::WITH_VALUE) {
    return withArgument.getValue().getType();
  }

  std::string attribute = withArgument.getAttribute();
  return attributeNameTypeMap[attribute.substr(attribute.find('.') + 1)];
}
