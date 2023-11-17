#ifndef QUEUE_H
#define QUEUE_H

#include "ForwardIterators.h"

namespace pelipeychenko
{
  template< typename T >
  class Queue
  {
  public:

    Queue() = default;

    void push(const T &instance);

    void drop();

    bool isEmpty() const noexcept;

    const T &getHead() const;

    std::size_t getSize() const noexcept;

  private:
    pelipeychenko::ForwardList< T > list_;
  };

  template< typename T >
  void Queue< T >::push(const T &instance)
  {
    list_.pushBack(instance);
  }

  template< typename T >
  void Queue< T >::drop()
  {
    list_.dropFront();
  }

  template< typename T >
  bool Queue< T >::isEmpty() const noexcept
  {
    return list_.isEmpty();
  }

  template< typename T >
  const T &Queue< T >::getHead() const
  {
    return list_.getHead();
  }

  template< typename T >
  std::size_t Queue< T >::getSize() const noexcept
  {
    return list_.getSize();
  }
}

#endif
