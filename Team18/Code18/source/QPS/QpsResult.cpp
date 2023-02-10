#include "QpsResult.h"

const std::list<std::string>& spa::QpsResult::getResults() const {
  return results;
}

const std::optional<std::string>& spa::QpsResult::getErrorMessage() const {
  return errorMessage;
}

void spa::QpsResult::setResults(std::list<std::string>& results) {
  this->results = results;
}

void spa::QpsResult::setErrorMessage(std::string errorMessage) {
  this->errorMessage = errorMessage;
}
