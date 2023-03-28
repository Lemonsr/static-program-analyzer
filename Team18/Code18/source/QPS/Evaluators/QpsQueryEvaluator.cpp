#include "QpsQueryEvaluator.h"

#include <vector>
#include <memory>
#include <string>
#include <utility>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <algorithm>

#include "SimpleEvaluator.h"

spa::QpsQueryEvaluator::QpsQueryEvaluator(ParsedQuery& parsedQuery) : parsedQuery(parsedQuery) {
}

spa::TableGroup::TableGroup() {
  parent = nullptr;
}

bool spa::TableGroup::isParent() {
  return parent == nullptr;
}

spa::TableGroup* spa::TableGroup::getParent() {
  if (parent == nullptr) {
    return this;
  }
  return parent->getParent();
}

void spa::TableGroup::unionChild(TableGroup* child) {
  child->parent = this;
  for (auto table : child->tables) {
    tables.insert(table);
  }
  for (auto& p : child->headerUsageMap) {
    headerUsageMap[p.first] += p.second;
  }
}

void spa::TableGroup::addTable(QpsResultTable& other) {
  tables.insert(&other);
  for (auto& header : other.getHeaderNames()) {
    ++headerUsageMap[header];
  }
}

void spa::TableGroup::innerJoin(QpsResultTable& table, QpsResultTable& result, bool& init) {
  if (!init) {
    result = table;
    init = true;
    return;
  }
  if (result.isEmpty()) {
    return;
  }
  if (table.isEmpty()) {
    result = table;
  } else if (!table.getHeaderNames().empty()) {
    result = result.innerJoin(table);
  }
}

spa::OrderedTable::OrderedTable(QpsResultTable& table,
                                std::unordered_map<std::string, int>& headerUsageMap) {
  tableP = &table;
  for (auto& header : table.getHeaderNames()) {
    int usage = headerUsageMap[header];
    if (usage > compareUsage) {
      compareUsage = usage;
      compareHeader = header;
    } else if (usage == compareUsage && header < compareHeader) {
      compareHeader = header;
    }
  }
}

constexpr bool spa::OrderedTablePriority::operator()(const OrderedTable& lhs, const OrderedTable& rhs) const {
  if (lhs.compareUsage != rhs.compareUsage) {
    return lhs.compareUsage < rhs.compareUsage;
  }
  return lhs.compareHeader < rhs.compareHeader;
}

spa::QpsResultTable spa::TableGroup::getTable(ParsedQuery& parsedQuery) {
  QpsResultTable result;
  bool init = false;
  std::priority_queue<OrderedTable, std::vector<OrderedTable>, OrderedTablePriority> orderedTables;
  for (auto it = tables.begin(); it != tables.end(); ++it) {
    orderedTables.push({ *(*it), headerUsageMap });
  }
  while (!orderedTables.empty()) {
    auto orderedTable = orderedTables.top();
    orderedTables.pop();
    auto& table = *(orderedTable.tableP);
    auto tableHeaders = table.getHeaderNames();
    std::vector<std::string> tableSelectColumns;
    for (auto& header : tableHeaders) {
      int usage = --headerUsageMap[header];
      if (usage > 0 || result.hasHeader(header) || parsedQuery.hasSelectColumn(header)) {
        tableSelectColumns.push_back(header);
      }
    }
    if (tableSelectColumns.size() > 0 && tableSelectColumns.size() < tableHeaders.size()) {
      table = table.getColumns(tableSelectColumns);
    }
    innerJoin(table, result, init);
    auto resultHeaders = result.getHeaderNames();
    std::vector<std::string> resultSelectColumns;
    for (auto& header : resultHeaders) {
      if (headerUsageMap[header] > 0 || parsedQuery.hasSelectColumn(header)) {
        resultSelectColumns.push_back(header);
      }
    }
    if (resultSelectColumns.size() > 0 && resultSelectColumns.size() < resultHeaders.size()) {
      result = result.getColumns(resultSelectColumns);
    }
  }
  if (!init) {
    QpsResultTable dummy;
    dummy.addHeader("");
    dummy.addRow({ QpsValue(0) });
    return dummy;
  }
  return result;
}

void spa::QpsQueryEvaluator::unionTable(std::unordered_map<std::string, TableGroup>& groupMap,
                                        QpsResultTable& table, QpsResultTable& result) {
  auto headerNames = table.getHeaderNames();
  if (headerNames.empty()) {
    result = result.innerJoin(table);
    return;
  }
  std::unordered_set<TableGroup*> parents;
  for (auto& header : headerNames) {
    parents.insert(groupMap[header].getParent());
  }
  auto parent = *parents.begin();
  parent->addTable(table);
  auto it = parents.begin();
  for (++it; it != parents.end(); ++it) {
    parent->unionChild(*it);
  }
}

void spa::QpsQueryEvaluator::extractTables(std::vector<QpsResultTable>& tables, PKBManager& pkbManager) {
  for (auto& p : parsedQuery.getSelectWithDeclarations()) {
    SimpleEvaluator eval(p.first, p.second);
    tables.push_back(eval.evaluate(pkbManager));
  }
  for (auto& clause : parsedQuery.getSuchThatClauses()) {
    auto eval = clause.getEvaluator();
    tables.push_back(eval->evaluate(pkbManager));
  }
  for (auto& clause : parsedQuery.getPatternClauses()) {
    auto eval = clause.getEvaluator();
    tables.push_back(eval->evaluate(pkbManager));
  }
  for (auto& clause : parsedQuery.getWithClauses()) {
    auto eval = clause.getEvaluator();
    tables.push_back(eval->evaluate(pkbManager));
  }
}

spa::QpsResultTable spa::QpsQueryEvaluator::evaluate(PKBManager& pkbManager) {
  std::unordered_map<std::string, TableGroup> groupMap;
  std::vector<QpsResultTable> tables;
  QpsResultTable result;
  result.addHeader("");
  result.addRow({ QpsValue(0) });
  
  extractTables(tables, pkbManager);

  for (auto& table : tables) {
    unionTable(groupMap, table, result);
  }

  auto& selectWithDeclarations = parsedQuery.getSelectWithDeclarations();
  for (auto& p : groupMap) {
    auto& group = p.second;
    if (!group.isParent()) {
      continue;
    }

    QpsResultTable table = group.getTable(parsedQuery);
    if (table.isEmpty()) {
      return table;
    }

    std::vector<std::string> reqColumns;
    for (auto& header : table.getHeaderNames()) {
      if (parsedQuery.hasSelectColumn(header)) {
        reqColumns.push_back(header);
      }
    }

    table = table.getColumns(reqColumns);

    if (!table.isEmpty()) {
      result = result.innerJoin(table);
    }
  }
  return result;
}
