#include "QpsTranslator.h"

#include <unordered_set>

spa::QpsTranslator::QpsTranslator(QpsResultTable& result) : result(result) {
}

std::list<std::string> spa::QpsTranslator::translate(std::string selectSynonym) {
  QpsValueSet values = result.getColumn(selectSynonym);
  std::list<std::string> translatedResult;
  for (const auto& val : values) {
    translatedResult.push_back(val.toString());
  }
  return translatedResult;
}
