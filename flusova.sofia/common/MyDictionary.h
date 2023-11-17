#ifndef MY_DICTIONARY_H
#define MY_DICTIONARY_H

#include <utility>
#include <functional>
#include <initializer_list>
#include "MyForwardList.h"

namespace flusova {
  template< typename Key, typename Value, typename Comparator = std::less< Key > >
  class MyDictionary {
  public:
    using sigLI = typename flusova::MyForwardList< std::pair< Key, Value > >::MyIterator;
    using sigLCI = typename flusova::MyForwardList< std::pair< Key, Value > >::MyConstIterator;

    MyDictionary() = default;
    MyDictionary(const std::initializer_list< std::pair< Key, Value > > newPair);
    ~MyDictionary() = default;

    void merge(const MyDictionary< Key, Value, Comparator >& source);
    void erase(const Key& key);
    template< typename ... Args >
    const std::pair< sigLI, bool > emplace(Key&& key, Args&& ... args);
    bool isEmpty() const noexcept;

    sigLI lower_bound(Key key);
    sigLI upper_bound(Key key);
    sigLI begin();
    sigLI end();
    sigLI find(const Key& key);
    sigLCI cbegin() const;
    sigLCI cend() const;
    sigLCI find(const Key& key) const;

    Value& operator[](const Key& key);

  private:
    MyForwardList< std::pair< Key, Value > > list_;
  };

  template < typename Key, typename Value, typename Comparator >
  using sigD = typename flusova::MyDictionary< Key, Value, Comparator >;

  template < typename Key, typename Value, typename Comparator >
  sigD< Key, Value, Comparator > complement(sigD< Key, Value, Comparator > dictL, sigD< Key, Value, Comparator > dictR);
  template < typename Key, typename Value, typename Comparator >
  sigD< Key, Value, Comparator > intersect(sigD< Key, Value, Comparator > dictL, sigD< Key, Value, Comparator > dictR);
  template < typename Key, typename Value, typename Comparator >
  sigD< Key, Value, Comparator > unite(sigD< Key, Value, Comparator > dictL, sigD< Key, Value, Comparator > dictR);
}

template< typename Key, typename Value, typename Comparator >
using typeLI = typename flusova::MyForwardList< std::pair < Key, Value > >::MyIterator;
template< typename Key, typename Value, typename Comparator >
using typeLCI = typename flusova::MyForwardList< std::pair < Key, Value > >::MyConstIterator;

template< typename Key, typename Value, typename Comparator >
flusova::MyDictionary< Key, Value, Comparator >::MyDictionary(const std::initializer_list< std::pair< Key, Value > > list)
{
  for (const std::pair< Key, Value > &newPair: list) {
    (*this)[newPair.first] = newPair.second;
  }
}

template< typename Key, typename Value, typename Comparator >
void flusova::MyDictionary< Key, Value, Comparator >::merge(const flusova::MyDictionary< Key, Value, Comparator >& source)
{
  typeLCI< Key, Value, Comparator > it = source.list_.cbegin();
  for (; it != source.list_.cend(); it++) {
    (*this)[(*it).first] = (*it).second;
  }
}

template< typename Key, typename Value, typename Comparator >
void flusova::MyDictionary< Key, Value, Comparator >::erase(const Key& key)
{
  typeLI< Key, Value, Comparator > prev = list_.begin();
  typeLI< Key, Value, Comparator > next = prev;
  while(prev != list_.end()) {
    next++;
    if ((*prev).first == key ) {
      list_.popFront();
      break;
    }
    else if ((*next).first == key) {
      list_.eraseAfter(prev);
      break;
    }
    else if (next == list_.end()) {
      break;
    }
    prev++;
  }
}

template< typename Key, typename Value, typename Comparator >
template< typename ... Args >
const std::pair< typename flusova::MyDictionary< Key, Value, Comparator >::sigLI, bool >
flusova::MyDictionary< Key, Value, Comparator >::emplace(Key&& key, Args&& ... args)
{
  typeLI< Key, Value, Comparator > it = lower_bound(key);
  if (it != end() && it->first == key) {
    return std::make_pair(it, false);
  }
  list_.insert(it, std::make_pair(key, Value(std::forward< Args >(args)...)));
  return std::make_pair(lower_bound(key), true);
}

template< typename Key, typename Value, typename Comparator >
bool flusova::MyDictionary< Key, Value, Comparator >::isEmpty() const noexcept
{
  return list_.isEmpty();
}

template < typename Key, typename Value, typename Comparator >
typeLI< Key, Value, Comparator > flusova::MyDictionary< Key, Value, Comparator >::lower_bound(Key key)
{
  Comparator c;
  for (auto it = list_.begin(); it != list_.end(); it++) {
    if (c(key, it->first) || it->first == key) {
      return it;
    }
  }
  return list_.end();
}

template < typename Key, typename Value, typename Comparator >
typeLI< Key, Value, Comparator > flusova::MyDictionary< Key, Value, Comparator >::upper_bound(Key key)
{
  Comparator c;
  for (auto it = list_.begin(); it != list_.end(); it++) {
    if (c(key, it->first)) {
      return it;
    }
  }
  return list_.end();
}

template< typename Key, typename Value, typename Comparator >
typeLI< Key, Value, Comparator > flusova::MyDictionary< Key, Value, Comparator >::begin()
{
  return list_.begin();
}

template< typename Key, typename Value, typename Comparator >
typeLI< Key, Value, Comparator > flusova::MyDictionary< Key, Value, Comparator >::end()
{
  return list_.end();
}

template< typename Key, typename Value, typename Comparator >
typeLI< Key, Value, Comparator > flusova::MyDictionary< Key, Value, Comparator >::find(const Key & key)
{
  typeLI< Key, Value, Comparator > it = list_.begin();
  while(it != list_.end()) {
    if ((*it).first == key) {
      return it;
    }
    it++;
  }
  it = list_.end();
  return it;
}

template< typename Key, typename Value, typename Comparator >
typeLCI< Key, Value, Comparator > flusova::MyDictionary< Key, Value, Comparator >::cbegin() const
{
  return list_.cbegin();
}

template<typename Key, typename Value, typename Comparator >
typeLCI< Key, Value, Comparator > flusova::MyDictionary< Key, Value, Comparator >::cend() const
{
  return list_.cend();
}

template< typename Key, typename Value, typename Comparator >
typeLCI< Key, Value, Comparator > flusova::MyDictionary< Key, Value, Comparator >::find(const Key& key) const
{
  typeLCI< Key, Value, Comparator > it = list_.cbegin();
  while(it != list_.cend()) {
    if ((*it).first == key) {
      return it;
    }
    it++;
  }
  it = list_.cend();
  return it;
}

template< typename Key, typename Value, typename Comparator >
Value& flusova::MyDictionary< Key, Value, Comparator >::operator[](const Key& key)
{
  Comparator c;
  std::pair< Key, Value > newPair;
  newPair.first = key;
  for (typeLI< Key, Value, Comparator > curr = this->begin(); curr != this->end(); curr++) {
    typeLI< Key, Value, Comparator > next = curr;
    next++;
    if (c((*curr).first, newPair.first)) {
      if ((next == list_.end()) || (c(newPair.first, (*next).first) )) {
        list_.insertAfter(curr, newPair);
        curr++;
        return (*curr).second;
      }
    }
    else if ((*curr).first == key) {
      return (*curr).second;
    }
  }
  list_.pushFront(newPair);
  return list_.getTop().second;
}

template< typename Key, typename Value, typename Comparator >
flusova::MyDictionary< Key, Value, Comparator > flusova::complement(flusova::MyDictionary< Key, Value, Comparator > dictL,
                                                                    flusova::MyDictionary< Key, Value, Comparator > dictR) {
  flusova::MyDictionary< Key, Value, Comparator > complemented;
  for (typeLI< Key, Value, Comparator > it = dictL.begin(); it != dictL.end(); ++it) {
    if (dictR.find((*it).first) == dictR.end()) {
      complemented[(*it).first] = (*it).second;
    }
  }
  return complemented;
}

template< typename Key, typename Value, typename Comparator >
flusova::MyDictionary< Key, Value, Comparator > flusova::intersect(flusova::MyDictionary< Key, Value, Comparator > dictL,
                                                                   flusova::MyDictionary< Key, Value, Comparator > dictR) {
  flusova::MyDictionary< Key, Value, Comparator > intersected;
  for (typeLI< Key, Value, Comparator > it = dictL.begin(); it != dictL.end(); ++it) {
    if (dictR.find((*it).first) != dictR.end()) {
      intersected[(*it).first] = (*it).second;
    }
  }
  return intersected;
}

template< typename Key, typename Value, typename Comparator >
flusova::MyDictionary< Key, Value, Comparator > flusova::unite(flusova::MyDictionary< Key, Value, Comparator > dictL,
                                                               flusova::MyDictionary< Key, Value, Comparator > dictR) {
  flusova::MyDictionary< Key, Value, Comparator > united = dictR;
  united.merge(dictL);
  return united;
}

#endif
