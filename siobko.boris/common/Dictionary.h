
#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <iostream>

#include "ForwardList.h"

namespace siobko {
  template< typename Key, typename Value, typename Comparator = std::less< Key > >
  class Dictionary {
  public:
    using storage_t = ForwardList< std::pair< Key, Value > >;
    using iterator = typename storage_t::iterator;
    using const_iterator = typename storage_t::const_iterator;
    using value_type = typename std::pair< Key, Value >;
    using size_type = typename storage_t::size_type;

    Dictionary() = default;
    Dictionary(const Dictionary& rhs);
    Dictionary(Dictionary&& rhs) noexcept;
    Dictionary(std::initializer_list< value_type > IList);
    ~Dictionary();

    Dictionary& operator=(const Dictionary& other);
    Dictionary& operator=(Dictionary&& other) noexcept;

    iterator begin() noexcept;
    iterator end() noexcept;
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;
    const_iterator lowerBound(const Key& key) const noexcept;
    const_iterator upperBound(const Key& key) const noexcept;

    void push(const Key& k, const Value& v);
    Value get(const Key& k) const;
    bool contains(const Key& k) const noexcept;
    bool isEmpty() const noexcept;
    void merge(const Dictionary& dictionary);
    void print();
    size_type size() const noexcept;

  private:
    storage_t storage_;
    Comparator comp_;
  };

  template< typename Key, typename Value, typename Comparator >
  Dictionary< Key, Value, Comparator >::Dictionary(const Dictionary& rhs) :
    storage_(rhs.storage_)
  {}

  template< typename Key, typename Value, typename Comparator >
  Dictionary< Key, Value, Comparator >::Dictionary(Dictionary&& rhs) noexcept:
    storage_(rhs.storage_)
  {}

  template< typename Key, typename Value, typename Comparator >
  Dictionary< Key, Value, Comparator >::Dictionary(std::initializer_list< value_type > IList):
    storage_(IList)
  {}

  template< typename Key, typename Value, typename Comparator >
  Dictionary< Key, Value, Comparator >::~Dictionary()
  {
    storage_.clear();
  }

  template< typename Key, typename Value, typename Comparator >
  Dictionary< Key, Value, Comparator >& Dictionary< Key, Value, Comparator >::operator=(const Dictionary< Key, Value, Comparator >& other)
  {
    if (this != &other) {
      storage_ = other.storage_;
    }
    return *this;
  }

  template< typename Key, typename Value, typename Comparator >
  Dictionary< Key, Value, Comparator >& Dictionary< Key, Value, Comparator >::operator=(Dictionary< Key, Value, Comparator >&& other) noexcept
  {
    if (this != &other) {
      storage_ = other.storage_;
    }
    return *this;
  }

  template< typename Key, typename Value, typename Comparator >
  typename Dictionary< Key, Value, Comparator >::iterator Dictionary< Key, Value, Comparator >::begin() noexcept
  {
    return storage_.begin();
  }

  template< typename Key, typename Value, typename Comparator >
  typename Dictionary< Key, Value, Comparator >::iterator Dictionary< Key, Value, Comparator >::end() noexcept
  {
    return storage_.end();
  }

  template< typename Key, typename Value, typename Comparator >
  typename Dictionary< Key, Value, Comparator >::const_iterator
  Dictionary< Key, Value, Comparator >::begin() const noexcept
  {
    return storage_.cbegin();
  }

  template< typename Key, typename Value, typename Comparator >
  typename Dictionary< Key, Value, Comparator >::const_iterator Dictionary< Key, Value, Comparator >::end() const noexcept
  {
    return storage_.cend();
  }

  template< typename Key, typename Value, typename Comparator >
  typename Dictionary< Key, Value, Comparator >::const_iterator Dictionary< Key, Value, Comparator >::cbegin() const noexcept
  {
    return storage_.cbegin();
  }

  template< typename Key, typename Value, typename Comparator >
  typename Dictionary< Key, Value, Comparator >::const_iterator Dictionary< Key, Value, Comparator >::cend() const noexcept
  {
    return storage_.cend();
  }

  template< typename Key, typename Value, typename Comparator >
  void Dictionary< Key, Value, Comparator >::push(const Key& k, const Value& v)
  {
    std::pair< Key, Value > newObj = {k, v};

    iterator iter = begin();
    if (size() == 0 || comp_(k, iter->first)) {
      storage_.pushFront(newObj);
      return;
    }

    iterator yait = iter;
    while (iter != end()) {
      if (iter->first == k) {
        iter->second = v;
      }
      if (comp_(k, iter->first)) {
        break;
      }
      yait = iter;
      ++iter;
    }

    storage_.insertAfter(newObj, yait);
  }

  template< typename Key, typename Value, typename Comparator >
  Value Dictionary< Key, Value, Comparator >::get(const Key& k) const
  {
    for (const_iterator it = cbegin(); it != cend(); ++it) {
      if (it->first == k) {
        return it->second;
      }
    }
    throw std::logic_error("Dictionary get error: cannot get an object.");
  }

  template< typename Key, typename Value, typename Comparator >
  bool Dictionary< Key, Value, Comparator >::contains(const Key& k) const noexcept
  {
    for (const_iterator it = cbegin(); it != cend(); ++it) {
      if (it->first == k) {
        return true;
      }
    }
    return false;
  }

  template< typename Key, typename Value, typename Comparator >
  bool Dictionary< Key, Value, Comparator >::isEmpty() const noexcept
  {
    return storage_.isEmpty();
  }

  template< typename Key, typename Value, typename Comparator >
  void Dictionary< Key, Value, Comparator >::merge(const Dictionary& dictionary)
  {
    for (const_iterator it = dictionary.cbegin(); it != dictionary.cend(); ++it) {
      if (contains(it->first)) {
        continue;
      }
      push(it->first, it->second);
    }
  }

  template< typename Key, typename Value, typename Comparator >
  void Dictionary< Key, Value, Comparator >::print()
  {
    for (const_iterator it = cbegin(); it != cend(); ++it) {
      std::cout << " " << it->first << " " << it->second;
    }
    std::cout << "\n";
  }

  template< typename Key, typename Value, typename Comparator >
  typename Dictionary< Key, Value, Comparator >::const_iterator
  Dictionary< Key, Value, Comparator >::lowerBound(const Key& key) const noexcept
  {
    const_iterator it = cbegin();
    while (it != cend()) {
      if (comp_(key, (*it))) {
        return it;
      }
      ++it;
    }
    return it;
  }

  template< typename Key, typename Value, typename Comparator >
  typename Dictionary< Key, Value, Comparator >::const_iterator
  Dictionary< Key, Value, Comparator >::upperBound(const Key& key) const noexcept
  {
    const_iterator it = cbegin();
    while (it != cend()) {
      if (!comp_((*it), key)) {
        return it;
      }
      ++it;
    }
    return it;
  }

  template< typename Key, typename Value, typename Comparator >
  typename Dictionary< Key, Value, Comparator >::size_type Dictionary< Key, Value, Comparator >::size() const noexcept
  {
    return storage_.size();
  }
}
#endif
