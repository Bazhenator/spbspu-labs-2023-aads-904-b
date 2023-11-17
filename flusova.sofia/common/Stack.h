#ifndef STACK_H
#define STACK_H
#include <cstdlib>
#include "MyForwardList.h"

namespace flusova {
  template < typename T >
  class Stack
  {
  public:
    Stack() = default;
    ~Stack() = default;

    void pushTop(const T& newHead);
    void popTop();
    bool isEmpty() const noexcept;
    std::size_t& getSize() const noexcept;
    const T& getHead() const;

  private:
    MyForwardList< T > stack_;
  };
}

template < typename T >
void flusova::Stack<T>::pushTop(const T& newHead)
{
  stack_.pushFront(newHead);
}
template < typename T >
void flusova::Stack< T >::popTop()
{
  if (!isEmpty())
  {
    stack_.popFront();
  }
}
template < typename T >
bool flusova::Stack< T >::isEmpty() const noexcept
{
  return stack_.isEmpty();
}
template < typename T >
std::size_t& flusova::Stack< T >::getSize() const noexcept
{
  return stack_.getSize();
}
template < typename T >
const T& flusova::Stack< T >::getHead() const
{
  return stack_.getTop();
}

#endif
