#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <utility>
#include <functional>
#include <cstddef>
#include <ForwardList.h>

namespace ponomarev {
  template< typename Key, typename Value, typename Compare = std::less< Key > >
  class Dictionary {
    public:
      Dictionary() = default;
      ~Dictionary() = default;

      Value &operator[](const Key &key);
      void insert(const Key &key, const Value &data);
      Value &get(const Key &key);

      typename ForwardList< std::pair< Key, Value > >::Iterator begin();
      typename ForwardList< std::pair< Key, Value > >::Iterator end();
      typename ForwardList< std::pair< Key, Value > >::ConstIterator cbegin() const;
      typename ForwardList< std::pair< Key, Value > >::ConstIterator cend() const;

      void merge(const Dictionary< Key, Value, Compare > &source);
      void erase(const Key &key);
      bool contains(const Key &key) const noexcept;
      bool isEmpty() const noexcept;
      std::size_t getSize() const noexcept;
    private:
      ForwardList< std::pair< Key, Value > > data_;
  };
  template < typename Key, typename Value, typename Compare, typename Predicate >
  std::size_t eraseIf(Dictionary< Key, Value, Compare > &dict, Predicate pred);
}
template< typename Key, typename Value, typename Compare >
Value &ponomarev::Dictionary< Key, Value, Compare >::operator[](const Key &key)
{
  Compare comp;
  typename ForwardList< std::pair< Key, Value > >::Iterator ptr = data_.begin();
  std::pair< Key, Value > newElement;
  newElement.first = key;
  for (ptr = data_.begin(); ptr != data_.end(); ptr++) {
    typename ForwardList< std::pair< Key, Value > >::Iterator next = ptr;
    next++;
    if (comp((*ptr).first, newElement.first) && (next == data_.end() || comp(newElement.first, (*next).first))) {
      data_.insertAfter(ptr, newElement);
      ptr++;
      return (*ptr).second;
    } else if ((*ptr).first == newElement.first) {
      return (*ptr).second;
    }
  }
  data_.pushFront(newElement);
  return data_.front().second;
}
template< typename Key, typename Value, typename Compare >
void ponomarev::Dictionary< Key, Value, Compare >::insert(const Key &key, const Value &data)
{
  (*this)[key] = data;
}
template< typename Key, typename Value, typename Compare >
Value &ponomarev::Dictionary< Key, Value, Compare >::get(const Key &key)
{
  return (*this)[key];
}
template< typename Key, typename Value, typename Compare >
typename ponomarev::ForwardList< std::pair< Key, Value > >::Iterator ponomarev::Dictionary< Key, Value, Compare >::begin()
{
  return data_.begin();
}
template< typename Key, typename Value, typename Compare >
typename ponomarev::ForwardList< std::pair< Key, Value > >::Iterator ponomarev::Dictionary< Key, Value, Compare >::end()
{
  return data_.end();
}
template< typename Key, typename Value, typename Compare >
typename ponomarev::ForwardList< std::pair< Key, Value > >::ConstIterator ponomarev::Dictionary< Key, Value, Compare >::cbegin() const
{
  return data_.cbegin();
}
template< typename Key, typename Value, typename Compare >
typename ponomarev::ForwardList< std::pair< Key, Value > >::ConstIterator ponomarev::Dictionary< Key, Value, Compare >::cend() const
{
  return data_.cend();
}
template< typename Key, typename Value, typename Compare >
void ponomarev::Dictionary< Key, Value, Compare >::merge(const Dictionary< Key, Value, Compare > &source)
{
  typename ForwardList< std::pair< Key, Value > >::ConstIterator ptr = source.data_.begin();
  while (ptr != source.data_.end()) {
    (*this)[(*ptr).first] = (*ptr).second;
    ptr++;
  }
}
template< typename Key, typename Value, typename Compare >
void ponomarev::Dictionary< Key, Value, Compare >::erase(const Key &key)
{
  typename ForwardList< std::pair< Key, Value > >::Iterator ptr = data_.begin();
  typename ForwardList< std::pair< Key, Value > >::Iterator next = ptr;
  while (ptr != data_.end()) {
    next++;
    if ((*ptr).first == key) {
      data_.popFront();
      break;
    } else if (next == data_.end()) {
      break;
    } else if ((*next).first == key) {
      data_.eraseAfter(ptr);
      break;
    }
    ptr = next;
  }
}
template< typename Key, typename Value, typename Compare >
bool ponomarev::Dictionary< Key, Value, Compare >::contains(const Key &key) const noexcept
{
  for (typename ForwardList< std::pair< Key, Value > >::ConstIterator ptr = data_.begin(); ptr != data_.end(); ptr++) {
    if ((*ptr).first == key) {
      return true;
    }
  }
  return false;
}
template< typename Key, typename Value, typename Compare >
bool ponomarev::Dictionary< Key, Value, Compare >::isEmpty() const noexcept
{
  return data_.isEmpty();
}
template< typename Key, typename Value, typename Compare >
std::size_t ponomarev::Dictionary< Key, Value, Compare >::getSize() const noexcept
{
  return data_.getSize();
}

template< typename Key, typename Value, typename Compare, typename Predicate >
std::size_t ponomarev::eraseIf(Dictionary< Key, Value, Compare > &dict, Predicate pred)
{
  std::size_t oldSize = dict.getSize();
  typename ForwardList< std::pair< Key, Value > >::Iterator ptr = dict.begin();
  typename ForwardList< std::pair< Key, Value > >::Iterator last = dict.end();
  while (ptr != last) {
    if (pred(*ptr)) {
      typename ForwardList< std::pair< Key, Value > >::Iterator toErase = ptr;
      ptr++;
      dict.erase((*toErase).first);
    } else {
      ptr++;
    }
  }
  return oldSize - dict.getSize();
}

#endif
