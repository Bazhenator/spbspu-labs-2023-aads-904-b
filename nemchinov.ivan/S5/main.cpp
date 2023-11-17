#include <iostream>
#include <fstream>

#include "QueryManagement.h"

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    return 1;
  }

  std::ifstream inputFile(argv[2]);
  if (!inputFile)
  {
    return 1;
  }

  using namespace nemchinov;
  BinarySearchTree< int, std::string > avlTree{};

  std::string inputStr;
  if (!getline(inputFile, inputStr) || inputStr.empty())
  {
    printEmptyDict(std::cout);
    return 0;
  }
  try
  {
    CommandManager(inputStr, std::string(argv[1]), std::cout);
  }
  catch (...)
  {
    std::cerr << "Error : incorrect input date" << '\n';
    return 1;
  }
  return 0;
}
