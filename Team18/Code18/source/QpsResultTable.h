#pragma once

#include <optional>
#include <string>
#include <vector>
#include <utility>
#include <unordered_set>

#include "PqlArgument.h"
#include "QpsValue.h"

namespace spa {

class QpsResultTable {
 private:
  std::vector<std::string> headers;
  std::vector<std::vector<QpsValue>> rows;
  std::optional<std::vector<QpsValue>> innerJoinRow(std::vector<std::string>& resultHeaders,
                                                    std::vector<QpsValue>& row,
                                                    std::vector<QpsValue>& other);
 public:
  QpsResultTable() = default;
  QpsResultTable(const std::vector<std::string>& headers,
                 const std::vector<std::vector<QpsValue>>& rows);
  void addHeader(std::string header);
  void addHeader(PqlArgument header);
  std::pair<int, int> getDimension();
  void addRow(std::vector<QpsValue> row);
  std::unordered_set<QpsValue, QpsValueHash, QpsValueEquality> getColumn(std::string header);
  QpsResultTable innerJoin(QpsResultTable& other);
  std::vector<std::vector<QpsValue>> getRows();
};
}  // namespace spa
