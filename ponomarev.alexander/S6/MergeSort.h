#ifndef MERGE_SORT_H
#define MERGE_SORT_H

#include <algorithm>
#include "../common/ForwardList.h"

namespace ponomarev {
  template< typename RandomIt, typename Compare >
  void mergeSort(RandomIt first, RandomIt last, Compare comp);
  template< typename RandomIt, typename Compare >
  void merge(RandomIt first, RandomIt mid, RandomIt last, Compare comp);
}
template< typename RandomIt, typename Compare >
void ponomarev::mergeSort(RandomIt first, RandomIt last, Compare comp)
{
  RandomIt left = first;
  left++;
  if (left == last) {
    return;
  }

  RandomIt mid = first + (last - first) / 2;
  mergeSort(first, mid, comp);
  mergeSort(mid, last, comp);
  merge(first, mid, last, comp);
}
template< typename RandomIt, typename Compare >
void ponomarev::merge(RandomIt first, RandomIt mid, RandomIt last, Compare comp)
{
  ForwardList< typename RandomIt::value_type > result;
  RandomIt leftL = first;
  RandomIt leftR = mid;
  while (leftL != mid || leftR != last) {
    if (leftL == mid) {
      result.pushBack(*leftR);
      leftR++;
      continue;
    }
    if (leftR == last) {
      result.pushBack(*leftL);
      leftL++;
      continue;
    }

    if (comp(*leftL, *leftR)) {
      result.pushBack(*leftL);
      leftL++;
    } else {
      result.pushBack(*leftR);
      leftR++;
    }
  }

  std::copy(result.begin(), result.end(), first);
}

#endif
