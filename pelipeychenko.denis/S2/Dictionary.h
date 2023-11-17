#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <ForwardList.h>
#include <ForwardIterators.h>
#include <pair.h>

namespace pelipeychenko
{
  template< typename Key, typename Value, typename Comparator = std::less< Key > >
  class Dictionary
  {
  public:

    using ConstIterator = typename pelipeychenko::ForwardList< pelipeychenko::pair< Key, Value > >::ConstIterator;

    using Iterator = typename pelipeychenko::ForwardList< pelipeychenko::pair< Key, Value > >::Iterator;

    Dictionary() = default;

    explicit Dictionary(Dictionary< Key, Value > &&rhs) noexcept;

    Dictionary(const Dictionary< Key, Value > &rhs);

    explicit Dictionary(const ForwardList< pair< Key, Value > > &rhs);

    Dictionary(std::initializer_list< pelipeychenko::pair< Key, Value > >);

    ~Dictionary() = default;

    Iterator begin() noexcept;

    Iterator end() noexcept;

    ConstIterator cbegin() const noexcept;

    ConstIterator cend() const noexcept;

    Iterator lowerBound(Iterator first, Iterator second, const Key &instance) const noexcept;

    Iterator upperBound(Iterator first, Iterator second, const Key &instance) const noexcept;

    void merge(const pair< Key, Value > &);

    void merge(const Key &key, const Value &value);

    template< typename ...Args >
    void emplace(Args &&... args);

    ConstIterator find(const Key &key) const;

    Iterator find(const Key &key);

    Value get(const Key &key) const;

    void erase(const Key &key);

    void inlinePrint() const;

    ForwardList< Key > getKeys() const;

    ForwardList< Value > getValues() const;

    bool isEmpty() const noexcept;

    bool operator==(const Dictionary< Key, Value, Comparator > &instance);

    bool operator!=(const Dictionary< Key, Value, Comparator > &instance);

  private:
    ForwardList< pair< Key, Value > > data_;
    ForwardList< Key > keys_;
    ForwardList< Value > values_;

  };

  template< typename Key, typename Value, typename Comparator >
  Dictionary< Key, Value, Comparator >::Dictionary(std::initializer_list< pelipeychenko::pair< Key, Value > > list)
  {
    for (auto i: list) {
      merge(i);
    }
  }

  template< typename Key, typename Value, typename Comparator >
  Dictionary< Key, Value, Comparator >::Dictionary(const Dictionary< Key, Value > &rhs):
    data_(rhs.data_),
    keys_(rhs.keys_),
    values_(rhs.values_)
  {}

  template< typename Key, typename Value, typename Comparator >
  Dictionary< Key, Value, Comparator >::Dictionary(Dictionary< Key, Value > &&rhs) noexcept :
    data_(std::move(rhs.data_)),
    keys_(std::move(rhs.keys_)),
    values_(std::move(rhs.values_))
  {}

  template< typename Key, typename Value, typename Comparator >
  Dictionary< Key, Value, Comparator >::Dictionary(const ForwardList< pair< Key, Value > > &rhs) :
    data_(rhs)
  {
    for (auto iterator = rhs.cbegin(); iterator != rhs.cend(); iterator++) {
      keys_.pushBack(iterator->first_);
      values_.pushBack(iterator->second_);
    }
  }

  template< typename Key, typename Value, typename Comparator >
  void Dictionary< Key, Value, Comparator >::merge(const pair< Key, Value > &newPair)
  {
    Key key = newPair.first_;
    Value value = newPair.second_;
    if (!isEmpty() && find(key) != end()) {
      erase(key);
    }
    keys_.pushBack(key);
    values_.pushBack(value);
    if (data_.isEmpty()) {
      data_.pushBack(newPair);
      return;
    }
    Comparator comparator;
    Iterator iterator = data_.begin();
    if (!comparator(iterator->first_, newPair.first_)) {
      data_.pushFront(newPair);
      return;
    }
    for (; iterator + 1 != data_.end(); iterator++) {
      if (!comparator((iterator + 1)->first_, newPair.first_)) {
        data_.emplace(iterator, key, value);
        return;
      }
    }
    data_.pushBack(newPair);
  }

  template< typename Key, typename Value, typename Comparator >
  void Dictionary< Key, Value, Comparator >::merge(const Key &key, const Value &value)
  {
    merge({key, value});
  }

  template< typename Key, typename Value, typename Comparator >
  template< typename... Args >
  void Dictionary< Key, Value, Comparator >::emplace(Args &&... args)
  {
    merge(Key(args...), Value(args...));
  }

  template< typename Key, typename Value, typename Comparator >
  Value Dictionary< Key, Value, Comparator >::get(const Key &key) const
  {
    for (auto iterator = data_.cbegin(); iterator != data_.cend(); iterator++) {
      if (iterator->first_ == key) {
        return iterator->second_;
      }
    }
    throw std::runtime_error("Error in get(): Such key doesn't exist");
  }

  template< typename Key, typename Value, typename Comparator >
  void Dictionary< Key, Value, Comparator >::erase(const Key &key)
  {
    keys_.remove(key);
    values_.remove(get(key));
    Iterator iterator = data_.begin();
    if (iterator->first_ == key) {
      data_.dropFront();
      return;
    }
    while (iterator != data_.end() && (iterator + 1)->first_ < key) {
      iterator++;
    }
    if (iterator == data_.end() || (iterator + 1)->first_ > key) {
      throw std::runtime_error("Error in erase(): Such key doesn't exist");
    }
    data_.erase(iterator);
  }

  template< typename Key, typename Value, typename Comparator >
  void Dictionary< Key, Value, Comparator >::inlinePrint() const
  {
    data_.printElements();
  }

  template< typename Key, typename Value, typename Comparator >
  ForwardList< Key > Dictionary< Key, Value, Comparator >::getKeys() const
  {
    return keys_;
  }

  template< typename Key, typename Value, typename Comparator >
  ForwardList< Value > Dictionary< Key, Value, Comparator >::getValues() const
  {
    return values_;
  }

  template< typename Key, typename Value, typename Comparator >
  typename Dictionary< Key, Value, Comparator >::Iterator Dictionary< Key, Value, Comparator >::begin() noexcept
  {
    return data_.begin();
  }

  template< typename Key, typename Value, typename Comparator >
  typename Dictionary< Key, Value, Comparator >::Iterator Dictionary< Key, Value, Comparator >::end() noexcept
  {
    return data_.end();
  }

  template< typename Key, typename Value, typename Comparator >
  typename Dictionary< Key, Value, Comparator >::ConstIterator
  Dictionary< Key, Value, Comparator >::cbegin() const noexcept
  {
    return data_.cbegin();
  }

  template< typename Key, typename Value, typename Comparator >
  typename Dictionary< Key, Value, Comparator >::ConstIterator
  Dictionary< Key, Value, Comparator >::cend() const noexcept
  {
    return data_.cend();
  }

  template< typename Key, typename Value, typename Comparator >
  bool Dictionary< Key, Value, Comparator >::isEmpty() const noexcept
  {
    return data_.isEmpty();
  }

  template< typename Key, typename Value, typename Comparator >
  bool Dictionary< Key, Value, Comparator >::operator==(const Dictionary< Key, Value, Comparator > &instance)
  {
    return data_ == instance.data_;
  }

  template< typename Key, typename Value, typename Comparator >
  bool Dictionary< Key, Value, Comparator >::operator!=(const Dictionary< Key, Value, Comparator > &instance)
  {
    return data_ != instance.data_;
  }

  template< typename Key, typename Value, typename Comparator >
  typename Dictionary< Key, Value, Comparator >::Iterator
  Dictionary< Key, Value, Comparator >::lowerBound(Iterator first, Iterator second, const Key &instance) const noexcept
  {
    for (auto iterator = first; iterator != second; iterator++) {
      if (*iterator > instance) {
        return iterator;
      }
    }
    return end();
  }

  template< typename Key, typename Value, typename Comparator >
  typename Dictionary< Key, Value, Comparator >::Iterator
  Dictionary< Key, Value, Comparator >::upperBound(Iterator first, Iterator second, const Key &instance) const noexcept
  {
    for (auto iterator = first; iterator != second; iterator++) {
      if (*iterator >= instance) {
        return iterator;
      }
    }
    return end();
  }

  template< typename Key, typename Value, typename Comparator >
  typename Dictionary< Key, Value, Comparator >::ConstIterator
  Dictionary< Key, Value, Comparator >::find(const Key &key) const
  {
    for (auto it = cbegin(); it != cend(); it++) {
      if (it->first_ == key) {
        return it;
      }
    }
    return cend();
  }

  template< typename Key, typename Value, typename Comparator >
  typename Dictionary< Key, Value, Comparator >::Iterator
  Dictionary< Key, Value, Comparator >::find(const Key &key)
  {
    for (auto it = begin(); it != end(); it++) {
      if (it->first_ == key) {
        return it;
      }
    }
    return end();
  }
}

#endif
