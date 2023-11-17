#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include <functional>
#include <algorithm>
#include <stdexcept>
#include <cassert>
#include <initializer_list>
#include <type_traits>
#include <utility>
#include "../common/Stack.h"
#include "../common/Queue.h"

namespace ponomarev {
  enum TraversalStrategy {
    IN_ORDER,
    REVERSE_IN_ORDER,
    BREADTH
  };
  template< typename Key, typename Value, typename Compare = std::less< Key > >
  class AVLTree {
    public:
      class Iterator;
      class ConstIterator;

      AVLTree();
      AVLTree(std::initializer_list< std::pair< Key, Value > > init);
      AVLTree(const AVLTree< Key, Value, Compare > &other);
      AVLTree(AVLTree< Key, Value, Compare > &&other) noexcept;
      ~AVLTree();

      AVLTree &operator=(const AVLTree< Key, Value, Compare > &rhs);
      AVLTree &operator=(AVLTree< Key, Value, Compare > &&rhs) noexcept;
      Value &operator[](const Key &key);

      template< typename F >
      F traverse_lnr(F f) const;
      template< typename F >
      F traverse_rnl(F f) const;
      template< typename F >
      F traverse_breadth(F f) const;

      void insert(const Key &key, const Value &data);
      void insert(std::pair< Key, Value > &&pair);
      template< class P >
      std::pair< Iterator, bool > insert(P&& value);
      Iterator insert(ConstIterator pos, const std::pair< Key, Value > &value);
      template< class InputIt >
      void insert(InputIt first, InputIt last );
      void insert(std::initializer_list< std::pair< Key, Value > > ilist);
      template< class... Args >
      std::pair< Iterator, bool > emplace(Args&&... args);

      void erase(const Key &key);
      Iterator erase(Iterator pos);
      Iterator erase(Iterator first, Iterator last);
      void clear();

      Value &at(const Key &key);
      Value &get(const Key &key);
      Iterator find(const Key &key);
      bool contains(const Key &key) const;
      std::size_t getSize() const noexcept;
      bool isEmpty() const noexcept;
      void merge(const AVLTree< Key, Value, Compare > &other);

      Iterator getUpperBound(const Key& key);
      Iterator getLowerBound(const Key& key);

      Iterator begin(TraversalStrategy strategy = IN_ORDER);
      ConstIterator cbegin(TraversalStrategy strategy = IN_ORDER) const;
      Iterator end();
      ConstIterator cend() const;
    private:
      struct Node;

      Node *insertIn(Node *tree, const Key &key, const Value &data);
      unsigned char getHeight(Node *tree) const;
      int getBalanceFactor(Node *tree);
      void setHeight(Node *tree);

      Node *rotateRight(Node *tree);
      Node *rotateLeft(Node *tree);
      Node *rotateRightLeft(Node *tree);
      Node *rotateLeftRight(Node *tree);
      Node *balanceTree(Node *tree);

      void eraseAll(Node *tree);
      Node *remove(Node *subtree, const Key &key);
      Iterator getIter(const Key &key);
      Node *removeNoDeleteMin(Node *tree);
      Node *findMin(Node *tree);

      void swap(AVLTree< Key, Value, Compare > &other) noexcept;

      Node *root;
      std::size_t size_;
  };

  template< typename Key, typename Value, typename Compare >
  struct AVLTree< Key, Value, Compare >::Node {
    Node(const Key &key, const Value &value, Node *left = nullptr, Node *right = nullptr);

    std::pair< Key, Value > data;
    Node *left;
    Node *right;
    unsigned char height;
  };
  template< typename Key, typename Value, typename Compare >
  AVLTree< Key, Value, Compare >::Node::Node(const Key &key, const Value &value,
                                             Node *left, Node *right):
    data(std::pair< Key, Value >(key, value)),
    left(left),
    right(right),
    height(1)
  {}

  template< typename Key, typename Value, typename Compare >
  AVLTree< Key, Value, Compare >::AVLTree():
    root(nullptr),
    size_(0)
  {}
  template< typename Key, typename Value, typename Compare >
  AVLTree< Key, Value, Compare >::AVLTree(std::initializer_list< std::pair< Key, Value > > init):
    AVLTree()
  {
    for (const auto &el: init) {
      insert(el.first, el.second);
    }
  }
  template< typename Key, typename Value, typename Compare >
  AVLTree< Key, Value, Compare >::AVLTree(const AVLTree< Key, Value, Compare > &other):
    AVLTree()
  {
    typename AVLTree< Key, Value, Compare >::ConstIterator ptr = other.cbegin();
    typename AVLTree< Key, Value, Compare >::ConstIterator end = other.cend();
    while (ptr != end) {
      insert(ptr->first, ptr->second);
      ptr++;
    }
  }
  template< typename Key, typename Value, typename Compare >
  class AVLTree< Key, Value, Compare >::ConstIterator {
    public:
      using iterator_category = std::forward_iterator_tag;
      using value_type        = const std::pair< Key, Value >;
      using difference_type   = std::ptrdiff_t;
      using pointer           = value_type*;
      using reference         = value_type&;

      explicit ConstIterator(Node *node, TraversalStrategy strategy = IN_ORDER);
      ~ConstIterator() = default;

      ConstIterator &operator++();
      ConstIterator operator++(int);

      reference operator*() const;
      pointer operator->() const;

      bool operator==(const ConstIterator &rhs) const noexcept;
      bool operator!=(const ConstIterator &rhs) const noexcept;

      friend class AVLTree< Key, Value, Compare >;
    protected:
      void pushAllLeft(Node *node);
      void pushAllRight(Node *node);
      void pushChildren(Node *node);

      typename AVLTree< Key, Value, Compare >::Node *node_;
      Stack< Node * > nodesStack_;
      Queue< Node * > nodesQueue_;
      TraversalStrategy strategy_;
  };
  template< typename Key, typename Value, typename Compare >
  AVLTree< Key, Value, Compare >::ConstIterator::ConstIterator(Node *node, TraversalStrategy strategy):
    node_(node),
    strategy_(strategy)
  {
    switch (strategy_) {
    case IN_ORDER:
      pushAllLeft(node_);
      node_ = (nodesStack_.isEmpty() ? nullptr : nodesStack_.getTop());
      break;
    case REVERSE_IN_ORDER:
      pushAllRight(node_);
      node_ = (nodesStack_.isEmpty() ? nullptr : nodesStack_.getTop());
      break;
    case BREADTH:
      nodesQueue_.push(node_);
      break;
    }
  }
  template< typename Key, typename Value, typename Compare >
  typename AVLTree< Key, Value, Compare >::ConstIterator::reference AVLTree< Key, Value, Compare >::ConstIterator::operator*() const
  {
    assert(node_ != nullptr);
    return node_->data;
  }
  template< typename Key, typename Value, typename Compare >
  typename AVLTree< Key, Value, Compare >::ConstIterator::pointer AVLTree< Key, Value, Compare >::ConstIterator::operator->() const
  {
    assert(node_ != nullptr);
    return std::pointer_traits< pointer >::pointer_to(**this);
  }
  template< typename Key, typename Value, typename Compare >
  bool AVLTree< Key, Value, Compare >::ConstIterator::operator==(const ConstIterator &rhs) const noexcept
  {
    return node_ == rhs.node_;
  }
  template< typename Key, typename Value, typename Compare >
  bool AVLTree< Key, Value, Compare >::ConstIterator::operator!=(const ConstIterator &rhs) const noexcept
  {
    return !(*this == rhs);
  }
  template <typename Key, typename Value, typename Compare>
  typename AVLTree<Key, Value, Compare>::ConstIterator &AVLTree<Key, Value, Compare>::ConstIterator::operator++()
  {
    assert(node_ != nullptr);

    switch (strategy_) {
    case IN_ORDER:
      nodesStack_.pop();
      pushAllLeft(node_->right);
      node_ = (nodesStack_.isEmpty() ? nullptr : nodesStack_.getTop());
      break;
    case REVERSE_IN_ORDER:
      nodesStack_.pop();
      pushAllRight(node_->left);
      node_ = (nodesStack_.isEmpty() ? nullptr : nodesStack_.getTop());
      break;
    case BREADTH:
      pushChildren(node_);
      nodesQueue_.pop();
      node_ = (nodesQueue_.isEmpty() ? nullptr : nodesQueue_.getNext());
      break;
    }

    return *this;
  }
  template <typename Key, typename Value, typename Compare>
  typename AVLTree<Key, Value, Compare>::ConstIterator AVLTree<Key, Value, Compare>::ConstIterator::operator++(int)
  {
    assert(node_ != nullptr);

    ConstIterator result = *this;
    ++(*this);

    return result;
  }
  template< typename Key, typename Value, typename Compare >
  void AVLTree< Key, Value, Compare >::ConstIterator::pushAllLeft(Node *node)
  {
    while (node) {
      nodesStack_.push(node);
      node = node->left;
    }
  }
  template< typename Key, typename Value, typename Compare >
  void AVLTree< Key, Value, Compare >::ConstIterator::pushAllRight(Node *node)
  {
    while (node) {
      nodesStack_.push(node);
      node = node->right;
    }
  }
  template< typename Key, typename Value, typename Compare >
  void AVLTree< Key, Value, Compare >::ConstIterator::pushChildren(Node *node)
  {
    if (node->left) {
      nodesQueue_.push(node->left);
    }
    if (node->right) {
      nodesQueue_.push(node->right);
    }
  }

  template< typename Key, typename Value, typename Compare >
  class AVLTree< Key, Value, Compare >::Iterator: public ConstIterator {
    public:
      using iterator_category = std::forward_iterator_tag;
      using value_type        = std::pair< Key, Value >;
      using difference_type   = std::ptrdiff_t;
      using pointer           = value_type*;
      using reference         = value_type&;

      explicit Iterator(Node *node, TraversalStrategy strategy = IN_ORDER);

      Iterator &operator++();
      Iterator operator++(int);
      reference operator*();
      pointer operator->();
  };
  template< typename Key, typename Value, typename Compare >
  AVLTree< Key, Value, Compare >::Iterator::Iterator(Node *node, TraversalStrategy strategy):
    ConstIterator(node, strategy)
  {}
  template <typename Key, typename Value, typename Compare>
  typename AVLTree<Key, Value, Compare>::Iterator &AVLTree<Key, Value, Compare>::Iterator::operator++()
  {
    ConstIterator::operator++();
    return *this;
  }
  template <typename Key, typename Value, typename Compare>
  typename AVLTree<Key, Value, Compare>::Iterator AVLTree<Key, Value, Compare>::Iterator::operator++(int)
  {
    Iterator result = *this;
    ConstIterator::operator++();
    return result;
  }
  template< typename Key, typename Value, typename Compare >
  typename AVLTree< Key, Value, Compare >::Iterator::reference AVLTree< Key, Value, Compare >::Iterator::operator*()
  {
    return const_cast< reference >(ConstIterator::operator*());
  }
  template< typename Key, typename Value, typename Compare >
  typename AVLTree< Key, Value, Compare >::Iterator::pointer AVLTree< Key, Value, Compare >::Iterator::operator->()
  {
    return std::pointer_traits< pointer >::pointer_to(**this);
  }
  template< typename Key, typename Value, typename Compare >
  AVLTree< Key, Value, Compare >::AVLTree(AVLTree< Key, Value, Compare > &&other) noexcept:
    root(other.root),
    size_(other.size_)
  {}
  template< typename Key, typename Value, typename Compare >
  AVLTree< Key, Value, Compare >::~AVLTree()
  {
    eraseAll(root);
  }
  template< typename Key, typename Value, typename Compare >
  AVLTree< Key, Value, Compare > &AVLTree< Key, Value, Compare >::operator=(const AVLTree< Key, Value, Compare > &rhs)
  {
    if (this != &rhs) {
      AVLTree< Key, Value, Compare > temp(rhs);
      swap(temp);
    }

    return *this;
  }
  template< typename Key, typename Value, typename Compare >
  AVLTree< Key, Value, Compare > &AVLTree< Key, Value, Compare >::operator=(AVLTree< Key, Value, Compare > &&rhs) noexcept
  {
    if (this != &rhs) {
      swap(rhs);
    }

    return *this;
  }
  template< typename Key, typename Value, typename Compare >
  Value &AVLTree< Key, Value, Compare >::operator[](const Key &key)
  {
    try {
      return at(key);
    } catch (const std::out_of_range &e) {
      insert({key, Value()});
      return at(key);
    }
  }
  template < typename Key, typename Value, typename Compare >
  template < typename F >
  F AVLTree< Key, Value, Compare >::traverse_lnr(F f) const
  {
    ConstIterator iter = cbegin(IN_ORDER);
    while (iter != cend()) {
      f(*iter);
      iter++;
    }
    return f;
  }
  template < typename Key, typename Value, typename Compare >
  template < typename F >
  F AVLTree< Key, Value, Compare >::traverse_rnl(F f) const
  {
    ConstIterator iter = cbegin(REVERSE_IN_ORDER);
    while (iter != cend()) {
      f(*iter);
      iter++;
    }
    return f;
  }
  template < typename Key, typename Value, typename Compare >
  template < typename F >
  F AVLTree< Key, Value, Compare >::traverse_breadth(F f) const
  {
    ConstIterator iter = cbegin(BREADTH);
    while (iter != cend()) {
      f(*iter);
      iter++;
    }
    return f;
  }
  template< typename Key, typename Value, typename Compare >
  void AVLTree< Key, Value, Compare >::insert(const Key &key, const Value &data)
  {
    root = insertIn(root, key, data);
    size_++;
  }
  template< typename Key, typename Value, typename Compare >
  void AVLTree< Key, Value, Compare >::insert(std::pair< Key, Value > &&pair)
  {
    insert(pair.first, pair.second);
  }
  template <typename Key, typename Value, typename Compare>
  template <class P>
  std::pair<typename AVLTree<Key, Value, Compare>::Iterator, bool> AVLTree<Key, Value, Compare>::insert(P &&value)
  {
    return emplace(std::forward<P>(value));
  }
  template<typename Key, typename Value, typename Compare>
  typename AVLTree< Key, Value, Compare >::Iterator AVLTree< Key, Value, Compare >::insert(ConstIterator pos,
                                                                                           const std::pair< Key, Value > &value)
  {
    insert(value.first, value.second);
    return getIter(value.first);
  }
  template< typename Key, typename Value, typename Compare >
  template< typename InputIt >
  void AVLTree< Key, Value, Compare >::insert(InputIt first, InputIt last) {
    for (InputIt it = first; it != last; ++it) {
      insert(*it);
    }
  }
  template< typename Key, typename Value, typename Compare >
  void AVLTree< Key, Value, Compare >::insert(std::initializer_list< std::pair< Key, Value > > ilist)
  {
    for (const auto& value : ilist) {
      insert(value);
    }
  }
  template< typename Key, typename Value, typename Compare >
  template< class... Args >
  std::pair< typename AVLTree< Key, Value, Compare >::Iterator, bool > AVLTree< Key, Value, Compare >::emplace(Args&&... args)
  {
    std::pair< Key, Value > pair(std::forward<Args>(args)...);
    Iterator it = getLowerBound(pair.first);

    Compare comparator;
    if (it == end() || comparator(pair.first, it->first)) {
      it = insert(it, std::move(pair));
      return std::make_pair(it, true);
    }

    return std::make_pair(it, false);
  }
  template< typename Key, typename Value, typename Compare >
  Value &AVLTree< Key, Value, Compare >::at(const Key &key)
  {
    Node *subtree = root;
    Compare comparator;
    while (getHeight(subtree)) {
      if (!comparator(key, subtree->data.first) && !comparator(subtree->data.first, key)) {
        return subtree->data.second;
      }

      if (comparator(key, subtree->data.first)) {
        subtree = subtree->left;
      } else {
        subtree = subtree->right;
      }
    }
    throw std::out_of_range("Error: No key found");
  }
  template< typename Key, typename Value, typename Compare >
  Value &AVLTree< Key, Value, Compare >::get(const Key &key)
  {
    return at(key);
  }
  template< typename Key, typename Value, typename Compare >
  typename AVLTree< Key, Value, Compare >::Iterator AVLTree< Key, Value, Compare >::find(const Key &key)
  {
    auto it = begin();
    Compare comparator;
    for (;it != end(); it++) {
      if (!comparator(key, it->first) && !comparator(it->first, key)) {
        return it;
      }
    }
    return it;
  }
  template< typename Key, typename Value, typename Compare >
  bool AVLTree< Key, Value, Compare >::contains(const Key &key) const
  {
    Node *subtree = root;
    Compare comparator;
    while (getHeight(subtree)) {
      if (!comparator(key, subtree->data.first) && !comparator(subtree->data.first, key)) {
        return true;
      }

      if (comparator(key, subtree->data.first)) {
        subtree = subtree->left;
      } else {
        subtree = subtree->right;
      }
    }
    return false;
  }
  template< typename Key, typename Value, typename Compare >
  std::size_t AVLTree< Key, Value, Compare >::getSize() const noexcept
  {
    return size_;
  }
  template< typename Key, typename Value, typename Compare >
  bool AVLTree< Key, Value, Compare >::isEmpty() const noexcept
  {
    return root == nullptr;
  }
  template< typename Key, typename Value, typename Compare >
  void AVLTree< Key, Value, Compare >::erase(const Key &key)
  {
    root = remove(root, key);
    size_--;
  }
  template <typename Key, typename Value, typename Compare>
  typename AVLTree< Key, Value, Compare >::Iterator
  AVLTree< Key, Value, Compare >::erase(typename AVLTree< Key, Value, Compare >::Iterator pos) {
    if (pos == end()) {
        return end();
    }
    Iterator next = pos;
    next++;
    erase(pos->first);
    return next;
  }
  template <typename Key, typename Value, typename Compare>
  typename AVLTree< Key, Value, Compare >::Iterator
  AVLTree< Key, Value, Compare >::erase(typename AVLTree< Key, Value, Compare >::Iterator first,
                                        typename AVLTree< Key, Value, Compare >::Iterator last)
  {
    if (first == last) {
        return iterator(const_cast<Node*>(first.node));
    }
    for (auto it = first; it != last; it++) {
      erase(it->first);
    }
    return last;
  }
  template <typename Key, typename Value, typename Compare>
  void AVLTree< Key, Value, Compare >::clear()
  {
    *this = AVLTree();
  }
  template< typename Key, typename Value, typename Compare >
  void AVLTree< Key, Value, Compare >::merge(const AVLTree< Key, Value, Compare > &other)
  {
    typename AVLTree< Key, Value, Compare >::ConstIterator ptr = other.cbegin();
    typename AVLTree< Key, Value, Compare >::ConstIterator end = other.cend();
    while (ptr != end) {
      insert(ptr->first, ptr->second);
      ptr++;
    }
  }
  template< typename Key, typename Value, typename Compare >
  typename AVLTree< Key, Value, Compare >::Iterator AVLTree< Key, Value, Compare >::getUpperBound(const Key& key)
  {
    Iterator itr = getLowerBound(key);
    if (!comparator(key, itr->first) && !comparator(itr->first, key)) {
      itr++;
    }
    return itr;
  }
  template< typename Key, typename Value, typename Compare >
  typename AVLTree< Key, Value, Compare >::Iterator AVLTree< Key, Value, Compare >::getLowerBound(const Key& key)
  {
    Iterator itr = begin();
    Compare comparator;
    while (itr != end) {
      if (!comparator(key, itr->first)) {
        return itr;
      }
      itr++;
    }
    return itr;
  }
  template< typename Key, typename Value, typename Compare >
  typename AVLTree< Key, Value, Compare >::Iterator AVLTree< Key, Value, Compare >::begin(TraversalStrategy strategy)
  {
    return Iterator(root, strategy);
  }
  template< typename Key, typename Value, typename Compare >
  typename AVLTree< Key, Value, Compare >::ConstIterator AVLTree< Key, Value, Compare >::cbegin(TraversalStrategy strategy) const
  {
    return ConstIterator(root, strategy);
  }
  template< typename Key, typename Value, typename Compare >
  typename AVLTree< Key, Value, Compare >::Iterator AVLTree< Key, Value, Compare >::end()
  {
    return Iterator(nullptr);
  }
  template< typename Key, typename Value, typename Compare >
  typename AVLTree< Key, Value, Compare >::ConstIterator AVLTree< Key, Value, Compare >::cend() const
  {
    return ConstIterator(nullptr);
  }

  template< typename Key, typename Value, typename Compare >
  typename AVLTree< Key, Value, Compare >::Node *AVLTree< Key, Value, Compare >::insertIn(Node *tree, const Key &key, const Value &data)
  {
    if (!tree) {
      return new Node(key, data);
    }

    Compare comparator;
    if (!comparator(key, tree->data.first) && !comparator(tree->data.first, key)) {
      tree->data.second = data;
      return tree;
    }

    if (comparator(key, tree->data.first)) {
      tree->left = insertIn(tree->left, key, data);
    } else {
      tree->right = insertIn(tree->right, key, data);
    }

    return balanceTree(tree);
  }
  template< typename Key, typename Value, typename Compare >
  unsigned char AVLTree< Key, Value, Compare >::getHeight(Node *tree) const
  {
    return (tree ? tree->height : 0);
  }
  template< typename Key, typename Value, typename Compare >
  int AVLTree< Key, Value, Compare >::getBalanceFactor(Node *tree)
  {
    return getHeight(tree->right) - getHeight(tree->left);
  }
  template< typename Key, typename Value, typename Compare >
  void AVLTree< Key, Value, Compare >::setHeight(Node *tree)
  {
    tree->height = std::max(getHeight(tree->right), getHeight(tree->left)) + 1;
  }
  template< typename Key, typename Value, typename Compare >
  typename AVLTree< Key, Value, Compare >::Node *AVLTree< Key, Value, Compare >::rotateRight(Node *tree)
  {
    Node *newRoot = tree->left;
    tree->left = newRoot->right;
    newRoot->right = tree;
    setHeight(tree);
    setHeight(newRoot);
    return newRoot;
  }
  template< typename Key, typename Value, typename Compare >
  typename AVLTree< Key, Value, Compare >::Node *AVLTree< Key, Value, Compare >::rotateLeft(Node *tree)
  {
    Node *newRoot = tree->right;
    tree->right = newRoot->left;
    newRoot->left = tree;
    setHeight(tree);
    setHeight(newRoot);
    return newRoot;
  }
  template< typename Key, typename Value, typename Compare >
  typename AVLTree< Key, Value, Compare >::Node *AVLTree< Key, Value, Compare >::rotateRightLeft(Node *tree)
  {
    tree->right = rotateRight(tree->right);
    tree = rotateLeft(tree);
    return tree;
  }
  template< typename Key, typename Value, typename Compare >
  typename AVLTree< Key, Value, Compare >::Node *AVLTree< Key, Value, Compare >::rotateLeftRight(Node *tree)
  {
    tree->left = rotateLeft(tree->left);
    tree = rotateRight(tree);
    return tree;
  }
  template< typename Key, typename Value, typename Compare >
  typename AVLTree< Key, Value, Compare >::Node *AVLTree< Key, Value, Compare >::balanceTree(Node *tree)
  {
    int bFactor = getBalanceFactor(tree);

    if (bFactor == 2) {
      if (getBalanceFactor(tree->right) < 0) {
        return rotateRightLeft(tree);
      }
      return rotateLeft(tree);
    }
    if (bFactor == -2) {
      if (getBalanceFactor(tree->left) > 0) {
        return rotateLeftRight(tree);
      }
      return rotateRight(tree);
    }

    return tree;
  }
  template< typename Key, typename Value, typename Compare >
  void AVLTree< Key, Value, Compare >::eraseAll(Node *tree)
  {
    if (!tree) {
      return;
    }

    eraseAll(tree->left);
    eraseAll(tree->right);

    delete tree;

    return;
  }
  template< typename Key, typename Value, typename Compare >
  typename AVLTree< Key, Value, Compare >::Node *AVLTree< Key, Value, Compare >::remove(Node *subtree, const Key &key)
  {
    if (!subtree) {
      return subtree;
    }
    Compare comparator;
    if (!comparator(key, subtree->data.first) && !comparator(subtree->data.first, key)) {
      Node *right = subtree->right;
      Node *left = subtree->left;
      delete subtree;

      if (!right) {
        return left;
      }

      Node *replacer = findMin(right);
      replacer->right = removeNoDeleteMin(right);
      replacer->left = left;
      return balanceTree(replacer);
    }

    if (comparator(key, subtree->data.first)) {
      subtree->left = remove(subtree->left, key);
    } else {
      subtree->right = remove(subtree->right, key);
    }
    return balanceTree(subtree);
  }
  template <typename Key, typename Value, typename Compare>
  typename AVLTree<Key, Value, Compare>::Iterator AVLTree<Key, Value, Compare>::getIter(const Key &key)
  {
    Iterator itr = begin();
    Compare comparator;
    while (itr != end()) {
      if (!comparator(key, itr->first) && !comparator(itr->first, key)) {
        return itr;
      }
      itr++;
    }
    return itr;
  }
  template< typename Key, typename Value, typename Compare >
  typename AVLTree< Key, Value, Compare >::Node *AVLTree< Key, Value, Compare >::removeNoDeleteMin(Node *tree)
  {
    if (!tree->left) {
      return tree->right;
    }

    tree->left = removeNoDeleteMin(tree->left);
    return balanceTree(tree);
  }
  template< typename Key, typename Value, typename Compare >
  typename AVLTree< Key, Value, Compare >::Node *AVLTree< Key, Value, Compare >::findMin(Node *tree)
  {
    return (tree->left ? findMin(tree->left) : tree);
  }

  template< typename Key, typename Value, typename Compare >
  void AVLTree< Key, Value, Compare >::swap(AVLTree< Key, Value, Compare > &other) noexcept
  {
    std::swap(root, other.root);
    std::swap(size_, other.size_);
  }
}

#endif
