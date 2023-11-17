#ifndef BINARY_TREE_ITERATOR_H
#define BINARY_TREE_ITERATOR_H

#include "BinarySearchTree.h"

namespace nemchinov
{
  template< typename Key, typename Value, typename Compare >
  template< typename ValueIt >
  class BinarySearchTree< Key, Value, Compare >::Iterator
  {
    friend class BinarySearchTree;

  public:
    using iterator_type = ValueIt;
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = iterator_type;
    using difference_type = ptrdiff_t;
    using reference = iterator_type &;
    using pointer = iterator_type *;
    using node = TreeNode;
    using iter = Iterator< ValueIt >;

    explicit Iterator(node *, BaseTreeNode *);
    ~Iterator() = default;

    Iterator(const Iterator &) = default;
    Iterator &operator=(const Iterator &) = delete;

    Iterator(Iterator &&) noexcept = default;
    Iterator &operator=(Iterator &&) = default;

    bool operator!=(iter const &) const;
    bool operator==(iter const &) const;
    reference operator*();
    const Key &key() const;
    ValueIt &value();
    pointer operator->();
    iter &operator++();
    iter &operator++(int);
    iter &operator--();
    iter &operator--(int);

  private:
    node *itNode_;
    BaseTreeNode *itFake_;
  };

  template< typename Key, typename Value, typename Compare >
  template< typename ValueIt >
  BinarySearchTree< Key, Value, Compare >::Iterator< ValueIt >::Iterator(TreeNode
                                                                         *node, BaseTreeNode *fakeNode):
          itNode_(node), itFake_(fakeNode)
  {
  }

  template< typename Key, typename Value, typename Compare >
  template< typename ValueIt >
  bool BinarySearchTree< Key, Value, Compare >::Iterator< ValueIt >::operator!=(
          const BinarySearchTree< Key, Value, Compare >::Iterator< ValueIt > &other) const
  {
    return this->itNode_ != other.itNode_;
  }

  template< typename Key, typename Value, typename Compare >
  template< typename ValueIt >
  bool BinarySearchTree< Key, Value, Compare >::Iterator< ValueIt >::operator==(
          const BinarySearchTree< Key, Value, Compare >::Iterator< ValueIt > &other) const
  {
    return this->itNode_ == other.itNode_;
  }

  template< typename Key, typename Value, typename Compare >
  template< typename ValueIt >
  ValueIt &BinarySearchTree< Key, Value, Compare >::Iterator< ValueIt >::operator*()
  {
    if (!itNode_) throw std::logic_error("Operator * error : nullptr iterator");
    return itNode_->data_.second;
  }

  template< typename Key, typename Value, typename Compare >
  template< typename ValueIt >
  const Key &BinarySearchTree< Key, Value, Compare >::Iterator< ValueIt >::key() const
  {
    if (!itNode_) throw std::logic_error("Operator * error : nullptr iterator");
    return itNode_->data_.first;
  }

  template< typename Key, typename Value, typename Compare >
  template< typename ValueIt >
  ValueIt &BinarySearchTree< Key, Value, Compare >::Iterator< ValueIt >::value()
  {
    return this->operator*();
  }

  template< typename Key, typename Value, typename Compare >
  template< typename ValueIt >
  ValueIt *BinarySearchTree< Key, Value, Compare >::Iterator< ValueIt >::operator->()
  {
    if (!itNode_) throw std::logic_error("Operator -> error : nullptr iterator");
    return &itNode_->data_.second;
  }

  template< typename Key, typename Value, typename Compare >
  template< typename ValueIt >
  typename BinarySearchTree< Key, Value, Compare >::template Iterator< ValueIt > &
  BinarySearchTree< Key, Value, Compare >::Iterator< ValueIt >::operator++()
  {
    if (itNode_ == itFake_->right_) itNode_ = nullptr;
    else if (!itNode_) itNode_ = itFake_->left_;
    else if (itNode_->right_)
    {
      itNode_ = itNode_->right_;
      while (itNode_->left_) itNode_ = itNode_->left_;
    } else
    {
      while (itNode_->parent_ && itNode_->parent_->left_ != itNode_)
      {
        itNode_ = itNode_->parent_;
      }
      itNode_ = itNode_->parent_;
    }
    return *this;
  }

  template< typename Key, typename Value, typename Compare >
  template< typename ValueIt >
  typename BinarySearchTree< Key, Value, Compare >::template Iterator< ValueIt > &
  BinarySearchTree< Key, Value, Compare >::Iterator< ValueIt >::operator++(int)
  {
    iter copy = *this;
    this->operator++();
    return copy;

  }

  template< typename Key, typename Value, typename Compare >
  template< typename ValueIt >
  typename BinarySearchTree< Key, Value, Compare >::template Iterator< ValueIt > &
  BinarySearchTree< Key, Value, Compare >::Iterator< ValueIt >::operator--()
  {
    if (itNode_ == itFake_->left_) itNode_ = nullptr;
    else if (!itNode_) itNode_ = itFake_->right_;
    else if (itNode_->left_)
    {
      itNode_ = itNode_->left_;
      while (itNode_->right_) itNode_ = itNode_->right_;
    } else
    {
      while (itNode_->parent_ && itNode_->parent_->right_ != itNode_)
      {
        itNode_ = itNode_->parent_;
      }
      itNode_ = itNode_->parent_;
    }
    return *this;
  }

  template< typename Key, typename Value, typename Compare >
  template< typename ValueIt >
  typename BinarySearchTree< Key, Value, Compare >::template Iterator< ValueIt > &
  BinarySearchTree< Key, Value, Compare >::Iterator< ValueIt >::operator--(int)
  {
    iter copy = *this;
    this->operator--();
    return copy;
  }
}

#endif
