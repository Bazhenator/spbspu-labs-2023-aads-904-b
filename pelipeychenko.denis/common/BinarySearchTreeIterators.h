#ifndef BINARYSEARCHTREEITERATORS_H
#define BINARYSEARCHTREEITERATORS_H

#include "BinarySearchTree.h"
#include "pair.h"
#include "Queue.h"

namespace pelipeychenko
{
  template< typename Key, typename Value, typename Comparator >
  class pelipeychenko::BinarySearchTree< Key, Value, Comparator >::ConstIterator
    : public std::iterator< std::bidirectional_iterator_tag, pelipeychenko::pair< Key, Value > >
  {
  public:
    friend class BinarySearchTree< Key, Value, Comparator >;

    ConstIterator();

    explicit ConstIterator(Node *head, TraversalMode);

    explicit ConstIterator(Node *head, Node *node, TraversalMode);

    ~ConstIterator() = default;

    using pointer = const pelipeychenko::pair< Key, Value > *;
    using reference = const pelipeychenko::pair< Key, Value > &;

    bool operator!=(ConstIterator const &right) const;

    bool operator==(ConstIterator const &right) const;

    bool operator>(ConstIterator const &right) const;

    bool operator<(ConstIterator const &right) const;

    reference operator*() const;

    pointer operator->() const;

    ConstIterator &operator++();

    ConstIterator &operator--();

    const ConstIterator operator++(int);

    const ConstIterator operator--(int);

    ConstIterator operator+(size_t) const;

    ConstIterator operator-(size_t) const;

  protected:
    Node *node_;
  private:
    Stack< Node * > path_;
    Queue< Node * > breadthPath_;
    TraversalMode mode_;

    ConstIterator &moveNext();

    ConstIterator &movePrev();

    ConstIterator &moveInWidth();
  };

  template< typename Key, typename Value, typename Comparator >
  BinarySearchTree< Key, Value, Comparator >::ConstIterator::ConstIterator(
    Node *head, Node *node, TraversalMode mode) :
    node_(node),
    mode_(mode)
  {
    if (node == nullptr) {
      return;
    }
    breadthPath_.push(head);
    Comparator comparator;
    Node *current = head;
    while (current != node) {
      path_.push(current);
      if (comparator(current->data_.first_, node->data_.first_)) {
        current = current->right_;
      } else {
        current = current->left_;
      }
    }
  }

  template< typename Key, typename Value, typename Comparator >
  BinarySearchTree< Key, Value, Comparator >::ConstIterator::ConstIterator() :
    node_(nullptr),
    mode_(ASCENDING)
  {}

  template< typename Key, typename Value, typename Comparator >
  BinarySearchTree< Key, Value, Comparator >::ConstIterator::ConstIterator(Node *head, TraversalMode mode) :
    node_(head),
    mode_(mode)
  {
    breadthPath_.push(head);
  }

  template< typename Key, typename Value, typename Comparator >
  bool
  BinarySearchTree< Key, Value, Comparator >::ConstIterator::ConstIterator::operator!=(const ConstIterator &right) const
  {
    return node_ != right.node_;
  }

  template< typename Key, typename Value, typename Comparator >
  bool
  BinarySearchTree< Key, Value, Comparator >::ConstIterator::ConstIterator::operator==(const ConstIterator &right) const
  {
    return node_ == right.node_;
  }

  template< typename Key, typename Value, typename Comparator >
  bool
  BinarySearchTree< Key, Value, Comparator >::ConstIterator::ConstIterator::operator<(const ConstIterator &right) const
  {
    if (right.node_ == nullptr) {
      return node_ != nullptr;
    }
    return node_->data_.first_ < right.node_->data_.first_;
  }

  template< typename Key, typename Value, typename Comparator >
  bool
  BinarySearchTree< Key, Value, Comparator >::ConstIterator::ConstIterator::operator>(const ConstIterator &right) const
  {
    if (node_ == nullptr) {
      return right.node_ != nullptr;
    }
    return node_->data_.first_ > right.node_->data_.first_;
  }

  template< typename Key, typename Value, typename Comparator >
  typename BinarySearchTree< Key, Value, Comparator >::ConstIterator &
  BinarySearchTree< Key, Value, Comparator >::ConstIterator::ConstIterator::moveNext()
  {
    if (node_ == nullptr) {
      throw std::runtime_error("increment: iterator run out of border");
    }
    if (node_->right_ != nullptr) {
      path_.push(node_);
      Node *current = node_->right_;
      while (current->left_ != nullptr) {
        path_.push(current);
        current = current->left_;
      }
      node_ = current;
      return *this;
    }
    if (path_.isEmpty() && node_->right_ == nullptr) {
      node_ = nullptr;
      return *this;
    }

    if (node_->right_ == nullptr && node_ == path_.getHead()->left_) {
      node_ = path_.getHead();
      path_.drop();
    } else if (node_->right_ == nullptr && node_ == path_.getHead()->right_) {
      Node *current = path_.getHead();
      path_.drop();
      while (!path_.isEmpty() && current != path_.getHead()->left_) {
        current = path_.getHead();
        path_.drop();
      }
      if (!path_.isEmpty() && current == path_.getHead()->left_) {
        node_ = path_.getHead();
        path_.drop();
      } else {
        node_ = nullptr;
      }
    }
    return *this;
  }

  template< typename Key, typename Value, typename Comparator >
  typename BinarySearchTree< Key, Value, Comparator >::ConstIterator &
  BinarySearchTree< Key, Value, Comparator >::ConstIterator::ConstIterator::moveInWidth()
  {
    if (node_ == nullptr) {
      throw std::runtime_error("increment: iterator run out of border");
    }
    if (node_->left_ != nullptr) {
      breadthPath_.push(node_->left_);
    }
    if (node_->right_ != nullptr) {
      breadthPath_.push(node_->right_);
    }
    breadthPath_.drop();
    if (!breadthPath_.isEmpty()) {
      node_ = breadthPath_.getHead();
    } else {
      node_ = nullptr;
    }
    return *this;
  }

  template< typename Key, typename Value, typename Comparator >
  typename BinarySearchTree< Key, Value, Comparator >::ConstIterator &
  BinarySearchTree< Key, Value, Comparator >::ConstIterator::ConstIterator::operator++()
  {
    switch (mode_) {
      case ASCENDING:
        return moveNext();
      case DESCENDING:
        return movePrev();
      case BREADTH:
        return moveInWidth();
    }
    return *this;
  }

  template< typename Key, typename Value, typename Comparator >
  typename BinarySearchTree< Key, Value, Comparator >::ConstIterator &
  BinarySearchTree< Key, Value, Comparator >::ConstIterator::ConstIterator::operator--()
  {
    switch (mode_) {
      case ASCENDING:
        return movePrev();
      case DESCENDING:
        return moveNext();
      case BREADTH:
        return moveInWidth();
    }
    return *this;
  }

  template< typename Key, typename Value, typename Comparator >
  typename BinarySearchTree< Key, Value, Comparator >::ConstIterator &
  BinarySearchTree< Key, Value, Comparator >::ConstIterator::ConstIterator::movePrev()
  {
    if (node_ == nullptr) {
      throw std::runtime_error("decrement: iterator run out of border");
    }
    if (node_->left_ != nullptr) {
      path_.push(node_);
      Node *current = node_->left_;
      while (current->right_ != nullptr) {
        path_.push(current);
        current = current->right_;
      }
      node_ = current;
      return *this;
    }
    if (path_.isEmpty() && node_->left_ == nullptr) {
      node_ = nullptr;
      return *this;
    }

    if (node_->left_ == nullptr && node_ == path_.getHead()->right_) {
      node_ = path_.getHead();
      path_.drop();
    } else if (node_->left_ == nullptr && node_ == path_.getHead()->left_) {
      Node *current = path_.getHead();
      path_.drop();
      while (!path_.isEmpty() && current != path_.getHead()->right_) {
        current = path_.getHead();
        path_.drop();
      }
      if (!path_.isEmpty() && current == path_.getHead()->right_) {
        node_ = path_.getHead();
        path_.drop();
      } else {
        node_ = nullptr;
      }
    }
    return *this;
  }

  template< typename Key, typename Value, typename Comparator >
  const typename BinarySearchTree< Key, Value, Comparator >::ConstIterator
  pelipeychenko::BinarySearchTree< Key, Value, Comparator >::ConstIterator::operator++(int)
  {
    ConstIterator temp = *this;
    operator++();
    return temp;
  }

  template< typename Key, typename Value, typename Comparator >
  const typename BinarySearchTree< Key, Value, Comparator >::ConstIterator
  pelipeychenko::BinarySearchTree< Key, Value, Comparator >::ConstIterator::operator--(int)
  {
    ConstIterator temp = *this;
    operator--();
    return temp;
  }

  template< typename Key, typename Value, typename Comparator >
  typename BinarySearchTree< Key, Value, Comparator >::ConstIterator
  pelipeychenko::BinarySearchTree< Key, Value, Comparator >::ConstIterator::operator+(size_t addition) const
  {
    ConstIterator temp(node_);
    for (int i = 0; i < addition; i++) {
      temp++;
    }
    return temp;
  }

  template< typename Key, typename Value, typename Comparator >
  typename BinarySearchTree< Key, Value, Comparator >::ConstIterator
  pelipeychenko::BinarySearchTree< Key, Value, Comparator >::ConstIterator::operator-(size_t addition) const
  {
    ConstIterator temp(node_);
    for (int i = 0; i < addition; i++) {
      temp--;
    }
    return temp;
  }

  template< typename Key, typename Value, typename Comparator >
  typename pelipeychenko::BinarySearchTree< Key, Value, Comparator >::ConstIterator::reference
  pelipeychenko::BinarySearchTree< Key, Value, Comparator >::ConstIterator::operator*() const
  {
    return node_->data_;
  }

  template< typename Key, typename Value, typename Comparator >
  typename pelipeychenko::BinarySearchTree< Key, Value, Comparator >::ConstIterator::pointer
  pelipeychenko::BinarySearchTree< Key, Value, Comparator >::ConstIterator::operator->() const
  {
    return &(node_->data_);
  }

  template< typename Key, typename Value, typename Comparator >
  class pelipeychenko::BinarySearchTree< Key, Value, Comparator >::Iterator
    : public std::iterator< std::bidirectional_iterator_tag, pelipeychenko::pair< Key, Value > >
  {
  public:
    friend class BinarySearchTree< Key, Value >;

    Iterator();

    explicit Iterator(Node *head, TraversalMode);

    explicit Iterator(Node *head, Node *node, TraversalMode);

    ~Iterator() = default;

    using pointer = pelipeychenko::pair< Key, Value > *;
    using reference = pelipeychenko::pair< Key, Value > &;

    bool operator!=(Iterator const &right) const;

    bool operator==(Iterator const &right) const;

    bool operator>(Iterator const &right) const;

    bool operator<(Iterator const &right) const;

    reference operator*();

    pointer operator->();

    Iterator &operator++();

    Iterator &operator--();

    const Iterator operator++(int);

    const Iterator operator--(int);

    Iterator operator+(size_t) const;

    Iterator operator-(size_t) const;

  protected:
    Node *node_;

  private:
    Stack< Node * > path_;
    Queue< Node * > breadthPath_;
    TraversalMode mode_;

    Iterator &moveNext();

    Iterator &movePrev();

    Iterator &moveInWidth();
  };

  template< typename Key, typename Value, typename Comparator >
  BinarySearchTree< Key, Value, Comparator >::Iterator::Iterator(
    Node *head, Node *node, TraversalMode mode) :
    node_(node),
    mode_(mode)
  {
    if (node == nullptr) {
      return;
    }
    breadthPath_.push(head);
    Comparator comparator;
    Node *current = head;
    while (current != node) {
      path_.push(current);
      if (comparator(current->data_.first_, node->data_.first_)) {
        current = current->right_;
      } else {
        current = current->left_;
      }
    }
  }

  template< typename Key, typename Value, typename Comparator >
  BinarySearchTree< Key, Value, Comparator >::Iterator::Iterator() :
    node_(nullptr),
    mode_(ASCENDING)
  {}

  template< typename Key, typename Value, typename Comparator >
  BinarySearchTree< Key, Value, Comparator >::Iterator::Iterator(Node *head, TraversalMode mode) :
    node_(head),
    mode_(mode)
  {
    breadthPath_.push(head);
  }

  template< typename Key, typename Value, typename Comparator >
  bool
  BinarySearchTree< Key, Value, Comparator >::Iterator::Iterator::operator!=(const Iterator &right) const
  {
    return node_ != right.node_;
  }

  template< typename Key, typename Value, typename Comparator >
  bool
  BinarySearchTree< Key, Value, Comparator >::Iterator::Iterator::operator==(const Iterator &right) const
  {
    return node_ == right.node_;
  }

  template< typename Key, typename Value, typename Comparator >
  bool
  BinarySearchTree< Key, Value, Comparator >::Iterator::Iterator::operator<(const Iterator &right) const
  {
    if (right.node_ == nullptr) {
      return node_ != nullptr;
    }
    return node_->data_.first_ < right.node_->data_.first_;
  }

  template< typename Key, typename Value, typename Comparator >
  bool
  BinarySearchTree< Key, Value, Comparator >::Iterator::Iterator::operator>(const Iterator &right) const
  {
    if (node_ == nullptr) {
      return right.node_ != nullptr;
    }
    return node_->data_.first_ > right.node_->data_.first_;
  }

  template< typename Key, typename Value, typename Comparator >
  typename BinarySearchTree< Key, Value, Comparator >::Iterator &
  BinarySearchTree< Key, Value, Comparator >::Iterator::Iterator::moveNext()
  {
    if (node_ == nullptr) {
      throw std::runtime_error("increment: iterator run out of border");
    }
    if (node_->right_ != nullptr) {
      path_.push(node_);
      Node *current = node_->right_;
      while (current->left_ != nullptr) {
        path_.push(current);
        current = current->left_;
      }
      node_ = current;
      return *this;
    }
    if (path_.isEmpty() && node_->right_ == nullptr) {
      node_ = nullptr;
      return *this;
    }

    if (node_->right_ == nullptr && node_ == path_.getHead()->left_) {
      node_ = path_.getHead();
      path_.drop();
    } else if (node_->right_ == nullptr && node_ == path_.getHead()->right_) {
      Node *current = path_.getHead();
      path_.drop();
      while (!path_.isEmpty() && current != path_.getHead()->left_) {
        current = path_.getHead();
        path_.drop();
      }
      if (!path_.isEmpty() && current == path_.getHead()->left_) {
        node_ = path_.getHead();
        path_.drop();
      } else {
        node_ = nullptr;
      }
    }
    return *this;
  }

  template< typename Key, typename Value, typename Comparator >
  typename BinarySearchTree< Key, Value, Comparator >::Iterator &
  BinarySearchTree< Key, Value, Comparator >::Iterator::Iterator::moveInWidth()
  {
    if (node_ == nullptr) {
      throw std::runtime_error("increment: iterator run out of border");
    }
    if (node_->left_ != nullptr) {
      breadthPath_.push(node_->left_);
    }
    if (node_->right_ != nullptr) {
      breadthPath_.push(node_->right_);
    }
    breadthPath_.drop();
    if (!breadthPath_.isEmpty()) {
      node_ = breadthPath_.getHead();
    } else {
      node_ = nullptr;
    }
    return *this;
  }

  template< typename Key, typename Value, typename Comparator >
  typename BinarySearchTree< Key, Value, Comparator >::Iterator &
  BinarySearchTree< Key, Value, Comparator >::Iterator::Iterator::operator++()
  {
    switch (mode_) {
      case ASCENDING:
        return moveNext();
      case DESCENDING:
        return movePrev();
      case BREADTH:
        return moveInWidth();
    }
  }

  template< typename Key, typename Value, typename Comparator >
  typename BinarySearchTree< Key, Value, Comparator >::Iterator &
  BinarySearchTree< Key, Value, Comparator >::Iterator::Iterator::operator--()
  {
    switch (mode_) {
      case ASCENDING:
        return movePrev();
      case DESCENDING:
        return moveNext();
      case BREADTH:
        return moveInWidth();
    }
  }

  template< typename Key, typename Value, typename Comparator >
  typename BinarySearchTree< Key, Value, Comparator >::Iterator &
  BinarySearchTree< Key, Value, Comparator >::Iterator::Iterator::movePrev()
  {
    if (node_ == nullptr) {
      throw std::runtime_error("decrement: iterator run out of border");
    }
    if (node_->left_ != nullptr) {
      path_.push(node_);
      Node *current = node_->left_;
      while (current->right_ != nullptr) {
        path_.push(current);
        current = current->right_;
      }
      node_ = current;
      return *this;
    }
    if (path_.isEmpty() && node_->left_ == nullptr) {
      node_ = nullptr;
      return *this;
    }

    if (node_->left_ == nullptr && node_ == path_.getHead()->right_) {
      node_ = path_.getHead();
      path_.drop();
    } else if (node_->left_ == nullptr && node_ == path_.getHead()->left_) {
      Node *current = path_.getHead();
      path_.drop();
      while (!path_.isEmpty() && current != path_.getHead()->right_) {
        current = path_.getHead();
        path_.drop();
      }
      if (!path_.isEmpty() && current == path_.getHead()->right_) {
        node_ = path_.getHead();
        path_.drop();
      } else {
        node_ = nullptr;
      }
    }
    return *this;
  }

  template< typename Key, typename Value, typename Comparator >
  const typename BinarySearchTree< Key, Value, Comparator >::Iterator
  pelipeychenko::BinarySearchTree< Key, Value, Comparator >::Iterator::operator++(int)
  {
    Iterator temp = *this;
    operator++();
    return temp;
  }

  template< typename Key, typename Value, typename Comparator >
  const typename BinarySearchTree< Key, Value, Comparator >::Iterator
  pelipeychenko::BinarySearchTree< Key, Value, Comparator >::Iterator::operator--(int)
  {
    Iterator temp = *this;
    operator--();
    return temp;
  }

  template< typename Key, typename Value, typename Comparator >
  typename BinarySearchTree< Key, Value, Comparator >::Iterator
  pelipeychenko::BinarySearchTree< Key, Value, Comparator >::Iterator::operator+(size_t addition) const
  {
    Iterator temp(node_);
    for (int i = 0; i < addition; i++) {
      temp++;
    }
    return temp;
  }

  template< typename Key, typename Value, typename Comparator >
  typename BinarySearchTree< Key, Value, Comparator >::Iterator
  pelipeychenko::BinarySearchTree< Key, Value, Comparator >::Iterator::operator-(size_t addition) const
  {
    Iterator temp(node_);
    for (int i = 0; i < addition; i++) {
      temp--;
    }
    return temp;
  }

  template< typename Key, typename Value, typename Comparator >
  typename pelipeychenko::BinarySearchTree< Key, Value, Comparator >::Iterator::reference
  pelipeychenko::BinarySearchTree< Key, Value, Comparator >::Iterator::operator*()
  {
    return node_->data_;
  }

  template< typename Key, typename Value, typename Comparator >
  typename pelipeychenko::BinarySearchTree< Key, Value, Comparator >::Iterator::pointer
  pelipeychenko::BinarySearchTree< Key, Value, Comparator >::Iterator::operator->()
  {
    return &(node_->data_);
  }
}

#endif
