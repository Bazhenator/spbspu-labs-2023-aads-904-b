#ifndef SORTS_MANAGER_H
#define SORTS_MANAGER_H

#include <string>
#include <cstdlib>
#include <type_traits>
#include <fstream>
#include <cstddef>
#include <iostream>
#include <random>
#include <deque>
#include <list>
#include <forward_list>
#include "Sortings.h"

namespace flusova {

  template < typename Container >
  class sortingsManager {
  public:

    sortingsManager(const std::string& strategy, const std::size_t amount, Container& container);

    void fillContainerWithInts();
    void fillContainerWithFloats();
    void processingDeque();
    void processingList();
    void processingForwardList();
    template< typename Ty >
    void printStorage(Ty& container);
    void copy();

  private:

    std::string strategy_;
    std::size_t amount_;
    Container startContainer_;
    std::deque< typename Container::value_type > deque_;
    std::list< typename Container::value_type > list_;
    std::forward_list< typename Container::value_type > forwardList_;

  };
}

template < typename Container >
flusova::sortingsManager< Container >::sortingsManager(const std::string& strategy, const std::size_t amount, Container& container) :
  strategy_(strategy),
  amount_(amount),
  startContainer_(container)
{
  if (strategy_ != "ascending" && strategy_ != "descending") {
    throw std::invalid_argument("Invalid strategy was given!");
  }
}

template < typename Container >
void flusova::sortingsManager< Container >::fillContainerWithInts() {
  std::random_device rd;
  std::mt19937 generator(rd());
  std::uniform_int_distribution< typename Container::value_type > distribution(-100, 100);
  for (std::size_t i = 0; i < amount_; i++) {
    startContainer_.push_back(distribution(generator));
  }
  copy();
  printStorage(deque_);
}

template < typename Container >
void flusova::sortingsManager< Container >::fillContainerWithFloats() {
  std::random_device rd;
  std::mt19937 generator(rd());
  std::uniform_real_distribution< typename Container::value_type > distribution(-100.0, 100.0);
  for (std::size_t i = 0; i < amount_; i++) {
    startContainer_.push_back(distribution(generator));
  }
  copy();
  printStorage(deque_);
}

template < typename Container >
void flusova::sortingsManager< Container >::processingDeque() {
  std::deque< typename Container::value_type > copied;
  std::copy(deque_.begin(),
            deque_.end(),
            std::back_inserter(copied));
  std::deque< typename Container::value_type > sorted = copied;
  if (strategy_ == "ascending") {
    flusova::bubbleSort(sorted, std::less< typename Container::value_type >());
    printStorage(sorted);
    sorted = copied;
    flusova::shellSort(sorted.begin(), sorted.end(), std::less< typename Container::value_type >());
    printStorage(sorted);
    sorted = copied;
    flusova::quickSort(sorted, std::less< typename Container::value_type >());
  } else {
    flusova::bubbleSort(sorted, std::greater< typename Container::value_type >());
    printStorage(sorted);
    sorted = copied;
    flusova::shellSort(sorted.begin(), sorted.end(), std::greater< typename Container::value_type >());
    printStorage(sorted);
    sorted = copied;
    flusova::quickSort(sorted, std::greater< typename Container::value_type >());
  }
  printStorage(sorted);
}

template < typename Container >
void flusova::sortingsManager< Container >::processingList() {
  std::list< typename Container::value_type > copied;
  std::copy(list_.begin(),
            list_.end(),
            std::back_inserter(copied));
  std::list< typename Container::value_type > sorted = copied;
  if (strategy_ == "ascending") {
    flusova::bubbleSort(sorted, std::less< typename Container::value_type >());
    printStorage(sorted);
    sorted = copied;
    flusova::shellSort(sorted.begin(), sorted.end(), std::less< typename Container::value_type >());
  } else {
    flusova::bubbleSort(sorted, std::greater< typename Container::value_type >());
    printStorage(sorted);
    sorted = copied;
    flusova::shellSort(sorted.begin(), sorted.end(), std::greater< typename Container::value_type >());
  }
  printStorage(sorted);
}

template < typename Container >
void flusova::sortingsManager< Container >::processingForwardList() {
  std::forward_list< typename Container::value_type > copied;
  for (auto it = forwardList_.begin(); it != forwardList_.end(); it++) {
    copied.push_front(*it);
  }
  if (strategy_ == "ascending") {
    flusova::bubbleSort(copied, std::less< typename Container::value_type >());
  } else {
    flusova::bubbleSort(copied, std::greater< typename Container::value_type >());
  }
  printStorage(copied);
}

template < typename Container >
template < typename Ty >
void flusova::sortingsManager< Container >::printStorage(Ty& container) {
  for (auto it = container.begin(); it != container.end();) {
    std::cout << *it << (++it == container.end() ? '\n' : ' ');
  }
}

template < typename Container >
void flusova::sortingsManager< Container >::copy()
{
  typename std::vector< typename Container::value_type >::iterator it = startContainer_.begin();
  for (; it != startContainer_.end(); it++) {
    deque_.push_front(*it);
    list_.push_back(*it);
    forwardList_.push_front(*it);
  }
}

#endif
