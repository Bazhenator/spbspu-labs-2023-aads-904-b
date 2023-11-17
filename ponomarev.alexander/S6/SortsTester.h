#ifndef TEST_SORTS_H
#define TEST_SORTS_H

#include <vector>
#include <iosfwd>
#include <list>
#include <deque>
#include <functional>
#include <algorithm>
#include <iterator>
#include <ForwardList.h>
#include "SelectionSort.h"
#include "BucketSort.h"
#include "MergeSort.h"

namespace ponomarev {
  template< typename T >
  class SortsTester {
    public:
      SortsTester(std::ostream &os, std::vector< T > validationSet);

      void setOrder(std::string sortingOrder);

      void applyOnDeque();
      void applyOnList();
      void applyOnForwardList();
    private:
      std::vector< T > validationSet_;
      std::ostream &os_;
      std::function< bool(T, T) > comparator_;

      template< typename Container >
      void print(Container cont);
  };
}
template< typename T >
ponomarev::SortsTester< T >::SortsTester(std::ostream &os, std::vector< T > validationSet):
  validationSet_(validationSet),
  os_(os),
  comparator_(std::less< T >())
{
  print(validationSet_);
}
template< typename T >
void ponomarev::SortsTester< T >::setOrder(std::string sortingOrder)
{
  if (!sortingOrder.compare("ascending")) {
    comparator_ = std::less< T >();
  } else if (!sortingOrder.compare("descending")) {
    comparator_ = std::greater< T >();
  } else {
    throw std::invalid_argument("Unsupported comparison type.");
  }
}
template< typename T >
void ponomarev::SortsTester< T >::applyOnDeque()
{
  std::deque< T > copied;
  std::copy(validationSet_.begin(),
    validationSet_.end(),
    std::back_inserter(copied));

  std::deque< T > temp = copied;
  selectionSort(temp.begin(), temp.end(), comparator_);
  print(temp);

  temp = copied;
  bucketSort(temp.begin(), temp.end(),
    *std::min_element(temp.begin(), temp.end()),
    *std::max_element(temp.begin(), temp.end()),
    temp.size(),
    comparator_);
  print(temp);

  temp = copied;
  mergeSort(temp.begin(), temp.end(), comparator_);
  print(temp);
}
template< typename T >
void ponomarev::SortsTester< T >::applyOnList()
{
  std::list< T > copied;
  std::copy(validationSet_.begin(),
    validationSet_.end(),
    std::back_inserter(copied));

  std::list< T > temp = copied;
  selectionSort(temp.begin(), temp.end(), comparator_);
  print(temp);

  temp = copied;
  bucketSort(temp.begin(), temp.end(),
    *std::min_element(temp.begin(), temp.end()),
    *std::max_element(temp.begin(), temp.end()),
    temp.size(),
    comparator_);
  print(temp);
}
template< typename T >
void ponomarev::SortsTester< T >::applyOnForwardList()
{
  ForwardList< T > copied;
  for (auto it = validationSet_.begin(); it != validationSet_.end(); it++) {
    copied.pushBack(*it);
  }

  ForwardList< T > temp = copied;
  selectionSort(temp.begin(), temp.end(), comparator_);
  print(temp);
}
template< typename T >
template< typename Container >
void ponomarev::SortsTester< T >::print(Container cont)
{
  for (auto it = cont.begin(); it != cont.end();) {
    os_ << *it << (++it == cont.end() ? '\n' : ' ');
  }
}

#endif
