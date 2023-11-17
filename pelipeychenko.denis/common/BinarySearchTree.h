#ifndef BINARYSEARCHTREE_H
#define BINARYSEARCHTREE_H

#include <string>
#include "Stack.h"
#include "pair.h"

namespace pelipeychenko
{
  template< typename Key, typename Value, typename Comparator = std::less< Key > >
  class BinarySearchTree
  {
  public:
    class ConstIterator;

    class Iterator;

    BinarySearchTree();

    BinarySearchTree(const BinarySearchTree< Key, Value, Comparator > &instance);

    BinarySearchTree(BinarySearchTree< Key, Value, Comparator > &&instance) noexcept;

    BinarySearchTree(std::initializer_list< pair< Key, Value > > list);

    ~BinarySearchTree();

    BinarySearchTree &operator=(const BinarySearchTree< Key, Value, Comparator > &instance);

    BinarySearchTree &operator=(BinarySearchTree< Key, Value, Comparator > &&instance) noexcept;

    bool operator==(const BinarySearchTree< Key, Value, Comparator > &instance) const noexcept;

    template< typename F >
    F traverse(F f) const;

    ConstIterator cbegin() const noexcept;

    ConstIterator cend() const noexcept;

    Iterator begin();

    Iterator end();

    ConstIterator merge(const Key &key, const Value &value);

    void setTraversalMode(std::string &mode);

    template< typename...Args >
    ConstIterator emplace(Args &&... args);

    const Value &get(const Key &key) const;

    const Value &getMin() const;

    const Value &getMax() const;

    Iterator lowerBound(const Key &key);

    Iterator upperBound(const Key &key);

    Iterator find(const Key &key) noexcept;

    ConstIterator find(const Key &key) const noexcept;

    void erase(const Key &key);

    bool isEmpty() const noexcept;

    std::size_t getSize() const noexcept;

    void inlinePrint() const noexcept;

    void print() const noexcept;

  private:
    enum TraversalMode
    {
      ASCENDING = 0,
      DESCENDING = 1,
      BREADTH = 2
    };

    struct Node
    {
      pair< Key, Value > data_;
      Node *left_;
      Node *right_;
      int height_;

      Node(const Key &key, const Value &value):
        data_({key, value}),
        left_(nullptr),
        right_(nullptr),
        height_(0)
      {}

      const Key &getKey()
      {
        return data_.first_;
      };

      const Value &getValue()
      {
        return data_.second_;
      };
    };

    Stack< Node * > path_;
    Node *head_;
    size_t size_;
    TraversalMode mode_;

    Node *insert(Node *node, const Key &key, const Value &value);

    Node *getMinNode(Node *) const;

    Node *getMaxNode(Node *) const;

    void deleteTree(Node *);

    Node *deleteNode(Node *, const Key &key);

    Node *searchNode(Node *node, const Key &key) const;

    void printNodes(Node *node) const;

    Node *rotateRight(Node *node);

    Node *rotateLeft(Node *node);

    int getHeight(Node *node) const;

    int getBalance(Node *node) const;

    int getNewHeight(Node *node) const;

    Node *balance(Node *node);

    void swap(BinarySearchTree< Key, Value, Comparator > &instance) noexcept;
  };

  template< typename Key, typename Value, typename Comparator >
  BinarySearchTree< Key, Value, Comparator >::BinarySearchTree() :
    head_(nullptr),
    size_(0),
    mode_(ASCENDING)
  {}

  template< typename Key, typename Value, typename Comparator >
  BinarySearchTree< Key, Value, Comparator >::BinarySearchTree(
    const BinarySearchTree< Key, Value, Comparator > &instance) :
    head_(nullptr),
    size_(0),
    mode_(instance.mode_)
  {
    for (auto it = instance.cbegin(); it != instance.cend(); it++) {
      merge(it->first_, it->second_);
    }
  }

  template< typename Key, typename Value, typename Comparator >
  BinarySearchTree< Key, Value, Comparator >::BinarySearchTree(
    BinarySearchTree< Key, Value, Comparator > &&instance) noexcept :
    head_(instance.head_),
    size_(instance.size_),
    mode_(instance.mode_)
  {
    instance.head_ = nullptr;
    instance.size_ = 0;
  }

  template< typename Key, typename Value, typename Comparator >
  BinarySearchTree< Key, Value, Comparator >::BinarySearchTree(std::initializer_list< pair< Key, Value > > list) :
    head_(nullptr),
    size_(0),
    mode_(ASCENDING)
  {
    for (auto it: list) {
      merge(it.first_, it.second_);
    }
  }

  template< typename Key, typename Value, typename Comparator >
  BinarySearchTree< Key, Value, Comparator >::~BinarySearchTree()
  {
    deleteTree(head_);
  }

  template< typename Key, typename Value, typename Comparator >
  BinarySearchTree< Key, Value, Comparator > &
  BinarySearchTree< Key, Value, Comparator >::operator=(const BinarySearchTree< Key, Value, Comparator > &instance)
  {
    if (this != &instance) {
      BinarySearchTree< Key, Value > temp(instance);
      swap(temp);
    }
    return *this;
  }

  template< typename Key, typename Value, typename Comparator >
  BinarySearchTree< Key, Value, Comparator > &
  BinarySearchTree< Key, Value, Comparator >::operator=(BinarySearchTree< Key, Value, Comparator > &&instance) noexcept
  {
    if (this != &instance) {
      swap(instance);
    }
    return *this;
  }

  template< typename Key, typename Value, typename Comparator >
  bool BinarySearchTree< Key, Value, Comparator >::operator==(
    const BinarySearchTree< Key, Value, Comparator > &instance) const noexcept
  {
    if (getSize() != instance.getSize()) {
      return false;
    }
    for (auto it = cbegin(), yeIt = instance.cbegin(); it != cend(); it++, yeIt++) {
      if (*it != *yeIt) {
        return false;
      }
    }
    return true;
  }

  template< typename Key, typename Value, typename Comparator >
  template< typename F >
  F BinarySearchTree< Key, Value, Comparator >::traverse(F f) const
  {
    for (auto it = cbegin(); it != cend(); it++) {
      f(*it);
    }
    return f;
  }

  template< typename Key, typename Value, typename Comparator >
  typename BinarySearchTree< Key, Value, Comparator >::ConstIterator
  BinarySearchTree< Key, Value, Comparator >::cbegin() const noexcept
  {
    switch (mode_) {
      case (ASCENDING) :
        return ConstIterator(head_, getMinNode(head_), mode_);
      case DESCENDING :
        return ConstIterator(head_, getMaxNode(head_), mode_);
      case BREADTH :
        return ConstIterator(head_, mode_);
      default :
        return ConstIterator();
    }
  }

  template< typename Key, typename Value, typename Comparator >
  typename BinarySearchTree< Key, Value, Comparator >::ConstIterator
  BinarySearchTree< Key, Value, Comparator >::cend() const noexcept
  {
    return ConstIterator();
  }

  template< typename Key, typename Value, typename Comparator >
  typename BinarySearchTree< Key, Value, Comparator >::Iterator BinarySearchTree< Key, Value, Comparator >::begin()
  {
    switch (mode_) {
      case (ASCENDING) :
        return Iterator(head_, getMinNode(head_), mode_);
      case DESCENDING :
        return Iterator(head_, getMaxNode(head_), mode_);
      case BREADTH :
        return Iterator(head_, mode_);
      default :
        return Iterator();
    }
  }

  template< typename Key, typename Value, typename Comparator >
  typename BinarySearchTree< Key, Value, Comparator >::Iterator BinarySearchTree< Key, Value, Comparator >::end()
  {
    return Iterator();
  }

  template< typename Key, typename Value, typename Comparator >
  typename BinarySearchTree< Key, Value, Comparator >::Node *
  BinarySearchTree< Key, Value, Comparator >::insert(Node *node, const Key &key, const Value &value)
  {
    Comparator comparator;
    path_.push(node);
    if (comparator(key, node->getKey())) {
      if (node->left_ == nullptr) {
        node->left_ = new Node(key, value);
        return node->left_;
      } else {
        return insert(node->left_, key, value);
      }
    } else {
      if (node->right_ == nullptr) {
        node->right_ = new Node(key, value);
        return node->right_;
      } else {
        return insert(node->right_, key, value);
      }
    }
  }

  template< typename Key, typename Value, typename Comparator >
  typename BinarySearchTree< Key, Value, Comparator >::ConstIterator
  BinarySearchTree< Key, Value, Comparator >::merge(const Key &key, const Value &value)
  {
    if (find(key) != end()) {
      erase(key);
    }
    size_++;
    if (head_ == nullptr) {
      head_ = new Node(key, value);
      return ConstIterator(head_, mode_);
    }
    Node *newNode = insert(head_, key, value);
    while (!path_.isEmpty()) {
      Node *node = path_.getHead();
      node = balance(node);
      node->height_ = getNewHeight(node);
      path_.drop();
    }
    return ConstIterator(head_, newNode, mode_);
  }

  template< typename Key, typename Value, typename Comparator >
  void BinarySearchTree< Key, Value, Comparator >::setTraversalMode(std::string &mode)
  {
    if (mode == "ascending") {
      mode_ = BinarySearchTree< int, std::string >::TraversalMode::ASCENDING;
    } else if (mode == "descending") {
      mode_ = BinarySearchTree< int, std::string >::TraversalMode::DESCENDING;
    } else if (mode == "breadth") {
      mode_ = BinarySearchTree< int, std::string >::TraversalMode::BREADTH;
    } else {
      throw std::runtime_error("Invalid argument");
    }
  }

  template< typename Key, typename Value, typename Comparator >
  template< typename ... Args >
  typename BinarySearchTree< Key, Value, Comparator >::ConstIterator
  BinarySearchTree< Key, Value, Comparator >::emplace(Args &&...args)
  {
    return ConstIterator(head_, insert(head_, Key(args...), Value(args...)), mode_);
  }

  template< typename Key, typename Value, typename Comparator >
  const Value &BinarySearchTree< Key, Value, Comparator >::get(const Key &key) const
  {
    return searchNode(head_, key)->getValue();
  }

  template< typename Key, typename Value, typename Comparator >
  void BinarySearchTree< Key, Value, Comparator >::erase(const Key &key)
  {
    head_ = deleteNode(head_, key);
    size_--;
    while (!path_.isEmpty()) {
      Node *node = path_.getHead();
      node->height_ = getNewHeight(node);
      path_.drop();
    }
  }

  template< typename Key, typename Value, typename Comparator >
  const Value &BinarySearchTree< Key, Value, Comparator >::getMin() const
  {
    return getMinNode(head_)->value_;
  }

  template< typename Key, typename Value, typename Comparator >
  const Value &BinarySearchTree< Key, Value, Comparator >::getMax() const
  {
    return getMaxNode(head_)->value_;
  }

  template< typename Key, typename Value, typename Comparator >
  typename BinarySearchTree< Key, Value, Comparator >::Iterator
  BinarySearchTree< Key, Value, Comparator >::lowerBound(const Key &key)
  {
    Comparator comparator;
    auto it = begin();
    for (; it != end() && comparator(it->first_, key); it++);
    return it;
  }

  template< typename Key, typename Value, typename Comparator >
  typename BinarySearchTree< Key, Value, Comparator >::Iterator
  BinarySearchTree< Key, Value, Comparator >::upperBound(const Key &key)
  {
    Comparator comparator;
    auto it = lowerBound(key);
    for (; it != end() && it->first_ == key; it++);
    return it;
  }

  template< typename Key, typename Value, typename Comparator >
  typename BinarySearchTree< Key, Value, Comparator >::Iterator
  BinarySearchTree< Key, Value, Comparator >::find(const Key &key) noexcept
  {
    auto searchedNode = searchNode(head_, key);
    return Iterator(head_, searchedNode, mode_);
  }

  template< typename Key, typename Value, typename Comparator >
  typename BinarySearchTree< Key, Value, Comparator >::ConstIterator
  BinarySearchTree< Key, Value, Comparator >::find(const Key &key) const noexcept
  {
    return ConstIterator(head_, searchNode(head_, key), mode_);
  }

  template< typename Key, typename Value, typename Comparator >
  void BinarySearchTree< Key, Value, Comparator >::inlinePrint() const noexcept
  {
    printNodes(head_);
  }

  template< typename Key, typename Value, typename Comparator >
  void BinarySearchTree< Key, Value, Comparator >::print() const noexcept
  {
    for (auto it = cbegin(); it != cend(); it++) {
      std::cout << it->first_ << " " << it->second_ << "\n";
    }
  }

  template< typename Key, typename Value, typename Comparator >
  bool BinarySearchTree< Key, Value, Comparator >::isEmpty() const noexcept
  {
    return size_ == 0;
  }

  template< typename Key, typename Value, typename Comparator >
  size_t BinarySearchTree< Key, Value, Comparator >::getSize() const noexcept
  {
    return size_;
  }

  template< typename Key, typename Value, typename Comparator >
  typename BinarySearchTree< Key, Value, Comparator >::Node *
  BinarySearchTree< Key, Value, Comparator >::getMinNode(Node *node) const
  {
    if (node == nullptr) {
      return nullptr;
    }
    if (node->left_ == nullptr) {
      return node;
    }
    return getMinNode(node->left_);
  }

  template< typename Key, typename Value, typename Comparator >
  typename BinarySearchTree< Key, Value, Comparator >::Node *
  BinarySearchTree< Key, Value, Comparator >::getMaxNode(Node *node) const
  {
    if (node == nullptr) {
      throw std::runtime_error("getMaxNode: node is nullptr");
    }
    if (node->right_ == nullptr) {
      return node;
    }
    return getMaxNode(node->right_);
  }

  template< typename Key, typename Value, typename Comparator >
  typename BinarySearchTree< Key, Value, Comparator >::Node *
  BinarySearchTree< Key, Value, Comparator >::searchNode(BinarySearchTree::Node *node, const Key &key) const
  {
    Comparator comparator;
    if (node == nullptr) {
      return nullptr;
    }
    if (node->getKey() == key) {
      return node;
    }
    return (comparator(key, node->getKey()) ? searchNode(node->left_, key) : searchNode(node->right_, key));
  }

  template< typename Key, typename Value, typename Comparator >
  void BinarySearchTree< Key, Value, Comparator >::deleteTree(BinarySearchTree::Node *node)
  {
    if (node != nullptr) {
      deleteTree(node->left_);
      deleteTree(node->right_);
      delete node;
    }
  }

  template< typename Key, typename Value, typename Comparator >
  typename BinarySearchTree< Key, Value, Comparator >::Node *
  BinarySearchTree< Key, Value, Comparator >::deleteNode(BinarySearchTree::Node *node, const Key &key)
  {
    Comparator comparator;
    if (node == nullptr) {
      return nullptr;
    }
    if (key == node->getKey()) {
      if (node->left_ == nullptr || node->right_ == nullptr) {
        Node *tempNode = (node->left_ == nullptr ? node->right_ : node->left_);
        delete node;
        node = tempNode;
      } else {
        Node *minInRight = getMinNode(node->right_);
        node->data_ = minInRight->data_;
        node->right_ = deleteNode(node->right_, minInRight->getKey());
      }
    } else if (!comparator(key, node->getKey())) {
      path_.push(node);
      node->right_ = deleteNode(node->right_, key);
    } else {
      path_.push(node);
      node->left_ = deleteNode(node->left_, key);
    }

    if (node != nullptr) {
      node = balance(node);
      node->height_ = getNewHeight(node);
    }
    return node;
  }

  template< typename Key, typename Value, typename Comparator >
  void
  BinarySearchTree< Key, Value, Comparator >::printNodes(BinarySearchTree< Key, Value, Comparator >::Node *node) const
  {
    if (node == nullptr) {
      return;
    }
    printNodes(node->left_);
    std::cout << " " << node->getKey() << " " << node->getValue();
    printNodes(node->right_);
  }

  template< typename Key, typename Value, typename Comparator >
  int BinarySearchTree< Key, Value, Comparator >::getHeight(BinarySearchTree::Node *node) const
  {
    return (node == nullptr ? -1 : node->height_);
  }

  template< typename Key, typename Value, typename Comparator >
  int BinarySearchTree< Key, Value, Comparator >::getNewHeight(BinarySearchTree::Node *node) const
  {
    return (getHeight(node->right_) > getHeight(node->left_) ? getHeight(node->right_) : getHeight(node->left_)) + 1;
  }

  template< typename Key, typename Value, typename Comparator >
  typename BinarySearchTree< Key, Value, Comparator >::Node *
  BinarySearchTree< Key, Value, Comparator >::rotateRight(BinarySearchTree::Node *node)
  {
    pair< Key, Value > tempData(node->getKey(), node->getValue());
    node->data_ = node->left_->data_;
    node->left_->data_ = tempData;

    Node *tempNode = node->right_;
    node->right_ = node->left_;
    node->left_ = node->right_->left_;
    node->right_->left_ = node->right_->right_;
    node->right_->right_ = tempNode;
    node->height_ = getNewHeight(node);
    node->right_->height_ = getNewHeight(node->right_);
    return node;
  }

  template< typename Key, typename Value, typename Comparator >
  typename BinarySearchTree< Key, Value, Comparator >::Node *
  BinarySearchTree< Key, Value, Comparator >::rotateLeft(BinarySearchTree::Node *node)
  {
    pair< Key, Value > tempData(node->getKey(), node->getValue());
    node->data_ = node->right_->data_;
    node->right_->data_ = tempData;

    Node *tempNode = node->left_;
    node->left_ = node->right_;
    node->right_ = node->left_->right_;
    node->left_->right_ = node->left_->left_;
    node->left_->left_ = tempNode;
    node->height_ = getNewHeight(node);
    node->left_->height_ = getNewHeight(node->left_);
    return node;
  }

  template< typename Key, typename Value, typename Comparator >
  int BinarySearchTree< Key, Value, Comparator >::getBalance(BinarySearchTree::Node *node) const
  {
    if (node == nullptr) {
      return 0;
    }
    return getHeight(node->right_) - getHeight(node->left_);
  }

  template< typename Key, typename Value, typename Comparator >
  typename BinarySearchTree< Key, Value, Comparator >::Node *
  BinarySearchTree< Key, Value, Comparator >::balance(BinarySearchTree::Node *node)
  {
    int balance = getBalance(node);
    if (balance == -2) {
      if (getBalance(node->left_) == 1) {
        node->left_ = rotateLeft(node->left_);
      }
      node = rotateRight(node);
      return node;
    }

    if (balance == 2) {
      if (getBalance(node->right_) == -1) {
        node->right_ = rotateRight(node->right_);
      }
      node = rotateLeft(node);
    }
    return node;
  }

  template< typename Key, typename Value, typename Comparator >
  void pelipeychenko::BinarySearchTree< Key, Value, Comparator >::swap(
    pelipeychenko::BinarySearchTree< Key, Value, Comparator > &instance) noexcept
  {
    std::swap(head_, instance.head_);
  }
}

#endif
