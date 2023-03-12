#include "QpsResultTable.h"

#include <stdexcept>
#include <string>
#include <unordered_set>
#include <unordered_map>

#include "HashTuple.h"

std::pair<std::vector<int>, std::vector<int>> spa::QpsResultTable::getCommonColumnIndices(QpsResultTable& table,
                                                                                          QpsResultTable& other) {
  std::vector<int> tIndices;
  std::vector<int> oIndices;
  auto& tMap = table.headerIndexMap;
  auto& oMap = other.headerIndexMap;
  for (auto it = tMap.begin(); it != tMap.end(); ++it) {
    auto otherIt = oMap.find(it->first);
    if (otherIt == oMap.end()) {
      continue;
    }
    tIndices.push_back((it->second)[0]);
    oIndices.push_back((otherIt->second)[0]);
  }
  return { tIndices, oIndices };
}

spa::QpsResultRow spa::QpsResultTable::concatenateRow(QpsResultRow& row, QpsResultRow& other) {
  spa::QpsResultRow result;
  for (auto& v : row) {
    result.push_back(v);
  }
  for (auto& v : other) {
    result.push_back(v);
  }
  return result;
}

void spa::QpsResultTable::addHeader(const std::string& header) {
  if (!header.empty()) {
    headerIndexMap[header].push_back(headers.size());
  }
  headers.push_back(header);
}

void spa::QpsResultTable::addHeader(PqlArgument arg) {
  if (arg.getType() != SYNONYM) {
    return headers.push_back("");
  }
  addHeader(arg.getValue());
}

std::pair<int, int> spa::QpsResultTable::getDimension() {
  return { headers.size(), rows.size() };
}

void spa::QpsResultTable::addRow(const QpsResultRow& row) {
  if (row.size() != headers.size()) {
    throw std::runtime_error("QpsResultTable addRow: row size not equal to columns");
  }
  rows.push_back(row);
}

spa::QpsValueSet spa::QpsResultTable::getColumn(std::string header) {
  QpsValueSet result;
  auto it = headerIndexMap.find(header);
  if (it == headerIndexMap.end()) {
    return {};
  }
  int index = it->second[0];
  for (auto& row : rows) {
    result.insert(row[index]);
  }
  return result;
}

std::vector<spa::QpsResultRow> spa::QpsResultTable::getColumns(std::vector<std::string> headers) {
  std::vector<int> indices;
  for (auto& header : headers) {
    auto it = headerIndexMap.find(header);
    if (it == headerIndexMap.end()) {
      throw std::runtime_error("Header doesn't exist");
    }
    indices.push_back(it->second[0]);
  }
  std::vector<spa::QpsResultRow> result;
  for (auto& row : rows) {
    spa::QpsResultRow temp;
    for (int index : indices) {
      temp.push_back(row[index]);
    }
    result.push_back(temp);
  }
  return result;
}

void spa::QpsResultTable::cartesianProduct(QpsResultTable& result, QpsResultTable& other) {
  for (auto& row : rows) {
    for (auto& oRow : other.rows) {
      result.addRow(concatenateRow(row, oRow));
    }
  }
}

size_t spa::QpsFilteredRowHash::operator()(const QpsFilteredRow& r) const {
  size_t result = 0;
  QpsValueHash valHash;
  for (int i : r.selectedColumns) {
    spa::hash_combine<size_t>(result, valHash(r.row[i]));
  }
  return result;
}

bool spa::QpsFilteredRowEquality::operator()(const QpsFilteredRow& first, const QpsFilteredRow& second) const {
  auto& firstColumns = first.selectedColumns;
  auto& secondColumns = second.selectedColumns;
  if (firstColumns.size() != secondColumns.size()) {
    return false;
  }
  for (size_t i = 0; i < firstColumns.size(); ++i) {
    if (first.row[firstColumns[i]] != second.row[secondColumns[i]]) {
      return false;
    }
  }
  return true;
}

void spa::QpsResultTable::fillQpsRowMap(QpsRowMap& m, QpsResultTable& table, std::vector<int>& selectedColumns) {
  for (auto& row : table.rows) {
    m[{selectedColumns, row}].push_back(&row);
  }
}

spa::QpsResultTable spa::QpsResultTable::innerJoin(QpsResultTable& other) {
  QpsResultTable resultTable;
  for (auto& s : headers) {
    resultTable.addHeader(s);
  }
  for (auto& s : other.headers) {
    resultTable.addHeader(s);
  }
  auto commonIndices = getCommonColumnIndices(*this, other);
  if (commonIndices.first.empty()) {
    cartesianProduct(resultTable, other);
    return resultTable;
  }
  QpsRowMap currMap;
  fillQpsRowMap(currMap, *this, commonIndices.first);
  QpsRowMap otherMap;
  fillQpsRowMap(otherMap, other, commonIndices.second);
  for (auto& currP : currMap) {
    auto it = otherMap.find(currP.first);
    if (it == otherMap.end()) {
      continue;
    }
    for (auto rowPtr : currP.second) {
      for (auto otherRowPtr : it->second) {
        resultTable.addRow(concatenateRow(*rowPtr, *otherRowPtr));
      }
    }
  }
  return resultTable;
}

const std::vector<spa::QpsResultRow>& spa::QpsResultTable::getRows() {
  return rows;
}

bool spa::QpsResultTable::isEmpty() {
  return rows.size() == 0;
}

spa::QpsResultTable spa::QpsResultTable::matchColumns(const std::string& first,
                                                      const std::string& second) {
  int firstIndex = headerIndexMap[first][0];
  int secondIndex = headerIndexMap[second][0];
  QpsResultTable result;
  for (auto& header : headers) {
    result.addHeader(header);
  }
  for (auto& row : rows) {
    if (row[firstIndex] == row[secondIndex]) {
      result.addRow(row);
    }
  }
  return result;
}
