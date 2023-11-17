#include "outputLiterals.h"

void flusova::printEmpty(std::ostream& out)
{
  out << "<EMPTY>" << '\n';
}

void flusova::printInvalidCmd(std::ostream& out)
{
  out << "<INVALID COMMAND>" << '\n';
}
