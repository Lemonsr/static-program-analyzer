#include "PqlSemanticChecker.h"

#include <optional>

bool spa::PqlSemanticChecker::isSemanticallyValid(ParsedQuery parsedQuery) {
  std::optional<SuchThatClause> suchThatClause = parsedQuery.getSuchThatClause();
  if (suchThatClause) {
    bool isSuchThatClauseValid = isValid(suchThatClause.value());
    if (!isSuchThatClauseValid) {
      return false;
    }
  }

  std::optional<PatternClause> patternClause = parsedQuery.getPatternClause();
  if (patternClause) {
    bool isPatternClauseValid = isValid(patternClause.value());
    if (!isPatternClauseValid) {
      return false;
    }
  }

  return true;
}

bool spa::PqlSemanticChecker::isValid(SuchThatClause suchThatClause) {
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
  }
  return false;
}

bool spa::PqlSemanticChecker::isValid(PatternClause patternClause) {
  PqlArgument firstArg = patternClause.getFirstArg();
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

  return true;
}

bool spa::PqlSemanticChecker::checkModifiesArguments(PqlArgument firstArg, PqlArgument secondArg) {
  ArgumentType firstArgType = firstArg.getType();
  if (firstArgType == WILDCARD || firstArgType == VARIABLE_NAME) {
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

bool spa::PqlSemanticChecker::checkUsesArguments(PqlArgument firstArg, PqlArgument secondArg) {
  ArgumentType firstArgType = firstArg.getType();
  if (firstArgType == WILDCARD || firstArgType == VARIABLE_NAME) {
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

bool spa::PqlSemanticChecker::checkParentArguments(PqlArgument firstArg, PqlArgument secondArg) {
  for (PqlArgument arg : { firstArg, secondArg }) {
    ArgumentType argType = arg.getType();
    if (argType == WILDCARD || argType == VARIABLE_NAME) {
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

bool spa::PqlSemanticChecker::checkFollowsArguments(PqlArgument firstArg, PqlArgument secondArg) {
  for (PqlArgument arg : { firstArg, secondArg }) {
    ArgumentType argType = arg.getType();
    if (argType == WILDCARD || argType == VARIABLE_NAME) {
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
