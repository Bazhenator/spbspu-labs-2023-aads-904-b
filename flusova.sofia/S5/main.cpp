#include <iostream>
#include <fstream>
#include <cstring>
#include "CMDmap.h"

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cerr << "Invalid amount of CML arguments!" << '\n';
    return 1;
  }
  std::ifstream fileName;
  fileName.open(argv[2], std::ios::in);
  if (!fileName.is_open()) {
    std::cerr << "Opening error was occured!" << '\n';
    return 1;
  }
  std::string order = "";
  if (!strcmp(argv[1], "ascending")) {
    order = "ascending";
  } else if (!strcmp(argv[1], "descending")) {
    order = "descending";
  } else if (!strcmp(argv[1], "breadth")) {
    order = "breadth";
  } else {
    std::cerr << "Invalid traverse!" << '\n';
    return 1;
  }
  try {
    flusova::CMDmap command(order);
    command.inputData(fileName);
    command.doTraverse();
  } catch (...) {
    std::cerr << "Errors!" << '\n';
    return 1;
  }
  fileName.close();
  return 0;
}
