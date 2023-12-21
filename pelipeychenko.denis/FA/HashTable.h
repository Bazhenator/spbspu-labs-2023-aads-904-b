#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string>
#include <ForwardIterators.h>
#include <pair.h>

namespace pelipeychenko
{
  template< typename Value >
  class HashTable
  {
  public:
    HashTable();

    HashTable(const HashTable &instance);

    HashTable(HashTable &&instance) noexcept;

    HashTable(std::initializer_list< pair< std::string, Value > >);

    ~HashTable();

    HashTable &operator=(const HashTable< Value > &instance);

    HashTable &operator=(HashTable< Value > &&instance) noexcept;

    void merge(const std::string &key, const Value &value);

    void merge(const HashTable &instance);

    template< typename...Args >
    void emplace(const std::string &key, Args &&...args);

    const Value &get(const std::string &key) const;

    Value &get(const std::string &key);

    void erase(const std::string &key);

    bool find(const std::string &key) const;

    size_t getSize() const noexcept;

    const ForwardList< std::string > &getKeys() const;

    bool isEmpty() const noexcept;

    void clear();

  private:
    size_t capacity_;
    size_t size_;
    size_t occupancy_;

    ForwardList< pair< std::string, Value > > *data_;

    ForwardList< std::string > keys_;

    void eraseFromData(const std::string &key);

    void swap(HashTable &instance);

    void expand();

    size_t getIndex(const std::string &key) const;

    static size_t getHash(const std::string &key);
  };

  template< typename Value >
  HashTable< Value >::HashTable() :
    capacity_(64),
    size_(0),
    occupancy_(0),
    data_(new ForwardList< pair< std::string, Value > >[capacity_])
  {}

  template< typename Value >
  HashTable< Value >::HashTable(const HashTable &instance) :
    capacity_(instance.capacity_),
    size_(0),
    occupancy_(0),
    data_(new ForwardList< pair< std::string, Value > >[capacity_])
  {
    merge(instance);
  }

  template< typename Value >
  HashTable< Value >::HashTable(HashTable &&instance) noexcept :
    capacity_(instance.capacity_),
    size_(instance.size_),
    occupancy_(instance.occupancy_),
    data_(instance.data_)
  {
    instance.capacity_ = 0;
    instance.size_ = 0;
    instance.occupancy_ = 0;
    instance.data_ = nullptr;
  }

  template< typename Value >
  HashTable< Value >::HashTable(std::initializer_list< pair< std::string, Value > > list) :
    capacity_(64),
    size_(0),
    occupancy_(0),
    data_(new ForwardList< pair< std::string, Value > >[capacity_])
  {
    for (auto it: list) {
      merge(it.first_, it.second_);
    }
  }

  template< typename Value >
  HashTable< Value >::~HashTable()
  {
    clear();
    delete[] data_;
  }

  template< typename Value >
  HashTable< Value > &HashTable< Value >::operator=(const HashTable< Value > &instance)
  {
    if (this != &instance) {
      HashTable< Value > temp(instance);
      swap(temp);
    }
    return *this;
  }

  template< typename Value >
  HashTable< Value > &HashTable< Value >::operator=(HashTable< Value > &&instance) noexcept
  {
    if (this != &instance) {
      swap(instance);
    }
    return *this;
  }

  template< typename Value >
  void HashTable< Value >::merge(const std::string &key, const Value &value)
  {
    keys_.pushBack(key);
    size_t index = getIndex(key);
    size_++;
    if (data_[index].isEmpty()) {
      occupancy_++;
    }
    data_[index].pushBack({key, value});
    if (occupancy_ * 2 >= capacity_) {
      expand();
    }
  }

  template< typename Value >
  void HashTable< Value >::merge(const HashTable &instance)
  {
    for (auto key = instance.keys_.cbegin(); key != instance.keys_.cend(); key++) {
      merge(*key, instance.get(*key));
    }
  }

  template< typename Value >
  template< typename...Args >
  void HashTable< Value >::emplace(const std::string &key, Args &&... args)
  {
    merge(key, Value(args...));
  }

  template< typename Value >
  void HashTable< Value >::erase(const std::string &key)
  {
    eraseFromData(key);
    keys_.remove(key);
  }

  template< typename Value >
  void HashTable< Value >::eraseFromData(const std::string &key)
  {
    size_t index = getIndex(key);
    data_[index].remove({key, get(key)});
    size_--;
    if (data_[index].isEmpty()) {
      occupancy_--;
    }
  }

  template< typename Value >
  const Value &HashTable< Value >::get(const std::string &key) const
  {
    if (!find(key)) {
      throw std::runtime_error("HashTable Error: such key doesn't exist");
    }

    size_t index = getIndex(key);
    auto it = data_[index].begin();
    while (it->first_ != key && it != data_[index].end()) {
      it++;
    }
    if (it->first_ == key) {
      return it->second_;
    }
    throw std::runtime_error("HashTable Error: can't find the key");
  }

  template< typename Value >
  Value &HashTable< Value >::get(const std::string &key)
  {
    if (!find(key)) {
      throw std::runtime_error("HashTable Error: such key doesn't exist");
    }

    size_t index = getIndex(key);
    auto it = data_[index].begin();
    while (it->first_ != key && it != data_[index].end()) {
      it++;
    }
    if (it->first_ == key) {
      return it->getSecond();
    }
    throw std::runtime_error("HashTable Error: can't find the key. Problem is probably in with map merge");
  }

  template< typename Value >
  bool HashTable< Value >::find(const std::string &key) const
  {
    size_t index = getIndex(key);
    if (data_[index].isEmpty()) {
      return false;
    }
    for (auto it: data_[index]) {
      if (it.first_ == key) {
        return true;
      }
    }
    return false;
  }

  template< typename Value >
  size_t HashTable< Value >::getSize() const noexcept
  {
    return size_;
  }

  template< typename Value >
  const ForwardList< std::string > &HashTable< Value >::getKeys() const
  {
    return keys_;
  }

  template< typename Value >
  bool HashTable< Value >::isEmpty() const noexcept
  {
    return (size_ == 0);
  }

  template< typename Value >
  void HashTable< Value >::clear()
  {
    for (auto key = keys_.cbegin(); key != keys_.cend() && size_ != 0; key++) {
      eraseFromData(*key);
    }
    keys_.clear();
  }

  template< typename Value >
  size_t HashTable< Value >::getIndex(const std::string &key) const
  {
    return getHash(key) % capacity_;
  }

  template< typename Value >
  size_t HashTable< Value >::getHash(const std::string &key)
  {
    size_t hash = 0;
    for (char i: key) {
      hash = (hash << 5) - hash + i;
    }
    return hash;
  }

  template< typename Value >
  void HashTable< Value >::swap(HashTable &instance)
  {
    std::swap(capacity_, instance.capacity_);
    std::swap(size_, instance.size_);
    std::swap(occupancy_, instance.occupancy_);
    std::swap(data_, instance.data_);
    keys_.swap(instance.keys_);
  }

  template< typename Value >
  void HashTable< Value >::expand()
  {
    auto oldData = data_;
    size_t oldCapacity = capacity_;
    size_ = 0;
    occupancy_ = 0;
    capacity_ *= 2;
    keys_.clear();
    data_ = new ForwardList< pair< std::string, Value > >[capacity_];
    for (size_t index = 0; index < oldCapacity; index++) {
      if (!oldData[index].isEmpty()) {
        for (auto pair: oldData[index]) {
          merge(pair.first_, pair.second_);
        }
      }
    }
  }
}

#endif
