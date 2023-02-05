#ifndef TEAM18_CODE18_AUTOTESTER_SOURCE_TESTWRAPPER_H_
#define TEAM18_CODE18_AUTOTESTER_SOURCE_TESTWRAPPER_H_

#include <string>
#include <iostream>
#include <list>
#include <memory>

// include your other headers here
#include "AbstractWrapper.h"
#include "PKBManager.h"
#include "PKB.h"

class TestWrapper : public AbstractWrapper {
 private:
   std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
 public:
  // default constructor
  TestWrapper();

  // destructor
  ~TestWrapper();

  // method for parsing the SIMPLE source
  virtual void parse(std::string filename);

  // method for evaluating a query
  virtual void evaluate(std::string query, std::list<std::string>& results);
};

#endif  // TEAM18_CODE18_AUTOTESTER_SOURCE_TESTWRAPPER_H_
