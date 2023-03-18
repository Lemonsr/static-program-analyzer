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

struct QpsFilteredRow {
  std::vector<int>& selectedColumns;
  QpsResultRow& row;

  QpsResultRow getFilteredRow() const;
};

struct QpsFilteredRowHash {
  size_t operator()(const QpsFilteredRow& r) const;
};

struct QpsFilteredRowEquality {
  bool operator()(const QpsFilteredRow& first, const QpsFilteredRow& second) const;
};

using QpsRowMap = std::unordered_map<QpsFilteredRow, std::vector<QpsResultRow*>,
                                     QpsFilteredRowHash, QpsFilteredRowEquality>;

using QpsRowSet = std::unordered_set<QpsFilteredRow, QpsFilteredRowHash, QpsFilteredRowEquality>;

using QpsValueSet = std::unordered_set<QpsValue, QpsValueHash, QpsValueEquality>;

class QpsResultTable {
 private:
  std::unordered_map<std::string, std::vector<int>> headerIndexMap;
  std::vector<std::string> headers;
  std::vector<QpsResultRow> rows;
  std::pair<std::vector<int>, std::vector<int>> getCommonColumnIndices(QpsResultTable& table,
                                                                       QpsResultTable& other);
  QpsResultRow concatenateRow(QpsResultRow& row, QpsResultRow& other);
  void cartesianProduct(QpsResultTable& result, QpsResultTable& other);
  void fillQpsRowMap(QpsRowMap& m, QpsResultTable& table, std::vector<int>& selectedColumns);
 public:
  std::vector<std::string> getHeaderNames();
  void addHeader(const std::string& header);
  void addHeader(PqlArgument header);
  std::pair<int, int> getDimension();
  void addRow(const QpsResultRow& row);
  QpsValueSet getColumn(std::string header);
  QpsResultTable getColumns(const std::vector<std::string>& headers);
  QpsResultTable innerJoin(QpsResultTable& other);
  const std::vector<QpsResultRow>& getRows();
  bool isEmpty();
  QpsResultTable matchColumns(const std::string& first, const std::string& second);
};
}  // namespace spa
