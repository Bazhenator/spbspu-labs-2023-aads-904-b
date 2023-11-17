#ifndef ITERATORS_H
#define ITERATORS_H

#include "ForwardList.h"

namespace pelipeychenko
{
  template< typename T >
  class pelipeychenko::ForwardList< T >::ConstIterator: public std::iterator< std::forward_iterator_tag, T >
  {
  public:
    friend class ForwardList< T >;

    using pointer = const T *;
    using reference = const T &;

    ConstIterator() = default;

    explicit ConstIterator(typename pelipeychenko::ForwardList< T >::Node *pointer);

    bool operator!=(ConstIterator const &right) const;

    bool operator==(ConstIterator const &right) const;

    reference operator*() const;

    pointer operator->() const;

    ConstIterator &operator++();

    ConstIterator operator++(int);

    ConstIterator operator+(size_t) const;

  protected:
    typename ForwardList< T >::Node *node_;
  };

  template< typename T >
  ForwardList< T >::ConstIterator::ConstIterator(ForwardList< T >::Node *node) :
    node_(node)
  {}

  template< typename T >
  bool ForwardList< T >::ConstIterator::ConstIterator::operator!=(const ConstIterator &right) const
  {
    return node_ != right.node_;
  }

  template< typename T >
  bool ForwardList< T >::ConstIterator::ConstIterator::operator==(const ConstIterator &right) const
  {
    return node_ == right.node_;
  }

  template< typename T >
  typename ForwardList< T >::ConstIterator &ForwardList< T >::ConstIterator::ConstIterator::operator++()
  {
    if (node_ == nullptr) {
      throw std::runtime_error("prefix increment: iterator run out of border");
    }
    node_ = node_->next_;
    return (*this);
  }

  template< typename T >
  typename ForwardList< T >::ConstIterator ForwardList< T >::ConstIterator::ConstIterator::operator++(int)
  {
    ConstIterator temp = *this;
    if (node_ == nullptr) {
      throw std::runtime_error("postfix increment: iterator run out of border");
    }
    node_ = node_->next_;
    return temp;
  }

  template< typename T >
  typename ForwardList< T >::ConstIterator::reference ForwardList< T >::ConstIterator::operator*() const
  {
    return node_->data_;
  }

  template< typename T >
  typename ForwardList< T >::ConstIterator::pointer ForwardList< T >::ConstIterator::operator->() const
  {
    return &(node_->data_);
  }

  template< typename T >
  typename ForwardList< T >::ConstIterator
  ForwardList< T >::ConstIterator::ConstIterator::operator+(const size_t addition) const
  {
    ConstIterator it;
    it.node_ = node_;
    for (size_t i = 0; i < addition; i++) {
      it++;
    }
    return it;
  }

  template< typename T >
  class pelipeychenko::ForwardList< T >::Iterator: public std::iterator< std::forward_iterator_tag, T >
  {
  public:
    friend pelipeychenko::ForwardList< T >;

    using pointer = T *;
    using reference = T &;

    typename pelipeychenko::ForwardList< T >::Node *node_;

    Iterator() = default;

    explicit Iterator(typename pelipeychenko::ForwardList< T >::Node *pointer);

    bool operator!=(Iterator const &right) const;

    bool operator==(Iterator const &right) const;

    reference operator*();

    pointer operator->();

    Iterator &operator++();

    Iterator operator++(int);

    Iterator operator+(size_t) const;
  };

  template< typename T >
  ForwardList< T >::Iterator::Iterator(ForwardList< T >::Node *pointer) :
    node_(pointer)
  {}

  template< typename T >
  bool ForwardList< T >::Iterator::Iterator::operator!=(const Iterator &right) const
  {
    return node_ != right.node_;
  }

  template< typename T >
  bool ForwardList< T >::Iterator::Iterator::operator==(const Iterator &right) const
  {
    return node_ == right.node_;
  }

  template< typename T >
  typename ForwardList< T >::Iterator &ForwardList< T >::Iterator::Iterator::operator++()
  {
    if (node_ == nullptr) {
      throw std::runtime_error("prefix: iterator run out of border");
    }
    node_ = node_->next_;
    return (*this);
  }

  template< typename T >
  typename ForwardList< T >::Iterator ForwardList< T >::Iterator::Iterator::operator++(int)
  {
    Iterator temp = *this;
    if (node_ == nullptr) {
      throw std::runtime_error("postfix: iterator run out of border");
    }
    node_ = node_->next_;
    return temp;
  }

  template< typename T >
  typename ForwardList< T >::Iterator::reference ForwardList< T >::Iterator::operator*()
  {
    return node_->data_;
  }

  template< typename T >
  typename ForwardList< T >::Iterator::pointer ForwardList< T >::Iterator::operator->()
  {
    return &(node_->data_);
  }

  template< typename T >
  typename ForwardList< T >::Iterator
  ForwardList< T >::Iterator::Iterator::operator+(const size_t addition) const
  {
    Iterator it;
    it.node_ = node_;
    for (size_t i = 0; i < addition; i++) {
      it++;
    }
    return it;
  }
}

#endif
