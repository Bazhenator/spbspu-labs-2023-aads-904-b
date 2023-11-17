#ifndef STACK_H
#define STACK_H

#include "ForwardList.h"

namespace nemchinov
{
  template < typename T >
  class Stack
  {
  public:
    Stack() = default;
    ~Stack() = default;

    Stack(const Stack &) = default;
    Stack &operator=(const Stack &) = delete;

    Stack(Stack &&) = default;
    Stack &operator=(Stack &&) = delete;

    void push(const T &);
    bool isEmpty() const noexcept;
    const T &get() const;
    void pop();

  private:
    ForwardList <T> list_;
  };

  template < typename T >
  void Stack< T >::push(const T &data)
  {
    Stack< T >::list_.pushTail(data);
  }

  template < typename T >
  bool Stack< T >::isEmpty() const noexcept
  {
    return Stack< T >::list_.isEmpty();
  }

  template < typename T >
  const T &Stack< T >::get() const
  {
    try
    {
      return Stack< T >::list_.getTail();
    }
    catch (...)
    {
      throw std::logic_error("ERROR : stack is empty");
    }
  }

  template < typename T >
  void Stack< T >::pop()
  {
    try
    {
      Stack< T >::list_.popTail();
    }
    catch (...)
    {
      throw std::logic_error("ERROR : stack is empty");
    }
  }
}

#endif
