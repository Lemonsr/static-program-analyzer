#pragma once

#include "ParsedQuery.h"
#include "QpsResultTable.h"
#include "PKBManager.h"

namespace spa {

class TableGroup {
 private:
  QpsResultTable table;
  TableGroup* parent;

 public:
  TableGroup();
  bool isParent();
  TableGroup* getParent();
  void unionChild(TableGroup* child);
  void innerJoin(QpsResultTable& other);
  const QpsResultTable& getTable();
};

class QpsQueryEvaluator : public QpsEvaluator {
 private:
  ParsedQuery& parsedQuery;
  void unionTable(std::unordered_map<std::string, TableGroup>& groupMap,
                  QpsResultTable& table, QpsResultTable& result);
 public:
  explicit QpsQueryEvaluator(ParsedQuery& parsedQuery);
  QpsResultTable evaluate(PKBManager& pkbManager);
};
}  // namespace spa
