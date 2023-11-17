#ifndef STACK_H
#define STACK_H

#include "ForwardIterators.h"

namespace pelipeychenko
{
  template< typename T >
  class Stack
  {
  public:

    Stack() = default;

    void push(const T &instance);

    void drop();

    bool isEmpty() const noexcept;

    const T &getHead() const;

  private:
    pelipeychenko::ForwardList< T > list_;
  };

  template< typename T >
  void Stack< T >::push(const T &instance)
  {
    list_.pushFront(instance);
  }

  template< typename T >
  void Stack< T >::drop()
  {
    list_.dropFront();
  }

  template< typename T >
  bool Stack< T >::isEmpty() const noexcept
  {
    return list_.isEmpty();
  }

  template< typename T >
  const T &Stack< T >::getHead() const
  {
    return list_.getHead();
  }
}

#endif
