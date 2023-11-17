#include <iostream>
#include <fstream>

#include "MathCalculator.h"

int main(int argc, char *argv[])
{
  pelipeychenko::MathCalculator calculator;

  if (argc == 2) {
    std::ifstream in;
    try {
      in.open(argv[1]);
      calculator.executeCalculator(in);
    } catch (const std::exception &e) {
      std::cerr << e.what() << '\n';
      return 1;
    }
  } else {
    try {
      calculator.executeCalculator(std::cin);
    } catch (const std::exception &e) {
      std::cerr << e.what() << '\n';
      return 1;
    }
  }
  return 0;
}
