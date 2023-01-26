#include <iostream>

#include "PqlParser.h"

PqlParser::PqlParser(std::vector<PqlToken> tokens) : tokens(tokens) {
}

QueryObject PqlParser::parse() {
  std::cout << "parse" << std::endl;
  return QueryObject{};
}
