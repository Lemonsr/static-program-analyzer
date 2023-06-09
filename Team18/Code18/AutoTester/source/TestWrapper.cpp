#include "TestWrapper.h"
#include "SP.h"
#include "QPS.h"
#include "SP.h"

#include <iostream>
#include <fstream>

// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper* WrapperFactory::wrapper = 0;

AbstractWrapper* WrapperFactory::createWrapper() {
    if (wrapper == 0) wrapper = new TestWrapper;
    return wrapper;
}

// Do not modify the following line
volatile bool TestWrapper::GlobalStop = false;

// a default constructor
TestWrapper::TestWrapper() {
    // create any objects here as instance variables of this class
    // as well as any initialization required for your spa program
}

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
    // call your parser to do the parsing
    // ...rest of your code...
    std::ifstream sourceFile(filename);
    const std::string source = std::string((std::istreambuf_iterator<char>(sourceFile)),
        std::istreambuf_iterator<char>());
    spa::SP sp = spa::SP(source, *pkbManager);
    sp.processSource();
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string>& results) {
    // call your evaluator to evaluate the query here
    // ...code to evaluate query...

    // store the answers to the query in the results list (it is initially empty)
    // each result must be a string.
  spa::QPS qps;
  spa::QpsResult qpsResult = qps.evaluate(query, *pkbManager);
  const auto& errorMessage = qpsResult.getErrorMessage();
  if (errorMessage) {
    results.push_back(errorMessage.value());
    return;
  }

  results = qpsResult.getResults();
}
