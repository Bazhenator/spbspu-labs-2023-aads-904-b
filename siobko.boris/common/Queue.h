
#ifndef QUEUE_H
#define QUEUE_H

#include "List.h"

namespace siobko {
  template< typename Ty >
  class Queue {
  public:
    using storage_t = List< Ty >;
    using iterator = typename storage_t::iterator;
    using const_iterator = typename storage_t::const_iterator;
    using reference = typename storage_t::reference;
    using const_reference = typename storage_t::const_reference;
    using size_type = typename storage_t::size_type;
    using value_type = typename storage_t::value_type;

    Queue() = default;
    Queue(const Queue& rhs);
    Queue(Queue&& rhs) noexcept;
    Queue(std::initializer_list< value_type > IList);
    ~Queue();

    iterator begin() noexcept;
    iterator end() noexcept;
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;

    reference front() noexcept;
    reference back() noexcept;
    const_reference front() const noexcept;
    const_reference back() const noexcept;

    void push(const Ty& obj);
    void pop();
    bool isEmpty() const noexcept;
    void clear() noexcept;
    size_type size() const noexcept;

  private:
    storage_t storage_;
  };

  template< typename Ty >
  Queue< Ty >::Queue(const Queue& rhs):
    storage_(rhs.storage_)
  {}

  template< typename Ty >
  Queue< Ty >::Queue(Queue&& rhs) noexcept :
    storage_(rhs.storage_)
  {}

  template< typename Ty >
  Queue< Ty >::Queue(std::initializer_list< value_type > IList):
    storage_(IList)
  {}

  template< typename Ty >
  Queue< Ty >::~Queue()
  {
    clear();
  }

  template< typename Ty >
  typename Queue< Ty >::iterator Queue< Ty >::begin() noexcept
  {
    return storage_.begin();
  }

  template< typename Ty >
  typename Queue< Ty >::iterator Queue< Ty >::end() noexcept
  {
    return storage_.end();
  }

  template< typename Ty >
  typename Queue< Ty >::const_iterator Queue< Ty >::begin() const noexcept
  {
    return storage_.cbegin();
  }

  template< typename Ty >
  typename Queue< Ty >::const_iterator Queue< Ty >::end() const noexcept
  {
    return storage_.cend();
  }

  template< typename Ty >
  typename Queue< Ty >::reference Queue< Ty >::front() noexcept
  {
    return storage_.front();
  }

  template< typename Ty >
  typename Queue< Ty >::const_reference Queue< Ty >::front() const noexcept
  {
    return storage_.front();
  }

  template< typename Ty >
  typename Queue< Ty >::reference Queue< Ty >::back() noexcept
  {
    return storage_.back();
  }

  template< typename Ty >
  typename Queue< Ty >::const_reference Queue< Ty >::back() const noexcept
  {
    return storage_.back();
  }

  template< typename Ty >
  typename Queue< Ty >::size_type Queue< Ty >::size() const noexcept
  {
    return storage_.size();
  }

  template< typename Ty >
  bool Queue< Ty >::isEmpty() const noexcept
  {
    return storage_.isEmpty();
  }

  template< typename Ty >
  void Queue< Ty >::push(const Ty& obj)
  {
    storage_.pushBack(obj);
  }

  template< typename Ty >
  void Queue< Ty >::pop()
  {
    storage_.popFront();
  }

  template< typename Ty >
  void Queue< Ty >::clear() noexcept
  {
    storage_.clear();
  }
}
#endif
