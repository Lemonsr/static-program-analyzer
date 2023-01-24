#ifndef TEAM18_CODE18_AUTOTESTER_SOURCE_TESTWRAPPER_H_
#define TEAM18_CODE18_AUTOTESTER_SOURCE_TESTWRAPPER_H_

#include <string>
#include <iostream>
#include <list>

// include your other headers here
#include "AbstractWrapper.h"

class TestWrapper : public AbstractWrapper {
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
