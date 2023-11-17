#ifndef QUEUE_H
#define QUEUE_H

#include "ForwardList.h"

namespace nemchinov
{
  template < typename T >
  class Queue
  {
  public:
    Queue() = default;
    ~Queue() = default;

    Queue(const Queue &) = default;
    Queue &operator=(const Queue &) = delete;

    Queue(Queue &&) = default;
    Queue &operator=(Queue &&) = delete;

    void push(const T &);
    bool isEmpty() const noexcept;
    const T &get() const;
    void pop();

  private:
    ForwardList< T > list_;
  };

  template < typename T >
  void Queue< T >::push(const T &data)
  {
    Queue< T >::list_.pushHead(data);
  }

  template < typename T >
  bool Queue< T >::isEmpty() const noexcept
  {
    return Queue< T >::list_.isEmpty();
  }

  template < typename T >
  const T &Queue< T >::get() const
  {
    try
    {
      return Queue< T >::list_.getTail();
    }
    catch (...)
    {
      throw std::logic_error("ERROR : queue is empty");
    }
  }

  template < typename T >
  void Queue< T >::pop()
  {
    try
    {
      Queue< T >::list_.popTail();
    }
    catch (...)
    {
      throw std::logic_error("ERROR : queue is empty");
    }
  }
}

#endif
