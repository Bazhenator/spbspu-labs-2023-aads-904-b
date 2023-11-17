#ifndef SORTS_H
#define SORTS_H

#include <vector>
#include <algorithm>

namespace flusova {

  template < typename Container, typename Comparator >
  void bubbleSort(Container& container, Comparator c);

  template < typename Iterator, typename Comparator >
  void shellSort(Iterator begin, Iterator end, Comparator c);

  template < typename Container, typename Comparator >
  void quickSort(Container& container, Comparator c);

}

template < typename Container, typename Comparator >
void flusova::bubbleSort(Container& container, Comparator c) {
  std::vector< typename Container::value_type > temp(container.begin(), container.end());
  bool swapped = true;
  while (swapped) {
    swapped = false;
    for (size_t i = 0; i < temp.size() - 1; ++i) {
      if (c(temp[i + 1], temp[i])) {
        std::swap(temp[i], temp[i + 1]);
        swapped = true;
      }
    }
  }
  std::copy(temp.begin(), temp.end(), container.begin());
}

template < typename Iterator, typename Comparator >
void flusova::shellSort(Iterator begin, Iterator end, Comparator c) {
  int n = std::distance(begin, end);
  int gap = n / 2;
  while (gap > 0) {
    for (Iterator i = std::next(begin, gap); i != end; ++i) {
      auto temp = std::move(*i);
      Iterator j = i;
      Iterator prevJ = j;
      std::advance(prevJ, -gap);
      while (j != begin && c(temp, *prevJ)) {
        *j = std::move(*prevJ);
        j = prevJ;
        if (std::distance(begin, j) < gap) break;
        prevJ = std::prev(j, gap);
      }
      *j = std::move(temp);
    }
    gap /= 2;
  }
}

template < typename Container, typename Comparator >
void flusova::quickSort(Container& container, Comparator c) {
  if (container.size() < 2) {
    return;
  }
  typename Container::value_type pivot = container.back();
  container.pop_back();
  Container lesser;
  Container greater;
  while (!container.empty()) {
    if (c(container.front(), pivot)) {
      lesser.push_back(container.front());
    } else {
      greater.push_back(container.front());
    }
    container.pop_front();
  }
  quickSort(lesser, c);
  quickSort(greater, c);
  container.insert(container.end(), lesser.begin(), lesser.end());
  container.push_back(pivot);
  container.insert(container.end(), greater.begin(), greater.end());
}

#endif
