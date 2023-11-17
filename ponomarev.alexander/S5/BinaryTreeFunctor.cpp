#include "BinaryTreeFunctor.h"

#include <limits>
#include <stdexcept>

void ponomarev::BinaryTreeSummFunctor::operator()(const std::pair< int, std::string > &node)
{
  static const int MAX_INT = std::numeric_limits< int >::max();
  static const int MIN_INT = std::numeric_limits< int >::min();

  if (node.first > 0) {
    if (result_.first > MAX_INT - node.first) {
      throw std::runtime_error("Overflow.");
    }
  } else {
    if (result_.first < MIN_INT - node.first) {
      throw std::runtime_error("Underflow.");
    }
  }

  result_.first += node.first;
  if (result_.second.empty()) {
    result_.second += node.second;
  } else {
    result_.second += ' ' + node.second;
  }
}
