#ifndef AVL_TREE_FUNCTIOANAL_H
#define AVL_TREE_FUNCTIOANAL_H

#include "BinarySearchTree.h"

namespace ponomarev {
  template < typename Key, typename Value, typename Compare, typename Predicate >
  void eraseIf(AVLTree< Key, Value, Compare > &tree, Predicate pred);
}

template < typename Key, typename Value, typename Compare, typename Predicate >
void ponomarev::eraseIf(AVLTree< Key, Value, Compare > &tree, Predicate pred)
{
  typename AVLTree< Key, Value, Compare >::Iterator ptr = tree.begin();
  ponomarev::Stack< std::pair< Key, Value > > toErase;
  while (ptr != tree.end()) {
    if (pred(*ptr)) {
      toErase.push(*ptr);
    }
    ptr++;
  }

  while (toErase.getSize()) {
    tree.erase(toErase.getTop().first);
    toErase.pop();
  }
}

#endif
