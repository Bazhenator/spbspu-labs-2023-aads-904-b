#include "pairSummCounter.h"

namespace pelipeychenko
{
  pairSummCounter::pairSummCounter():
    keySum(0)
  {}

  void pairSummCounter::operator()(const pelipeychenko::pair< int, std::string > &pair)
  {
    keySum += pair.first_;
    valueSum += " " + pair.second_;
  }

  void pairSummCounter::printSumm(std::ostream &out) const
  {
    out << keySum << valueSum << "\n";
  }
}
