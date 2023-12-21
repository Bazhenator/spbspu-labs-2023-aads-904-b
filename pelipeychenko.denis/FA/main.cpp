#include <iostream>
#include <fstream>
#include "DictionaryManager.h"

int main(int argc, char *argv[])
{
  if (argc == 1) {
    pelipeychenko::DictionaryManager manager(std::cout);
    while (!std::cin.eof()) {
      std::string line;
      std::getline(std::cin, line);
      try {
        manager.doCommand(line);
      } catch (const std::runtime_error &e) {
        std::cout << e.what() << "\n";
      }
    }
  } else if (argc == 2) {
    std::ifstream inputFile;
    inputFile.open(argv[1]);
    pelipeychenko::DictionaryManager manager(std::cout);
    while (!inputFile.eof()) {
      std::string line;
      std::getline(inputFile, line);
      try {
        manager.doCommand(line);
      } catch (const std::runtime_error &e) {
        std::cout << e.what() << "\n";
      }
    }
  } else {
    std::cerr << "Wrong command line arguments count" << "\n";
    return 1;
  }
  return 0;
}
