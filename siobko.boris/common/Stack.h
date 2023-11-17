
#ifndef STACK_H
#define STACK_H

#include "ForwardList.h"

namespace siobko {
  template< typename Ty >
  class Stack {
  public:
    using storage_t = ForwardList< Ty >;
    using iterator = typename storage_t::iterator;
    using const_iterator = typename storage_t::const_iterator;
    using reference = Ty&;
    using const_reference = const Ty&;
    using size_type = typename storage_t::size_type;
    using value_type = typename storage_t::value_type;

    Stack() = default;
    Stack(const Stack& rhs);
    Stack(Stack&& rhs) noexcept;
    Stack(std::initializer_list< value_type > IList);
    ~Stack();

    iterator begin() noexcept;
    iterator end() noexcept;
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;

    reference front() noexcept;
    const_reference front() const noexcept;

    void push(const Ty& obj);
    void pop();
    bool isEmpty() const noexcept;
    void clear();
    size_type size() const noexcept;

  private:
    storage_t storage_;
  };

  template< typename Ty >
  Stack< Ty >::Stack(const Stack& rhs):
    storage_(rhs.storage_)
  {}

  template< typename Ty >
  Stack< Ty >::Stack(Stack&& rhs) noexcept:
    storage_(rhs.storage_)
  {}

  template< typename Ty >
  Stack< Ty >::Stack(std::initializer_list< value_type > IList):
    storage_(IList)
  {}

  template< typename Ty >
  Stack< Ty >::~Stack()
  {
    clear();
  }

  template< typename Ty >
  typename Stack< Ty >::iterator Stack< Ty >::begin() noexcept
  {
    return storage_.begin();
  }

  template< typename Ty >
  typename Stack< Ty >::iterator Stack< Ty >::end() noexcept
  {
    return storage_.end();
  }

  template< typename Ty >
  typename Stack< Ty >::const_iterator Stack< Ty >::begin() const noexcept
  {
    return storage_.cbegin();
  }

  template< typename Ty >
  typename Stack< Ty >::const_iterator Stack< Ty >::end() const noexcept
  {
    return storage_.cend();
  }

  template< typename Ty >
  typename Stack< Ty >::reference Stack< Ty >::front() noexcept
  {
    return storage_.front();
  }

  template< typename Ty >
  typename Stack< Ty >::const_reference Stack< Ty >::front() const noexcept
  {
    return storage_.front();
  }

  template< typename Ty >
  typename Stack< Ty >::size_type Stack< Ty >::size() const noexcept
  {
    return storage_.size();
  }

  template< typename Ty >
  bool Stack< Ty >::isEmpty() const noexcept
  {
    return storage_.isEmpty();
  }

  template< typename Ty >
  void Stack< Ty >::push(const Ty& obj)
  {
    storage_.pushFront(obj);
  }

  template< typename Ty >
  void Stack< Ty >::pop()
  {
    storage_.popFront();
  }

  template< typename Ty >
  void Stack< Ty >::clear()
  {
    storage_.clear();
  }
}
#endif
