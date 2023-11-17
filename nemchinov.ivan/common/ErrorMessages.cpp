#include "ErrorMessages.h"

namespace nemchinov
{
  void printErrorCommand(std::ostream &out)
  {
    out << "<INVALID COMMAND>" << "\n";
  }

  void printEmptyDict(std::ostream &out)
  {
    out << "<EMPTY>" << "\n";
  }
}
