#include "QpsResultTable.h"

#include <stdexcept>
#include <string>
#include <unordered_set>
#include <unordered_map>

#include "HashTuple.h"

std::vector<std::pair<int, int>> spa::QpsResultTable::getCommonColumnIndices(QpsResultTable& table,
                                                                             QpsResultTable& other) {
  std::vector<std::pair<int, int>> result;
  auto& tMap = table.headerIndexMap;
  auto& oMap = other.headerIndexMap;
  for (auto it = tMap.begin(); it != tMap.end(); ++it) {
    auto otherIt = oMap.find(it->first);
    if (otherIt == oMap.end()) {
      continue;
    }
    result.push_back({ it->second[0], otherIt->second[0] });
  }
  return result;
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

void spa::QpsResultTable::addHeader(std::string header) {
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

void spa::QpsResultTable::addRow(QpsResultRow row) {
  if (row.size() != headers.size()) {
    throw std::runtime_error("QpsResultTable addRow: row size not equal to columns");
  }
  rows.push_back(row);
}

spa::QpsValueSet spa::QpsResultTable::getColumn(std::string header) {
  QpsValueSet result;
  int index = headerIndexMap[header][0];
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

spa::QpsValueRowsMap spa::QpsResultTable::getValueRowsMap(QpsResultTable& table,
                                                          int index) {
  QpsValueRowsMap result;
  for (auto& row : table.rows) {
    result[row[index]].push_back(&row);
  }
  return result;
}

size_t spa::QpsResultRowPairHash::operator()(const QpsResultRowPair& p) const {
  size_t result = 0;
  spa::hash_combine<QpsResultRow*>(result, p.first);
  spa::hash_combine<QpsResultRow*>(result, p.second);
  return result;
}

bool spa::QpsResultRowPairEquality::operator()(const QpsResultRowPair& p1,
                                               const QpsResultRowPair& p2) const {
  return p1.first == p2.first && p1.second == p2.second;
}

void spa::QpsResultTable::fillQpsResultRowPairMatchMap(QpsResultRowPairMatchMap& result,
                                                       QpsValueRowsMap& cMap,
                                                       QpsValueRowsMap& oMap) {
  for (auto it = cMap.begin(); it != cMap.end(); ++it) {
    auto oIt = oMap.find(it->first);
    if (oIt == oMap.end()) {
      continue;
    }
    for (auto row : it->second) {
      for (auto oRow : oIt->second) {
        ++result[{row, oRow}];
      }
    }
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
  std::vector<std::pair<int, int>> commonIndices = getCommonColumnIndices(*this, other);
  if (commonIndices.empty()) {
    cartesianProduct(resultTable, other);
    return resultTable;
  }
  QpsResultRowPairMatchMap matchMap;
  for (auto& p : commonIndices) {
    auto tMap = getValueRowsMap(*this, p.first);
    auto oMap = getValueRowsMap(other, p.second);
    fillQpsResultRowPairMatchMap(matchMap, tMap, oMap);
  }
  for (auto it = matchMap.begin(); it != matchMap.end(); ++it) {
    if (it->second != commonIndices.size()) {
      continue;
    }
    auto& p = it->first;
    resultTable.addRow(concatenateRow(*p.first, *p.second));
  }
  return resultTable;
}

const std::vector<spa::QpsResultRow>& spa::QpsResultTable::getRows() {
  return rows;
}
