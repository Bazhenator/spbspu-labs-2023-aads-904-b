#include <iostream>
#include <cstring>
#include <cstddef>
#include <fstream>
#include <manageWithCStrings.h>
#include <Stack.h>
#include "MathExpressionSolver.h"

int main(int argc, char *argv[])
{
  bool continueInput = true;
  std::ifstream inputFile;
  if (argc == 2) {
    inputFile.open(argv[1]);
    if (!inputFile.is_open()) {
      std::cerr << "Error: File couldn't be opened.\n";
      return 1;
    }
    inputFile.seekg(0, std::ios::end);
    if (inputFile.tellg() < 1) {
      continueInput = false;
    }
    inputFile.clear();
    inputFile.seekg(0, std::ios::beg);
  }

  ponomarev::Stack< long long > answers;
  while (continueInput) {
    std::size_t capacity = 20;
    char *string = nullptr;
    try {
      if (inputFile.is_open()) {
        string = ponomarev::input(inputFile, capacity);

        continueInput = !(inputFile.eof());
      } else {
        string = ponomarev::input(std::cin, capacity);
        continueInput = !std::cin.eof();
      }
    } catch (const std::bad_alloc &e) {
      std::cerr << "Error: bad_alloc occured\n";
      return 1;
    } catch (const std::runtime_error &e) {
      continue;
    }
    try {
      answers.push(ponomarev::solveMathExpr(string));
    } catch (...) {
      std::cerr << "Error: Expression is not correct.\n";
      delete[] string;
      return 1;
    }
    delete[] string;
  }
  if (answers.isEmpty()) {
    std::cout << '\n';
  }
  while (!answers.isEmpty()) {
    bool lastNumber = (answers.getSize() == 1);
    std::cout << answers.getTop() << (lastNumber ? '\n' : ' ');
    answers.pop();
  }

  if (inputFile.is_open()) {
    inputFile.close();
  }
  return 0;
}
