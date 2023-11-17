#ifndef PAIRSUMMCOUNTER_H
#define PAIRSUMMCOUNTER_H

#include <string>
#include <pair.h>

namespace pelipeychenko
{
  struct pairSummCounter
  {
    int keySum;
    std::string valueSum;

    pairSummCounter();

    void operator()(const pelipeychenko::pair< int, std::string > &pair);

    void printSumm(std::ostream &out) const;
  };
}

#endif
