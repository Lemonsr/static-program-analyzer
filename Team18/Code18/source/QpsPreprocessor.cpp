#include <vector>
#include <sstream>

#include "QpsPreprocessor.h"
#include "Tokenizer.h"
#include "Token.h"
#include "Stream.h"

spa::ParsedQuery spa::QpsPreprocessor::preprocess(std::string query) {
  std::stringstream ss;
  ss.str(query);
  Tokenizer* tokenizer = new Tokenizer();
  Stream<Token> tokens = tokenizer->tokenize(ss);

  PqlParser* parser = new PqlParser(tokens);
  ParsedQuery queryObject = parser->parse();

  return queryObject;
}
