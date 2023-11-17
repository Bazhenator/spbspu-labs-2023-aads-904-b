#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <BinarySearchTree.h>
#include <CommandAnswer.h>
#include <CommandHandler.h>
#include "BinaryTreeFunctor.h"

int main(int args, char **argv)
{
  using Tree = ponomarev::AVLTree< int, std::string >;

  if (args != 3) {
    std::cerr << "Error: CML args are not correct\n";
    return 2;
  }
  std::ifstream file(argv[2]);
  if (!file.is_open()) {
    std::cerr << "Error: File couldn't be opened\n";
    return 1;
  }

  Tree data;
  try {
    ponomarev::inputMapped(file, data);
  } catch (const std::out_of_range &e) {
    std::cerr << "Error: " << e.what() << '\n';
    return 1;
  } catch (...) {}

  if (data.isEmpty()) {
    ponomarev::printEmpty(std::cout);
    return 0;
  }

  ponomarev::BinaryTreeSummFunctor f;
  if (!std::strcmp("ascending", argv[1])) {
    try {
      f = data.traverse_lnr(f);
    } catch (const std::runtime_error &e) {
      std::cerr << "Error: " << e.what() << '\n';
      return 1;
    }
  } else if (!std::strcmp("descending", argv[1])) {
    try {
      f = data.traverse_rnl(f);
    } catch (const std::runtime_error &e) {
      std::cerr << "Error: " << e.what() << '\n';
      return 1;
    }
  } else if (!std::strcmp("breadth", argv[1])) {
    try {
      f = data.traverse_breadth(f);
    } catch (const std::runtime_error &e) {
      std::cerr << "Error: " << e.what() << '\n';
      return 1;
    }
  } else {
    std::cerr << "Error: Unsupported traversal\n";
    return 1;
  }

  std::cout << f.result_.first << ' ' << f.result_.second << '\n';
  return 0;
}
