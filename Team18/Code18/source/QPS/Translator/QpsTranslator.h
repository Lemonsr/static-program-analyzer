#pragma once

#include <list>
#include <string>

#include "QpsResultTable.h"

namespace spa {
class QpsTranslator {
 private:
  QpsResultTable& result;
 public:
  explicit QpsTranslator(QpsResultTable& result);
  std::list<std::string> translate(std::string selectSynonym);
};
}  // namespace spa
