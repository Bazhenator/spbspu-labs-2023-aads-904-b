#include <iostream>
#include <random>
#include <ctime>
#include <cstring>
#include <vector>
#include "SortsTester.h"

int main(int argc, char **argv)
{
  if (argc != 4) {
    std::cerr << "CML args aren't correct.\n";
    return 2;
  }

  std::size_t containerSize = std::strtoull(argv[3], nullptr, 10);
  if (errno == ERANGE || containerSize == 0) {
    std::cerr << "Incorect size parameter.\n";
    return 2;
  }

  if (!std::strcmp(argv[2], "ints")) {
    std::vector< int > set(containerSize);

    std::srand(std::time(nullptr));
    std::generate(set.begin(), set.end(),
      []()
      {
        return std::rand() % 1001;
      });

    ponomarev::SortsTester< int > tester(std::cout, set);
    try {
      tester.setOrder(argv[1]);
    } catch (const std::invalid_argument &e) {
      std::cerr << "Error: " << e.what() << '\n';
      return 1;
    }
    tester.applyOnDeque();
    tester.applyOnList();
    tester.applyOnForwardList();
  } else if (!std::strcmp(argv[2], "floats")) {
    std::vector< float > set(containerSize);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution< float > dis(0.0, 100.0);
    std::generate(set.begin(), set.end(),
      [&dis, &gen]()
      {
        return dis(gen);
      });

    ponomarev::SortsTester< float > tester(std::cout, set);
    try {
      tester.setOrder(argv[1]);
    } catch (const std::invalid_argument &e) {
      std::cerr << "Error: " << e.what() << '\n';
      return 1;
    }
    tester.applyOnDeque();
    tester.applyOnList();
    tester.applyOnForwardList();
  } else {
    std::cerr << "Incorrect type for sorting.\n";
    return 2;
  }
  return 0;
}
