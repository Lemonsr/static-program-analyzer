#pragma once

#include <list>
#include <string>
#include <optional>

namespace spa {
class QpsResult {
 private:
  std::list<std::string> results;
  std::optional<std::string> errorMessage;
 public:
  const std::list<std::string>& getResults() const;
  const std::optional<std::string>& getErrorMessage() const;
  void setResults(std::list<std::string>& results);
  void setErrorMessage(std::string errorMessage);
};
}  // namespace spa
