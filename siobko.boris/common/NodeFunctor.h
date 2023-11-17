
#ifndef FUNCTOR_H
#define FUNCTOR_H

#include <string>
#include <utility>

namespace siobko {
  struct NodeFunctor {
    void operator()(const std::pair< int, std::string >& node);

    int keyResult = 0;
    std::string valueResult;
  };
}
#endif
