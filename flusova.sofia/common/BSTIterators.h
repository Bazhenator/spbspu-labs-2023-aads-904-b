#ifndef BST_ITERATORS_H
#define BST_ITERATORS_H

#include <cassert>
#include <Stack.h>
#include "BST.h"

namespace flusova {
  template< typename Key, typename Value, typename Comparator >
  class BinarySearchTree< Key, Value, Comparator >::ConstIterator {
  public:

    explicit ConstIterator(typename flusova::BinarySearchTree< Key, Value, Comparator >::Node* node);
    ~ConstIterator() = default;
    ConstIterator& operator++();
    ConstIterator operator++(int);
    const std::pair< Key, Value >& operator*() const;
    const std::pair< Key, Value >* operator->() const;
    bool operator==(const ConstIterator& obj) const noexcept;
    bool operator!=(const ConstIterator& obj) const noexcept;

    friend class BinarySearchTree< Key, Value, Comparator >;

  protected:

    typename flusova::BinarySearchTree< Key, Value, Comparator >::Node* current_;
    Stack< typename flusova::BinarySearchTree< Key, Value, Comparator >::Node* > stack_;

  };

  template< typename Key, typename Value, typename Comparator >
  class BinarySearchTree< Key, Value, Comparator >::Iterator : public ConstIterator {
  public:

    explicit Iterator(BinarySearchTree< Key, Value, Comparator >::Node* node);
    Iterator& operator++();
    Iterator operator++(int);
    std::pair< Key, Value >& operator*();
    std::pair< Key, Value >* operator->();

  };
}

template< typename Key, typename Value, typename Comparator >
flusova::BinarySearchTree< Key, Value, Comparator >::ConstIterator::ConstIterator(Node* node):
        current_(node) {
  while (current_) {
    stack_.pushTop(current_);
    current_ = current_->left_;
  }
  if (stack_.isEmpty()) {
    current_ = nullptr;
  } else {
    current_ = stack_.getHead();
  }
}

template< typename Key, typename Value, typename Comparator >
typename flusova::BinarySearchTree< Key, Value, Comparator >::ConstIterator
& flusova::BinarySearchTree< Key, Value, Comparator >::ConstIterator::operator++() {
  assert(current_ != nullptr);
  stack_.popTop();
  Node* node = current_->right_;
  while (node) {
    stack_.pushTop(node);
    node = node->left_;
  }
  if (stack_.isEmpty()) {
    current_ = nullptr;
  } else {
    current_ = stack_.getHead();
  }
  return *this;
}

template< typename Key, typename Value, typename Comparator >
typename flusova::BinarySearchTree< Key, Value, Comparator >::ConstIterator
flusova::BinarySearchTree< Key, Value, Comparator >::ConstIterator::operator++(int) {
  assert(current_ != nullptr);
  ConstIterator it = (*this);
  ++(*this);
  return it;
}

template< typename Key, typename Value, typename Comparator >
const std::pair< Key, Value >& flusova::BinarySearchTree< Key, Value, Comparator >::ConstIterator::operator*() const {
  assert(current_ != nullptr);
  return current_->data_;
}

template< typename Key, typename Value, typename Comparator >
const std::pair< Key, Value >* flusova::BinarySearchTree< Key, Value, Comparator >::ConstIterator::operator->() const {
  assert(current_ != nullptr);
  return std::pointer_traits< const std::pair< Key, Value >* >::pointer_to(**this);
}

template< typename Key, typename Value, typename Comparator >
bool flusova::BinarySearchTree< Key, Value, Comparator >::ConstIterator::operator==(
        const flusova::BinarySearchTree< Key, Value, Comparator >::ConstIterator& obj) const noexcept {
  return (current_ == obj.current_);
}

template< typename Key, typename Value, typename Comparator >
bool flusova::BinarySearchTree< Key, Value, Comparator >::ConstIterator::operator!=(
        const flusova::BinarySearchTree< Key, Value, Comparator >::ConstIterator& obj) const noexcept {
  return !((*this) == obj);
}

template< typename Key, typename Value, typename Comparator >
flusova::BinarySearchTree< Key, Value, Comparator >::Iterator::Iterator(Node* node):
        ConstIterator(node) {}

template< typename Key, typename Value, typename Comparator >
typename flusova::BinarySearchTree< Key, Value, Comparator >::Iterator
& flusova::BinarySearchTree< Key, Value, Comparator >::Iterator::operator++() {
  ConstIterator::operator++();
  return (*this);
}

template< typename Key, typename Value, typename Comparator >
typename flusova::BinarySearchTree< Key, Value, Comparator >::Iterator
flusova::BinarySearchTree< Key, Value, Comparator >::Iterator::operator++(int) {
  Iterator it = *this;
  ConstIterator::operator++();
  return it;
}

template< typename Key, typename Value, typename Comparator >
std::pair< Key, Value >& flusova::BinarySearchTree< Key, Value, Comparator >::Iterator::operator*() {
  return const_cast< std::pair< Key, Value >& >(ConstIterator::operator*());
}

template< typename Key, typename Value, typename Comparator >
std::pair< Key, Value >* flusova::BinarySearchTree< Key, Value, Comparator >::Iterator::operator->() {
  return const_cast< std::pair< Key, Value >* >(ConstIterator::operator->());
}

#endif
