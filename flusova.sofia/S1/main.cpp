#include <iostream>
#include <string>
#include <fstream>
#include "Stack.h"
#include "manageData.h"

int main(int argc, char* argv[])
{
  std::string strInput = "";
  flusova::manageExpression expression;
  flusova::Stack< long long > stackResult;
  if (argc == 1)
  {
    do {
      if (strInput == "") {
        continue;
      }
      try
      {
        expression.convertToPolish(strInput);
        expression.calculatePolish(stackResult);
      }
      catch (std::runtime_error &e) {
        std::cerr << e.what() << '\n';
        return 1;
      }
    } while (std::getline(std::cin, strInput));
    expression.printResult(stackResult);
  }
  else if (argc == 2)
  {
    std::ifstream fileName;
    fileName.open(argv[1]);
    if (!fileName.is_open())
    {
      std::cerr << "No such file in directory!" << '\n';
      return 1;
    }
    while (std::getline(fileName, strInput))
    {
      if (strInput == "") {
        continue;
      }
      try
      {
        expression.convertToPolish(strInput);
        expression.calculatePolish(stackResult);
      }
      catch (...) {
        std::cerr << "Errors!" << '\n';
        return 1;
      }
    }
    expression.printResult(stackResult);
    fileName.close();
  }
  if (stackResult.isEmpty())
  {
    std::cout << '\n';
  }
  return 0;
}
