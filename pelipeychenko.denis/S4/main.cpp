#include <iostream>
#include <fstream>

#include <DictionaryManager.h>
#include <BinarySearchTreeIterators.h>

int main(int argc, char *argv[])
{
  using namespace pelipeychenko;
  if (argc != 2) {
    std::cerr << "wrong amount of arguments" << "\n";
    return 1;
  }
  std::ifstream in;
  in.open(argv[1]);
  DictionaryManager< int, std::string, BinarySearchTree > dictionaryManager;
  try {
    dictionaryManager.inputDictionaries(in);
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
    return 1;
  }

  dictionaryManager.doCommands(std::cin);
  return 0;
}
