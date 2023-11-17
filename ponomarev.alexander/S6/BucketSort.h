#ifndef BUCKET_SORT_H
#define BUCKET_SORT_H

#include <algorithm>
#include <cmath>
#include <vector>
#include "../common/ForwardList.h"
#include "SelectionSort.h"

namespace ponomarev {
  template< typename Iterator, typename Compare >
  void bucketSort(Iterator first, Iterator last,
    typename Iterator::value_type min, typename Iterator::value_type max,
    std::size_t nBuckets, Compare comp);
}
template< typename Iterator, typename Compare >
void ponomarev::bucketSort(Iterator first, Iterator last,
  typename Iterator::value_type min, typename Iterator::value_type max,
  std::size_t nBuckets, Compare comp)
{
  double range = std::floor(max - min + 1);
  std::vector< ForwardList< double > > buckets(nBuckets);
  for (Iterator it = first; it != last; it++) {
    int i = std::floor(nBuckets * (*it - min) / range);
    buckets[i].pushBack(*it);
  }
  for (auto it = buckets.begin(); it != buckets.end(); it++) {
    selectionSort(it->begin(), it->end(), comp);
  }
  if (comp(min, max)) {
    for (auto it = buckets.begin(); it != buckets.end(); it++) {
      for (auto bIt = it->begin(); bIt != it->end(); bIt++, first++) {
        *first = *bIt;
      }
    }
  } else {
    for (auto it = buckets.rbegin(); it != buckets.rend(); it++) {
      for (auto bIt = it->begin(); bIt != it->end(); bIt++, first++) {
        *first = *bIt;
      }
    }
  }
}

#endif
