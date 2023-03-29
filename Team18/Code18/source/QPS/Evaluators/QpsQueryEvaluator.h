#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

#include "ParsedQuery.h"
#include "QpsResultTable.h"
#include "PKBManager.h"

namespace spa {

class TableGroup {
 private:
  TableGroup* parent;
  std::unordered_map<std::string, int> headerUsageMap;
  std::unordered_map<std::string, std::unordered_set<QpsResultTable*>> headerTablesMap;
  void innerJoin(QpsResultTable& table, QpsResultTable& result, bool& init);
  std::vector<QpsResultTable*> getBestPlan();
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
