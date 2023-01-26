#include <vector>

#include "QpsPreprocessor.h"

QueryObject QpsPreprocessor::preprocess(std::string query) {
  PqlTokenizer* tokenizer = new PqlTokenizer(query);
  std::vector<PqlToken> tokens = tokenizer->tokenize();

  PqlParser* parser = new PqlParser(tokens);
  QueryObject queryObject = parser->parse();

  return queryObject;
}
