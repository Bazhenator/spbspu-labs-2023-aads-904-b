#ifndef BINARY_TRAVERSE_H
#define BINARY_TRAVERSE_H

#include <BinarySearchTree.h>
#include <BinaryTreeIterator.h>
#include <Stack.h>

namespace nemchinov
{
  template< typename Key, typename Value, typename Compare >
  template< typename F >
  F BinarySearchTree< Key, Value, Compare >::traverse_lnr(F f) const
  {
    for (const_iterator it = begin(); it != end(); ++it)
    {
      f(it.key(), it.value());
    }
    return f;
  }

  template< typename Key, typename Value, typename Compare >
  template< typename F >
  F BinarySearchTree< Key, Value, Compare >::traverse_rnl(F f) const
  {
    if (!fake_->parent_)
    {
      return f;
    }

    const_iterator it = end();
    --it;
    for (; it != begin(); --it)
    {
      f(it.key(), it.value());
    }
    f(it.key(), it.value());
    return f;
  }

  template< typename Key, typename Value, typename Compare >
  template< typename F >
  F BinarySearchTree< Key, Value, Compare >::traverse_breadth(F f) const
  {
    if (!fake_->parent_)
    {
      return f;
    }

    Stack< const TreeNode * > nodeStack;
    nodeStack.push(fake_->parent_);
    while (!nodeStack.isEmpty())
    {
      const TreeNode *node = nodeStack.get();
      nodeStack.pop();
      f(node->data_.first, node->data_.second);
      if (node->right_)
        nodeStack.push(node->right_);
      if (node->left_)
        nodeStack.push(node->left_);
    }
    return f;
  }
}

#endif
