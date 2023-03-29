#include "QpsTranslator.h"
#include "Literal.h"

#include <unordered_set>
#include <sstream>

spa::QpsTranslator::QpsTranslator(QpsResultTable& result) : result(result) {
}

std::string spa::QpsTranslator::rowToString(QpsResultRow row) {
  std::stringstream ss;
  for (auto& itr = row.begin(); itr != row.end(); itr++) {
    if (itr != row.begin()) {
      ss << SPACE_LITERAL;
    }
    ss << (*itr).toString();
  }
  return ss.str();
}

std::list<std::string> spa::QpsTranslator::translate(SelectClauseType selectType,
                                                     std::vector<std::string> selectColumns) {
  std::list<std::string> translatedResult;
  if (selectType == SelectClauseType::SELECT_BOOLEAN) {
    if (result.isEmpty()) {
      translatedResult.push_back(FALSE_LITERAL);
    } else {
      translatedResult.push_back(TRUE_LITERAL);
    }
    return translatedResult;
  }

  if (result.isEmpty()) {
    return {};
  }
  QpsResultTable selectedTable = result.getColumns(selectColumns);
  for (auto& row : selectedTable.getRows()) {
    translatedResult.push_back(rowToString(row));
  }
  return translatedResult;
}
