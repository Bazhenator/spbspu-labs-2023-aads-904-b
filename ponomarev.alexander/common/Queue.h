#ifndef QUEUE_H
#define QUEUE_H

#include <cstddef>
#include "ForwardList.h"

namespace ponomarev {
  template< typename T >
  class Queue {
    public:
      Queue() = default;

      void push(const T &obj);
      void pop();
      bool isEmpty() const noexcept;
      std::size_t getSize() const noexcept;
      T &getLast();
      T &getNext();
      const T &getLast() const;
      const T &getNext() const;
    private:
      ForwardList< T > data;
  };
}

template < typename T >
void ponomarev::Queue< T >::push(const T &obj)
{
  data.pushBack(obj);
}
template < typename T >
void ponomarev::Queue< T >::pop()
{
  data.popFront();
}
template < typename T >
bool ponomarev::Queue< T >::isEmpty() const noexcept
{
  return data.isEmpty();
}
template < typename T >
std::size_t ponomarev::Queue< T >::getSize() const noexcept
{
  return data.getSize();
}
template < typename T >
T &ponomarev::Queue< T >::getLast()
{
  return data.back();
}
template < typename T >
T &ponomarev::Queue< T >::getNext()
{
  return data.front();
}
template < typename T >
const T &ponomarev::Queue< T >::getLast() const
{
  return data.back();
}
template < typename T >
const T &ponomarev::Queue< T >::getNext() const
{
  return data.front();
}

#endif
