#include <iostream>
#include <fstream>
#include <DictionaryManager.h>

#include "Dictionary.h"

int main(int argc, char *argv[])
{
  if (argc != 2) {
    std::cerr << "wrong amount of arguments" << "\n";
    return 1;
  }
  std::ifstream in;
  in.open(argv[1]);
  pelipeychenko::DictionaryManager< int, std::string, pelipeychenko::Dictionary > dictionaryManager;
  try {
    dictionaryManager.inputDictionaries(in);
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
    return 1;
  }

  dictionaryManager.doCommands(std::cin);

  return 0;
}
