#include <iostream>
#include <string>

#include "PqlTokenizer.h"

PqlTokenizer::PqlTokenizer(std::string query): query(query) {
}

std::vector<PqlToken> PqlTokenizer::tokenize() {
  std::cout << "tokenize" << std::endl;
  return {};
}
