#include <iostream>
#include <fstream>
#include <string>
#include "BST.h"
#include "BSTIterators.h"
#include "DManager.h"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Invalid amount of CML arguments!";
    return 1;
  }
  std::ifstream fileName;
  fileName.open(argv[1], std::ios::in);
  if (!fileName.is_open()) {
    std::cerr << "Opening error was occured!";
    return 1;
  }
  flusova::DictManager< flusova::BinarySearchTree,
          std::string, flusova::BinarySearchTree< int, std::string > > data(std::cout);
  data.inputDictionary(fileName);
  std::string userInput = "";
  while (std::getline(std::cin, userInput)) {
    if (userInput != "") {
      data.manageDictionary(userInput);
    }
  }
  fileName.close();
  return 0;
}
