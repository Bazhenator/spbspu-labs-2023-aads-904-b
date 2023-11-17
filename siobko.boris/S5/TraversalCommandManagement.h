
#ifndef DICTIONAARY_MANAGEMENT_H
#define DICTIONAARY_MANAGEMENT_H

#include <deque>

#include "AVLTree.h"
#include "NodeFunctor.h"

namespace siobko {
  class TraversalCommand {
  public:
    explicit TraversalCommand(const std::string& traversalCategory);

    NodeFunctor execute(const AVLTree< int, std::string >& tree);

  private:
    TraversalStrategy traversalStrategy_;
  };

  class TraversalCommandManagement {
  public:
    TraversalCommandManagement(const std::deque< std::string >& dictionaryInfo, const std::string& commandInfo);
    NodeFunctor executeCommand();

  private:
    AVLTree< int, std::string > tree{};
    TraversalCommand command;
  };

}
#endif
