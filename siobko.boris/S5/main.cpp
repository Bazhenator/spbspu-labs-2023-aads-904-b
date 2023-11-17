#include <iostream>
#include <fstream>
#include <deque>

#include "input.h"
#include "TraversalCommandManagement.h"
#include "NodeFunctor.h"
#include "output.h"

int main(int argc, char *argv[])
{
  if (argc != 3) {
    std::cerr << "Invalid argv amount";
    return 1;
  }

  const char *filename = argv[2];
  const char *traversalCategory = argv[1];

  std::ifstream fin(filename);
  if (!fin.is_open()) {
    std::cerr << "Error: invalid filename";
    return 1;
  }
  std::deque< std::string > dictionaryInfo = siobko::inputTextLineFromFile(fin);
  if (dictionaryInfo.empty()) {
    siobko::printEmptyErrorMessage(std::cout);
    return 0;
  }

  siobko::NodeFunctor functor;
  try {
    siobko::TraversalCommandManagement management(dictionaryInfo, traversalCategory);
    functor = management.executeCommand();
  }
  catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return 1;
  }

  std::cout << functor.keyResult << " " << functor.valueResult << '\n';
}
