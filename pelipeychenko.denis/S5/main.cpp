#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <outputFunctions.h>
#include <BinarySearchTreeIterators.h>

#include "pairSummCounter.h"

int main(int argc, char *argv[])
{
  using namespace pelipeychenko;
  if (argc != 3) {
    std::cerr << "Wrong amount of arguments" << "\n";
    return 1;
  }

  BinarySearchTree< int, std::string > tree;
  try {
    std::string mode(argv[1]);
    tree.setTraversalMode(mode);
  } catch (const std::runtime_error &e) {
    std::cerr << e.what() << "\n";
    return 1;
  }

  std::ifstream in(argv[2]);
  if (!in) {
    std::cerr << "Can't open file" << '\n';
    return 1;
  }

  while (!in.eof()) {
    std::string line;
    in >> line;
    int key;
    try {
      key = std::stoi(line);
    } catch (const std::out_of_range &e) {
      std::cerr << e.what() << "\n";
      return 1;
    } catch (const std::invalid_argument &e) {
      break;
    }
    in >> line;
    tree.merge(key, line);
  }
  if (tree.isEmpty()) {
    printEmpty(std::cout);
    return 0;
  }

  pairSummCounter result;
  result = tree.traverse(result);
  result.printSumm(std::cout);
  return 0;
}
