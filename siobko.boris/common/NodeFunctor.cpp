#include "NodeFunctor.h"

#include <limits>
#include <stdexcept>

namespace siobko {
  void NodeFunctor::operator()(const std::pair< int, std::string >& node)
  {
    if (node.first < 0 && (std::numeric_limits< int >::min() - node.first) > keyResult) {
      throw std::logic_error("Error: integer overflow.");
    }
    if (node.first > 0 && (std::numeric_limits< int >::max() - node.first) < keyResult) {
      throw std::logic_error("Error: integer overflow.");
    }

    keyResult += node.first;
    valueResult += valueResult.empty() ? node.second : " " + node.second;
  }
}

