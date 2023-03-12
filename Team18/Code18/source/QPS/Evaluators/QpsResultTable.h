#pragma once

#include <optional>
#include <string>
#include <vector>
#include <utility>
#include <unordered_map>
#include <unordered_set>

#include "PqlArgument.h"
#include "QpsValue.h"

namespace spa {

using QpsResultRow = std::vector<QpsValue>;

using QpsValueRowsMap = std::unordered_map<QpsValue, std::vector<QpsResultRow*>,
                                           QpsValueHash, QpsValueEquality>;

using QpsResultRowPair = std::pair<QpsResultRow*, QpsResultRow*>;

struct QpsResultRowPairHash {
  size_t operator()(const QpsResultRowPair& p) const;
};

struct QpsResultRowPairEquality {
  bool operator()(const QpsResultRowPair& p1, const QpsResultRowPair& p2) const;
};

using QpsResultRowPairMatchMap = std::unordered_map<QpsResultRowPair, int,
                                                    QpsResultRowPairHash, QpsResultRowPairEquality>;

using QpsValueSet = std::unordered_set<QpsValue, QpsValueHash, QpsValueEquality>;

class QpsResultTable {
 private:
  std::unordered_map<std::string, std::vector<int>> headerIndexMap;
  std::vector<std::string> headers;
  std::vector<QpsResultRow> rows;
  std::vector<std::pair<int, int>> getCommonColumnIndices(QpsResultTable& table,
                                                          QpsResultTable& other);
  QpsResultRow concatenateRow(QpsResultRow& row, QpsResultRow& other);
  void cartesianProduct(QpsResultTable& result, QpsResultTable& other);
  QpsValueRowsMap getValueRowsMap(QpsResultTable& table, int index);
  void fillQpsResultRowPairMatchMap(QpsResultRowPairMatchMap& result,
                                    QpsValueRowsMap& cMap,
                                    QpsValueRowsMap& oMap);
 public:
  void addHeader(const std::string& header);
  void addHeader(PqlArgument header);
  std::pair<int, int> getDimension();
  void addRow(const QpsResultRow& row);
  QpsValueSet getColumn(std::string header);
  std::vector<QpsResultRow> getColumns(std::vector<std::string> headers);
  QpsResultTable innerJoin(QpsResultTable& other);
  const std::vector<QpsResultRow>& getRows();
  bool isEmpty();
};
}  // namespace spa
