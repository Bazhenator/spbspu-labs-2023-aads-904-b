#ifndef QUEUE_H
#define QUEUE_H
#include <cstdlib>
#include "MyForwardList.h"

namespace flusova {
  template < typename T >
  class Queue
  {
  public:
    Queue() = default;
    ~Queue() = default;
    Queue(const Queue&) = delete;
    Queue(Queue&&) = delete;

    Queue& operator=(const Queue&) = delete;
    Queue& operator=(Queue&&) = delete;

    void pushFront(const T& newHead);
    void popBack();
    bool isEmpty() const noexcept;
    std::size_t getSize() const noexcept;
    const T& getHead() const ;
    const T& getTail() const;

  private:
    MyForwardList< T > queue_;
  };
}

template < typename T >
void flusova::Queue< T >::pushFront(const T& newHead)
{
  queue_.pushFront(newHead);
}
template < typename T >
void flusova::Queue< T >::popBack()
{
  if (!isEmpty())
  {
    queue_.popBack();
  }
}
template < typename T >
bool flusova::Queue< T >::isEmpty() const noexcept
{
  return queue_.isEmpty();
}
template < typename T >
std::size_t flusova::Queue< T >::getSize() const noexcept
{
  return queue_.getSize();
}
template < typename T >
const T& flusova::Queue< T >::getHead() const
{
  return queue_.getTop();
}
template < typename T >
const T& flusova::Queue< T >::getTail() const
{
  return queue_.getBack();
}

#endif
