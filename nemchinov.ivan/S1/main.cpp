#include <iostream>
#include <fstream>
#include <Stack.h>

#include "PostfixMathExpr.h"

int main(int argc, char *argv[])
{
  nemchinov::Queue< std::string > stringsToProcess = nemchinov::Queue< std::string >{};

  if (argc == 1)
  {
    std::string inputStr;
    while (getline(std::cin, inputStr))
    {
      if (inputStr.empty())
      {
        continue;
      }
      stringsToProcess.push(inputStr);
    }
  } else
  {
    std::ifstream inputFile(argv[1]);
    if (!inputFile)
    {
      return 1;
    }
    std::string inputStr;
    while (getline(inputFile, inputStr))
    {
      if (inputStr.empty())
      {
        continue;
      }
      stringsToProcess.push(inputStr);
    }
    inputFile.close();
  }

  nemchinov::Stack< long long > answers = nemchinov::Stack< long long >{};

  while (!stringsToProcess.isEmpty())
  {
    try
    {
      answers.push(nemchinov::PostfixMathExpr(stringsToProcess.get()).getResultOfMathExpr());
      stringsToProcess.pop();
    }
    catch (const std::exception &error)
    {
      std::cerr << error.what() << '\n';
      return 1;
    }
  }

  while (!answers.isEmpty())
  {
    std::cout << answers.get();
    answers.pop();
    if (!answers.isEmpty())
    {
      std::cout << " ";
    }
  }
  std::cout << "\n";

  return 0;
}
