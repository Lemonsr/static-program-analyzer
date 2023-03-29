#pragma once

#include <string>

// SYNONYMS
const std::string READ_SYNONYM_LITERAL = "re";
const std::string VARIABLE_SYNONYM_LITERAL = "v";
const std::string PRINT_SYNONYM_LITERAL = "pr";
const std::string PROC_SYNONYM_LITERAL = "p";

// ATTRIBUTES
const std::string STMT_NUM_ATTR_LITERAL = "stmt#";
const std::string VALUE_ATTR_LITERAL = "value";
const std::string PROC_NAME_ATTR_LITERAL = "procName";
const std::string VAR_NAME_ATTR_LITERAL = "varName";

// ERROR_TYPES
const std::string SYNTAX_ERROR_LITERAL = "SyntaxError";
const std::string SEMANTIC_ERROR_LITERAL = "SemanticError";

// QUERY LITERALS
const std::string SELECT_LITERAL = "Select";
const std::string BOOLEAN_LITERAL = "BOOLEAN";
const std::string SUCH_LITERAL = "such";
const std::string THAT_LITERAL = "that";
const std::string WITH_LITERAL = "with";
const std::string PATTERN_LITERAL = "pattern";
const std::string AND_LITERAL = "and";

// OEPRATOR_LITERALS
const std::string EMPTY_LITERAL = "";
const std::string UNDERSCORE_LITERAL = "_";
const std::string DOUBLE_QUOTES_LITERAL = "\"";
const std::string HASH_LITERAL = "#";
const std::string FULL_STOP_LITERAL = ".";
const std::string COMMA_LITERAL = ",";
const std::string OPEN_BRACKET_LITERAL = "(";
const std::string CLOSE_BRACKET_LITERAL = ")";
const std::string LESS_THAN_LITERAL = "<";
const std::string MORE_THAN_LITERAL = ">";
const std::string MULTIPLY_LITERAL = "*";
const std::string EQUAL_LITERAL = "=";

// ENTITY LITERALS
const std::string PROCEDURE_LITERAL = "procedure";
const std::string STMT_LITERAL = "stmt";
const std::string READ_LITERAL = "read";
const std::string PRINT_LITERAL = "print";
const std::string ASSIGN_LITERAL = "assign";
const std::string CALL_LITERAL = "call";
const std::string WHILE_LITERAL = "while";
const std::string IF_LITERAL = "if";
const std::string VARIABLE_LITERAL = "variable";
const std::string CONSTANT_LITERAL = "constant";
const std::string THEN_LITERAL = "then";
const std::string ELSE_LITERAL = "else";

// RELATIONSHIP LITERALS
const std::string FOLLOWS_LITERAL = "Follows";
const std::string FOLLOWS_STAR_LITERAL = "Follows*";
const std::string PARENT_LITERAL = "Parent";
const std::string PARENT_STAR_LITERAL = "Parent*";
const std::string MODIFIES_LITERAL = "Modifies";
const std::string USES_LITERAL = "Uses";
const std::string CALLS_LITERAL = "Calls";
const std::string CALLS_STAR_LITERAL = "Calls*";
const std::string NEXT_LITERAL = "Next";
const std::string NEXT_STAR_LITERAL = "Next*";
const std::string AFFECTS_LITERAL = "Affects";
const std::string AFFECTS_STAR_LITERAL = "Affects*";

// CFG LITERALS
const int DUMMY_NODE_VAL = -1;
