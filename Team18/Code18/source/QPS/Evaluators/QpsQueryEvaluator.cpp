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
  for (auto& p : child->headerTablesMap) {
    for (auto tableP : p.second) {
      headerTablesMap[p.first].insert(tableP);
    }
  }
  for (auto& p : child->headerUsageMap) {
    headerUsageMap[p.first] += p.second;
  }
}

void spa::TableGroup::addTable(QpsResultTable& other) {
  for (auto& header : other.getHeaderNames()) {
    headerTablesMap[header].insert(&other);
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

std::vector<spa::QpsResultTable*> spa::TableGroup::getBestPlan() {
  std::string startingHeader;
  int startingHeaderUsage = INT_MAX;
  for (auto& p : headerUsageMap) {
    if (p.second < startingHeaderUsage) {
      startingHeaderUsage = p.second;
      startingHeader = p.first;
    }
  }
  std::vector<QpsResultTable*> result;
  std::unordered_set<QpsResultTable*> usedTables;
  std::unordered_set<std::string> seenHeaders;
  std::queue<std::string> headers;
  headers.push(startingHeader);
  while (!headers.empty()) {
    auto header = headers.front();
    headers.pop();
    if (seenHeaders.find(header) != seenHeaders.end()) {
      continue;
    }
    seenHeaders.insert(header);
    for (auto tableP : headerTablesMap[header]) {
      if (usedTables.find(tableP) != usedTables.end()) {
        continue;
      }
      usedTables.insert(tableP);
      result.push_back(tableP);
      for (auto& header : tableP->getHeaderNames()) {
        headers.push(header);
      }
    }
  }
  return result;
}

void spa::TableGroup::dropColumns(QpsResultTable& table,
                                  std::vector<std::string>& headers,
                                  std::vector<std::string>& selectColumns) {
  if (!selectColumns.empty() && selectColumns.size() < headers.size()) {
    table = table.getColumns(selectColumns);
  }
}

void spa::TableGroup::dropIncomingTableColumns(QpsResultTable& table,
                                               QpsResultTable& result,
                                               ParsedQuery& parsedQuery) {
  auto tableHeaders = table.getHeaderNames();
  std::vector<std::string> tableSelectColumns;
  for (auto& header : tableHeaders) {
    int usage = --headerUsageMap[header];
    if (usage > 0 || result.hasHeader(header) || parsedQuery.hasSelectColumn(header)) {
      tableSelectColumns.push_back(header);
    }
  }
  dropColumns(table, tableHeaders, tableSelectColumns);
}

void spa::TableGroup::dropResultTableColumns(QpsResultTable& result,
                                               ParsedQuery& parsedQuery) {
  auto resultHeaders = result.getHeaderNames();
  std::vector<std::string> resultSelectColumns;
  for (auto& header : resultHeaders) {
    if (headerUsageMap[header] > 0 || parsedQuery.hasSelectColumn(header)) {
      resultSelectColumns.push_back(header);
    }
  }
  dropColumns(result, resultHeaders, resultSelectColumns);
}

spa::QpsResultTable spa::TableGroup::getTable(ParsedQuery& parsedQuery) {
  QpsResultTable result;
  bool init = false;
  auto tables = getBestPlan();
  for (auto tableP : tables) {
    auto& table = *tableP;
    dropIncomingTableColumns(table, result, parsedQuery);
    innerJoin(table, result, init);
    dropResultTableColumns(result, parsedQuery);
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
