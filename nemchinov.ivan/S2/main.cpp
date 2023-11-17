#include <fstream>

#include "QueryManagement.h"

int main(int argc, char *argv[])
{
  if (argc == 1) return 1;

  std::ifstream inputFile(argv[1]);
  if (!inputFile)
  {
    return 1;
  }

  nemchinov::Dictionary< std::string, nemchinov::Dictionary< int, std::string > > dictionaryOfDicts{};

  std::string inputStr;
  while (getline(inputFile, inputStr))
  {
    if (inputStr.empty())
    {
      continue;
    }
    try
    {
      nemchinov::CommandManagement::putInDict(dictionaryOfDicts, inputStr);
    }
    catch (...)
    {
      std::cerr << "Error : incorrect file date" << '\n';
      return 1;
    }
  }
  inputFile.close();

  nemchinov::CommandManagement{dictionaryOfDicts};

  return 0;
}
