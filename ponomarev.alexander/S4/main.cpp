#include <iostream>
#include <fstream>
#include <string>
#include <CommandHandler.h>
#include <BinarySearchTree.h>
#include <AVLTreeFunctional.h>

int main(int args, char **argv)
{
  if (args != 2) {
    std::cerr << "Error: CML arguments aren't correct.";
    return 2;
  }

  std::ifstream file(argv[1]);
  if (!file.is_open()) {
    std::cerr << "Error: File couldn't be opened";
    return 2;
  }

  ponomarev::CommandHandler< ponomarev::AVLTree, std::string, ponomarev::AVLTree< int, std::string > > handler(std::cout);
  handler.inputData(file);
  std::string command;
  while (std::getline(std::cin, command)) {
    handler(command);
  }
  file.close();
  return 0;
}
