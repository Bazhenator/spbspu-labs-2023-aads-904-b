#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <functional>
#include <iostream>
#include <initializer_list>
#include <ForwardList.h>

namespace nemchinov
{
  template< typename Key, typename Value, typename Comparator = std::less< Key > >
  class Dictionary
  {
  public:
    using forwardlist = ForwardList< std::pair< Key, Value > >;
    using dictionary = Dictionary< Key, Value, Comparator >;

    using iterator = typename forwardlist::iterator;
    using const_iterator = typename forwardlist::const_iterator;

    Dictionary() = default;
    Dictionary(const std::initializer_list< std::pair< Key, Value > > &);

    ~Dictionary() = default;

    Value &operator[](const Key &);
    const Value &operator[](const Key &) const;
    void clear();
    void erase(iterator &);
    void erase(iterator, iterator);
    bool contains(Key) const;
    std::size_t size() const noexcept;
    bool isEmpty() const noexcept;
    iterator lower_bound(Key);
    iterator upper_bound(Key);
    template< typename ...Args >
    const std::pair< iterator, bool > emplace(Key &&, Args &&...);
    void print(std::ostream &) const;
    void complement(dictionary &, dictionary &);
    void intersect(dictionary &, dictionary &);
    void dictUnion(dictionary &, dictionary &);
    void delIf(bool (*checkFoo)(Key, dictionary &), dictionary &);

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;

  private:
    forwardlist container_;
  };

  template< typename Key, typename Value, typename Comparator >
  Dictionary< Key, Value, Comparator >::Dictionary(const std::initializer_list< std::pair< Key, Value>> &keyValuePair)
  {
    for (const std::pair< Key, Value > &pair : keyValuePair)
    {
      (*this)[pair.first] = pair.second;
    }
  }

  template< typename Key, typename Value, typename Comparator >
  Value &Dictionary< Key, Value, Comparator >::operator[](const Key &key)
  {
    auto keyInd = lower_bound(key);
    if (keyInd == container_.end())
    {
      container_.pushHead(std::make_pair(key, Value{}));
      return lower_bound(key)->second;
    } else if (keyInd->first != key)
    {
      if (keyInd == container_.begin())
      {
        container_.pushTail(std::make_pair(key, Value{}));
        return container_.begin()->second;
      } else
      {
        container_.insert(keyInd, std::make_pair(key, Value{}));
        return lower_bound(key)->second;
      }
    }
    return keyInd->second;
  }

  template< typename Key, typename Value, typename Comparator >
  const Value &Dictionary< Key, Value, Comparator >::operator[](const Key &key) const
  {
    for (auto it : container_)
    {
      if (it.first == key) return it.second;
    }
    throw std::out_of_range("Error : out of range");
  }

  template< typename Key, typename Value, typename Comparator >
  void Dictionary< Key, Value, Comparator >::clear()
  {
    erase(container_.begin(), container_.end());
  }

  template< typename Key, typename Value, typename Comparator >
  void Dictionary< Key, Value, Comparator >::erase(Dictionary::iterator &popIt)
  {
    if (popIt.itNode() == nullptr)
    {
      throw std::logic_error("Dictionary erase error: incorrect iterator");
    }
    container_.pop(popIt);
  }

  template< typename Key, typename Value, typename Comparator >
  void Dictionary< Key, Value, Comparator >::erase(Dictionary::iterator begin, Dictionary::iterator end)
  {
    if (begin.itNode() == nullptr)
    {
      if (end.itNode() == nullptr) return;
      throw std::logic_error("Dictionary erase error: incorrect begin iterator");
    }
    if (begin > end)
    {
      throw std::logic_error("Dictionary erase error: incorrect end iterator");
    }
    while (begin != end)
    {
      erase(begin);
    }
  }

  template< typename Key, typename Value, typename Comparator >
  bool Dictionary< Key, Value, Comparator >::contains(Key key) const
  {
    for (const auto &it : container_)
    {
      if (it.first == key) return true;
    }
    return false;
  }

  template< typename Key, typename Value, typename Comparator >
  std::size_t Dictionary< Key, Value, Comparator >::size() const

  noexcept
  {
    return container_.

            size();
  }

  template< typename Key, typename Value, typename Comparator >
  bool Dictionary< Key, Value, Comparator >::isEmpty() const

  noexcept
  {
    return container_.

            isEmpty();

  }

  template< typename Key, typename Value, typename Comparator >
  typename Dictionary< Key, Value, Comparator >::iterator Dictionary< Key, Value, Comparator >::lower_bound(Key key)
  {
    Comparator comp;
    for (auto it = container_.begin(); it != container_.end(); ++it)
    {
      if (comp(key, it->first) || it->first == key) return it;
    }
    return container_.end();
  }

  template< typename Key, typename Value, typename Comparator >
  typename Dictionary< Key, Value, Comparator >::iterator Dictionary< Key, Value, Comparator >::upper_bound(Key key)
  {
    Comparator comp;
    for (auto it = container_.begin(); it != container_.end(); ++it)
    {
      if (comp(key, it->first)) return it;
    }
    return container_.end();
  }

  template< typename Key, typename Value, typename Comparator >
  template< typename... Args >
  const std::pair< typename Dictionary< Key, Value, Comparator >::iterator, bool >
  Dictionary< Key, Value, Comparator >::emplace(Key &&key, Args &&... args)
  {
    iterator it = lower_bound(key);
    if (it != end() && it->first == key)
    {
      return std::make_pair(it, false);
    }
    container_.insert(it, std::make_pair(key, Value(std::forward< Args >(args)...)));
    return std::make_pair(lower_bound(key), true);
  }

  template< typename Key, typename Value, typename Comparator >
  void Dictionary< Key, Value, Comparator >::print(std::ostream &out) const
  {
    int length = this->size();
    int i = 0;
    for (const auto &it : *this)
    {
      out << it.first << " " << it.second;
      if (++i < length) std::cout << " ";
    }
    out << "\n";
  }

  template< typename Key, typename Value, typename Comparator >
  void Dictionary< Key, Value, Comparator >::delIf(bool (*checkFoo)(Key, dictionary &), dictionary &other)
  {
    bool isTail;
    for (auto it = this->begin(); it != this->end();)
    {
      isTail = false;
      if (checkFoo(it->first, other))
      {
        this->container_.pop(it);
        if (it == begin()) isTail = true;
      }
      if (!isTail) ++it;
    }
  }

  template< typename Key, typename Value, typename Comparator >
  void Dictionary< Key, Value, Comparator >::complement(dictionary &first, dictionary &second)
  {
    if (this == &first)
    {
      first.delIf([](Key key, dictionary &other)
                  { return other.contains(key); }, second);
      return;
    }
    if (this == &second)
    {
      first.delIf([](Key key, dictionary &other)
                  { return other.contains(key); }, first);
      return;
    }

    for (const auto &it : first)
    {
      if (second.contains(it.first)) continue;
      (*this)[it.first] = it.second;
    }
    for (const auto &it : second)
    {
      if (first.contains(it.first)) continue;
      (*this)[it.first] = it.second;
    }
  }

  template< typename Key, typename Value, typename Comparator >
  void Dictionary< Key, Value, Comparator >::intersect(dictionary &first, dictionary &second)
  {
    if (this == &first)
    {
      first.delIf([](Key key, dictionary &other)
                  { return !other.contains(key); }, second);
      return;
    }
    if (this == &second)
    {
      first.delIf([](Key key, dictionary &other)
                  { return !other.contains(key); }, first);
      return;
    }

    erase(this->begin(), this->end());

    for (const auto &it : first)
    {
      if (second.contains(it.first))
      {
        (*this)[it.first] = it.second;
      }
    }
  }

  template< typename Key, typename Value, typename Comparator >
  void Dictionary< Key, Value, Comparator >::dictUnion(dictionary &first, dictionary &second)
  {
    for (const auto &it : first)
    {
      (*this)[it.first] = it.second;
    }
    for (const auto &it : second)
    {
      if (!this->contains(it.first))
      {
        (*this)[it.first] = it.second;
      }
    }
  }

  template< typename Key, typename Value, typename Comparator >
  typename Dictionary< Key, Value, Comparator >::iterator Dictionary< Key, Value, Comparator >::begin()
  {
    return container_.begin();
  }

  template< typename Key, typename Value, typename Comparator >
  typename Dictionary< Key, Value, Comparator >::iterator Dictionary< Key, Value, Comparator >::end()
  {
    return container_.end();
  }

  template< typename Key, typename Value, typename Comparator >
  typename Dictionary< Key, Value, Comparator >::const_iterator Dictionary< Key, Value, Comparator >::begin() const
  {
    return container_.begin();
  }

  template< typename Key, typename Value, typename Comparator >
  typename Dictionary< Key, Value, Comparator >::const_iterator Dictionary< Key, Value, Comparator >::end() const
  {
    return container_.end();
  }
}

#endif
