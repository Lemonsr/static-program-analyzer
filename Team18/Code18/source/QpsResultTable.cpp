#include "QpsResultTable.h"

#include <stdexcept>
#include <string>
#include <unordered_set>
#include <unordered_map>

spa::QpsResultTable::QpsResultTable(const std::vector<std::string>& headers,
                                    const std::vector<std::vector<QpsValue>>& rows)
  : headers(headers), rows(rows) {
}

void spa::QpsResultTable::addHeader(std::string header) {
  headers.push_back(header);
}

void spa::QpsResultTable::addHeader(PqlArgument arg) {
  if (arg.getType() != SYNONYM) {
    return headers.push_back("");
  }
  headers.push_back(arg.getValue());
}

std::pair<int, int> spa::QpsResultTable::getDimension() {
  return { headers.size(), rows.size() };
}

void spa::QpsResultTable::addRow(std::vector<QpsValue> row) {
  if (row.size() != headers.size()) {
    throw std::runtime_error("QpsResultTable addRow: row size not equal to columns");
  }
  rows.push_back(row);
}

std::unordered_set<spa::QpsValue,
                   spa::QpsValueHash,
                   spa::QpsValueEquality> spa::QpsResultTable::getColumn(std::string header) {
  int headerIndex = -1;
  for (int i = 0; i < headers.size(); ++i) {
    if (headers[i] == header) {
      headerIndex = i;
      break;
    }
  }
  if (headerIndex == -1) {
    return {};
  }
  std::unordered_set<QpsValue, QpsValueHash, QpsValueEquality> result;
  for (auto& row : rows) {
    result.insert(row[headerIndex]);
  }
  return result;
}

std::optional<std::vector<spa::QpsValue>>
     spa::QpsResultTable::innerJoinRow(std::vector<std::string>& resultHeaders,
                                       std::vector<QpsValue>& row,
                                       std::vector<QpsValue>& otherRow) {
  std::vector<spa::QpsValue> result;
  std::unordered_map<std::string, QpsValue> valueMap;
  for (int i = 0; i < row.size(); ++i) {
    std::string& header = resultHeaders[i];
    QpsValue& val = row[i];
    result.push_back(val);
    if (!header.empty()) {
      valueMap.insert({header, val});
    }
  }
  for (int i = 0; i < otherRow.size(); ++i) {
    std::string& header = resultHeaders[row.size() + i];
    QpsValue& val = otherRow[i];
    if (!header.empty()) {
      auto it = valueMap.find(header);
      if (it != valueMap.end() && it->second != val) {
        return {};
      }
    }
    result.push_back(val);
  }
  return { result };
}

spa::QpsResultTable spa::QpsResultTable::innerJoin(QpsResultTable& other) {
  std::vector<std::string> resultHeaders;
  for (auto& s : headers) {
    resultHeaders.push_back(s);
  }
  for (auto& s : other.headers) {
    resultHeaders.push_back(s);
  }
  std::vector<std::vector<QpsValue>> resultRows;
  for (auto& row : rows) {
    for (auto& otherRow : other.rows) {
      std::optional<std::vector<spa::QpsValue>> resultRowOpt =
        innerJoinRow(resultHeaders, row, otherRow);
      if (resultRowOpt) {
        resultRows.push_back(resultRowOpt.value());
      }
    }
  }
  return QpsResultTable(resultHeaders, resultRows);
}
