#include "QpsQueryEvaluator.h"
#include "SimpleEvaluator.h"
#include "NoSynonymClauseGroup.h"
#include "QueryOptimizer.h"

#include <vector>
#include <memory>
#include <string>
#include <utility>

spa::QpsQueryEvaluator::QpsQueryEvaluator(ParsedQuery& parsedQuery) : parsedQuery(parsedQuery) {
}

spa::TableGroup::TableGroup() {
  table.addHeader("");
  table.addRow({ QpsValue(0) });
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
  table = table.innerJoin(child->table);
}

void spa::TableGroup::innerJoin(QpsResultTable& other) {
  table = table.innerJoin(other);
}

const spa::QpsResultTable& spa::TableGroup::getTable() {
  return table;
}

void spa::QpsQueryEvaluator::unionTable(std::unordered_map<std::string, TableGroup>& groupMap,
                                        QpsResultTable& table, QpsResultTable& result) {
  auto headerNames = table.getHeaderNames();
  if (headerNames.empty()) {
    result = result.innerJoin(table);
    return;
  }
  std::unordered_set<TableGroup*> groups;
  for (auto& header : headerNames) {
    groups.insert(groupMap[header].getParent());
  }
  auto parent = *groups.begin();
  parent->innerJoin(table);
  auto it = groups.begin();
  for (++it; it != groups.end(); ++it) {
    parent->unionChild(*it);
  }
}

void spa::QpsQueryEvaluator::unionTables(std::unordered_map<std::string, TableGroup>& groupMap, QpsResultTable& result, PKBManager& pkbManager) {
  for (auto& p : parsedQuery.getSelectWithDeclarations()) {
    SimpleEvaluator eval(p.first, p.second);
    unionTable(groupMap, eval.evaluate(pkbManager), result);
  }
  for (auto& clause : parsedQuery.getSuchThatClauses()) {
    auto eval = clause.getEvaluator();
    unionTable(groupMap, eval->evaluate(pkbManager), result);
  }
  for (auto& clause : parsedQuery.getPatternClauses()) {
    auto eval = clause.getEvaluator();
    unionTable(groupMap, eval->evaluate(pkbManager), result);
  }
  for (auto& clause : parsedQuery.getWithClauses()) {
    auto eval = clause.getEvaluator();
    unionTable(groupMap, eval->evaluate(pkbManager), result);
  }
}

spa::QpsResultTable spa::QpsQueryEvaluator::evaluate(PKBManager& pkbManager) {
  std::unordered_map<std::string, TableGroup> groupMap;
  QpsResultTable result;
  result.addHeader("");
  result.addRow({ QpsValue(0) });
  unionTables(groupMap, result, pkbManager);

  auto& selectWithDeclarations = parsedQuery.getSelectWithDeclarations();
  for (auto& p : groupMap) {
    if (!p.second.isParent()) {
      continue;
    }
    auto table = p.second.getTable();
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
