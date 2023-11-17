#ifndef SELECTION_SORT_H
#define SELECTION_SORT_H

#include <algorithm>

namespace ponomarev {
  template< typename Iterator, typename Compare >
  void selectionSort(Iterator first, Iterator last, Compare comp);
}
template< typename Iterator, typename Compare >
void ponomarev::selectionSort(Iterator first, Iterator last, Compare comp)
{
  for (Iterator current = first; current != last; current++) {
    Iterator next = current;
    next++;
    if (next == last) {
      return;
    }

    Iterator min = current;
    for (; next != last; next++) {
      if (comp(*next, *min)) {
        min = next;
      }
    }

    if (*min != *current) {
      std::swap(*min, *current);
    }
  }
}

#endif
