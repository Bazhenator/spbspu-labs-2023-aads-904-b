#ifndef BINARY_TREE_FUNCTOR_H
#define BINARY_TREE_FUNCTOR_H

#include <utility>
#include <string>

namespace ponomarev {
  struct BinaryTreeSummFunctor {
    void operator()(const std::pair< int, std::string > &node);

    std::pair< int, std::string > result_{0, ""};
  };
}

#endif
