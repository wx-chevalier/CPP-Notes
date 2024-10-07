
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "andquery.h"
#include "orquery.h"
#include "query.h"
#include "query_base.h"
#include "queryresult.h"
#include "textquery.h"

int main() {
  std::ifstream file("storyDataFile.txt");

  TextQuery tQuery(file);

  Query q = Query("hello") | Query("am");

  return 0;
}
