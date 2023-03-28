#include "QpsQueryEvaluator.h"

#include <vector>
#include <memory>
#include <string>
#include <utility>
#include <unordered_map>
#include <unordered_set>

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
  children.insert(child);
}

void spa::TableGroup::addTable(QpsResultTable& other) {
  tables.insert(&other);
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

spa::QpsResultTable spa::TableGroup::getTable() {
  QpsResultTable result;
  bool init = false;
  for (auto it = tables.begin(); it != tables.end(); ++it) {
    innerJoin(*(*it), result, init);
  }
  for (auto child : children) {
    auto temp = child->getTable();
    innerJoin(temp, result, init);
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
  groupMap[headerNames[0]].addTable(table);
  std::unordered_set<TableGroup*> parents;
  for (auto& header : headerNames) {
    parents.insert(groupMap[header].getParent());
  }
  auto parent = *parents.begin();
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

    QpsResultTable table = group.getTable();
    if (table.isEmpty()) {
      return table;
    }

    std::vector<std::string> reqColumns;
    for (auto& header : table.getHeaderNames()) {
      auto synonym = header.substr(0, header.find('.'));
      if (selectWithDeclarations.find(synonym) != selectWithDeclarations.end()) {
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
