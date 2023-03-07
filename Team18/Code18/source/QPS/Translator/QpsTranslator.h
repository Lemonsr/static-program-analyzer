#pragma once

#include <list>
#include <string>

#include "QpsResultTable.h"
#include "ParsedQuery.h"

namespace spa {
class QpsTranslator {
 private:
  QpsResultTable& result;
  std::string rowToString(QpsResultRow row);
 public:
  explicit QpsTranslator(QpsResultTable& result);
  std::list<std::string> translate(SelectClauseType selectType, std::vector<std::string> selectColumns);
};
}  // namespace spa
