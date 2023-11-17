#include <iostream>
#include <cstring>
#include "sortingsManager.h"

int main(int argc, char* argv[]) {
  if (argc != 4) {
    std::cerr << "Invalid amount of CML arguments!" << '\n';
    return 2;
  }
  std::size_t size = std::strtoull(argv[3], nullptr, 10);
  if (errno == ERANGE || size == 0) {
    std::cerr << "Incorrect size parameter.\n";
    return 2;
  }
  if (!strcmp("ints", argv[2])) {
    std::vector< int > data;
    try {
      flusova::sortingsManager< std::vector< int > > manager(argv[1], size, data);
      manager.fillContainerWithInts();
      manager.processingDeque();
      manager.processingList();
      manager.processingForwardList();
    } catch (const std::invalid_argument& e) {
      std::cerr << e.what() << '\n';
      return 1;
    }
  } else if (!strcmp("floats", argv[2])) {
    std::vector< float > data;
    try {
      flusova::sortingsManager< std::vector< float > > manager(argv[1], size, data);
      manager.fillContainerWithFloats();
      manager.processingDeque();
      manager.processingList();
      manager.processingForwardList();
    } catch (const std::invalid_argument& e) {
      std::cerr << e.what() << '\n';
      return 1;
    }
  } else {
    std::cerr << "Invalid nums type was given!" << '\n';
    return 1;
  }
  return 0;
}
