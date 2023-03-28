#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

#include "ParsedQuery.h"
#include "QpsResultTable.h"
#include "PKBManager.h"

namespace spa {

struct OrderedTable {
  QpsResultTable* tableP = nullptr;
  int compareUsage = 0;
  std::string compareHeader;

  OrderedTable() = default;
  OrderedTable(QpsResultTable& table, std::unordered_map<std::string, int>& headerUsageMap);
};

struct OrderedTablePriority {
  bool operator()(const OrderedTable& lhs, const OrderedTable& rhs) const;
};

class TableGroup {
 private:
  TableGroup* parent;
  std::unordered_map<std::string, int> headerUsageMap;
  std::unordered_set<QpsResultTable*> tables;
  void innerJoin(QpsResultTable& table, QpsResultTable& result, bool& init);
 public:
  TableGroup();
  bool isParent();
  TableGroup* getParent();
  void unionChild(TableGroup* child);
  void addTable(QpsResultTable& other);
  QpsResultTable getTable(ParsedQuery& parsedQuery);
};

class QpsQueryEvaluator : public QpsEvaluator {
 private:
  ParsedQuery& parsedQuery;
  void unionTable(std::unordered_map<std::string, TableGroup>& groupMap,
                  QpsResultTable& table, QpsResultTable& result);
  void extractTables(std::vector<QpsResultTable>& tables, PKBManager& pkbManager);
 public:
  explicit QpsQueryEvaluator(ParsedQuery& parsedQuery);
  QpsResultTable evaluate(PKBManager& pkbManager);
};
}  // namespace spa
