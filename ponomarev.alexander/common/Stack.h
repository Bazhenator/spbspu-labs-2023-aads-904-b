#ifndef STACK_H
#define STACK_H

#include <cstddef>
#include "ForwardList.h"

namespace ponomarev {
  template< typename T >
  class Stack {
    public:
      Stack() = default;

      void push(const T &obj);
      void pop();
      bool isEmpty() const noexcept;
      std::size_t getSize() const noexcept;
      T &getTop();
      const T &getTop() const;
    private:
      ForwardList< T > data;
  };
}

template < typename T >
void ponomarev::Stack< T >::push(const T &obj)
{
  data.pushFront(obj);
}
template < typename T >
void ponomarev::Stack< T >::pop()
{
  data.popFront();
}
template < typename T >
bool ponomarev::Stack< T >::isEmpty() const noexcept
{
  return data.isEmpty();
}
template < typename T >
std::size_t ponomarev::Stack< T >::getSize() const noexcept
{
  return data.getSize();
}
template < typename T >
T &ponomarev::Stack< T >::getTop()
{
  return data.front();
}
template < typename T >
const T &ponomarev::Stack< T >::getTop() const
{
  return data.front();
}

#endif
