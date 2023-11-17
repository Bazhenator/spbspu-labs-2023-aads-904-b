#include "TraversalCommandManagement.h"

namespace siobko {
  TraversalCommand::TraversalCommand(const std::string& traversalStrategy)
  {
    if (traversalStrategy == "breadth") {
      traversalStrategy_ = TraversalStrategy::BREADTH;
    } else if (traversalStrategy == "ascending") {
      traversalStrategy_ = TraversalStrategy::ASCENDING;
    } else if (traversalStrategy == "descending") {
      traversalStrategy_ = TraversalStrategy::DESCENDING;
    } else {
      throw std::logic_error("error");
    }
  }

  NodeFunctor TraversalCommand::execute(const AVLTree< int, std::string >& tree)
  {
    return tree.traverse(NodeFunctor(), traversalStrategy_);
  }

  TraversalCommandManagement::TraversalCommandManagement(const std::deque< std::string >& dictionaryInfo, const std::string& commandInfo) :
    command(TraversalCommand(commandInfo))
  {

    for (size_t i = 0; i < dictionaryInfo.size() - 1; i += 2) {
      tree.push(std::stoi(dictionaryInfo[i]), dictionaryInfo[i + 1]);
    }
  }

  NodeFunctor TraversalCommandManagement::executeCommand()
  {
    return command.execute(tree);
  }
}
