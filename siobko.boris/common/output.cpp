#include "output.h"

namespace siobko {
  void printInvalidCommandErrorMessage(std::ostream& out)
  {
    out << "<INVALID COMMAND>" << '\n';
  }

  void printEmptyErrorMessage(std::ostream& out)
  {
    out << "<EMPTY>" << '\n';
  }
}

