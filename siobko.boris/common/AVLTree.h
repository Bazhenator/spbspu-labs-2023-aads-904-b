
#ifndef AVLTREE_H
#define AVLTREE_H

#include <iostream>

#include "Stack.h"
#include "Queue.h"
#include "NodeFunctor.h"

namespace siobko {
  enum class TraversalStrategy {
    ASCENDING,
    DESCENDING,
    BREADTH
  };

  template< typename Key, typename Value, typename Compare = std::less< Key > >
  class AVLTree {
  public:
    class ConstIterator;
    class Iterator;

    using size_type = std::size_t;
    using value_type = std::pair< Key, Value >;
    using iterator = Iterator;
    using const_iterator = ConstIterator;

    AVLTree();
    AVLTree(const AVLTree& rhs);
    AVLTree(AVLTree&& rhs) noexcept;
    AVLTree(std::initializer_list< value_type > IList) noexcept;
    ~AVLTree();

    AVLTree& operator=(const AVLTree& other);
    AVLTree& operator=(AVLTree&& other) noexcept;

    iterator begin();
    iterator end();
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;
    const_iterator cbegin(TraversalStrategy strategy = TraversalStrategy::ASCENDING) const noexcept;
    const_iterator cend() const noexcept;

    NodeFunctor traverse(NodeFunctor f, TraversalStrategy strategy) const;
    const Value& get(const Key& key);
    void push(const Key& key, const Value& value);
    void merge(const AVLTree& other);
    void clear();
    bool contains(const Key& key) const noexcept;
    bool isEmpty() const noexcept;
    void remove(const Key& key, const Value& value);
    void print() const noexcept;

  private:
    struct Node {
      Node(const Key& key, const Value& value) :
        value_(std::pair< Key, Value >(key, value)),
        left_(nullptr),
        right_(nullptr),
        height_(1)
      {}

      std::pair< Key, Value > value_;
      Node *left_;
      Node *right_;
      std::uint8_t height_;
    };

    void clear(Node *node);
    void fixHeight(Node *node);
    Node *insert(Node *node, const Key& key, const Value& value);
    Node *rotateRight(Node *node);
    Node *removeMin(Node *node);
    Node *find(const Key& key) const noexcept;
    Node *rotateLeft(Node *node);
    Node *doubleLeftRotate(Node *node);
    Node *doubleRightRotate(Node *node);
    Node *balance(Node *node);
    Node *findMin(Node *node);
    Node *remove(Node *node, const Key& key);
    int getHeight(Node *node);
    int getBalance(Node *node);

    Node *root_;
    Compare comp_;
    size_type size_;
  };

  template< typename Key, typename Value, typename Compare >
  struct AVLTree< Key, Value, Compare >::ConstIterator {
    using const_reference = const std::pair< Key, Value >&;
    using pointer = const std::pair< Key, Value > *;

    ConstIterator() = default;
    explicit ConstIterator(Node *node, TraversalStrategy strategy = TraversalStrategy::ASCENDING);
    ~ConstIterator() = default;

    ConstIterator& operator++();
    ConstIterator operator++(int);
    const_reference operator*();
    pointer operator->();
    bool operator==(const ConstIterator& other);
    bool operator!=(const ConstIterator& other);

    Node *current_;
    TraversalStrategy strategy_ = TraversalStrategy::ASCENDING;
    Stack< Node * > stack_;
    Queue< Node * > queue_;
  };

  template< typename Key, typename Value, typename Compare >
  AVLTree< Key, Value, Compare >::ConstIterator::ConstIterator(Node *node, TraversalStrategy strategy):
    current_(node),
    strategy_(strategy)
  {
    if (strategy_ == TraversalStrategy::BREADTH) {
      queue_.push(current_);
    } else {
      if (strategy_ == TraversalStrategy::ASCENDING) {
        while (current_ != nullptr) {
          stack_.push(current_);
          current_ = current_->left_;
        }
      } else {
        while (current_ != nullptr) {
          stack_.push(current_);
          current_ = current_->right_;
        }
      }
      current_ = (stack_.isEmpty()) ? nullptr : stack_.front();
    }
  }

  template< typename Key, typename Value, typename Compare >
  typename AVLTree< Key, Value, Compare >::ConstIterator& AVLTree< Key, Value, Compare >::ConstIterator::operator++()
  {
    if (strategy_ == TraversalStrategy::BREADTH) {
      if (current_->left_ != nullptr) {
        queue_.push(current_->left_);
      }
      if (current_->right_ != nullptr) {
        queue_.push(current_->right_);
      }
      queue_.pop();
      current_ = queue_.isEmpty() ? nullptr : queue_.front();
    } else {
      stack_.pop();
      if (strategy_ == TraversalStrategy::ASCENDING) {
        Node *node = current_->right_;
        while (node != nullptr) {
          stack_.push(node);
          node = node->left_;
        }
      } else {
        Node *node = current_->left_;
        while (node != nullptr) {
          stack_.push(node);
          node = node->right_;
        }
      }
      current_ = (stack_.isEmpty()) ? nullptr : stack_.front();
    }
    return *this;
  }

  template< typename Key, typename Value, typename Compare >
  typename AVLTree< Key, Value, Compare >::ConstIterator AVLTree< Key, Value, Compare >::ConstIterator::operator++(int)
  {
    ConstIterator tmp = *this;
    ++(*this);
    return tmp;
  }

  template< typename Key, typename Value, typename Compare >
  typename AVLTree< Key, Value, Compare >::ConstIterator::const_reference AVLTree< Key, Value, Compare >::ConstIterator::operator*()
  {
    return current_->value_;
  }

  template< typename Key, typename Value, typename Compare >
  typename AVLTree< Key, Value, Compare >::ConstIterator::pointer AVLTree< Key, Value, Compare >::ConstIterator::operator->()
  {
    return std::pointer_traits< pointer >::pointer_to(**this);
  }

  template< typename Key, typename Value, typename Compare >
  bool AVLTree< Key, Value, Compare >::ConstIterator::operator==(const ConstIterator& other)
  {
    return current_ == other.current_;
  }

  template< typename Key, typename Value, typename Compare >
  bool AVLTree< Key, Value, Compare >::ConstIterator::operator!=(const ConstIterator& other)
  {
    return current_ != other.current_;
  }

  template< typename Key, typename Value, typename Compare >
  struct AVLTree< Key, Value, Compare >::Iterator {
    using reference = std::pair< Key, Value >&;
    using pointer = std::pair< Key, Value > *;

    Iterator() = default;
    explicit Iterator(Node *node, TraversalStrategy strategy = TraversalStrategy::ASCENDING);
    ~Iterator() = default;

    Iterator& operator++();
    Iterator operator++(int);
    reference operator*();
    pointer operator->();
    bool operator==(const Iterator& other);
    bool operator!=(const Iterator& other);

    Node *current_;
    TraversalStrategy strategy_ = TraversalStrategy::ASCENDING;
    Stack< Node * > stack_;
    Queue< Node * > queue_;
  };

  template< typename Key, typename Value, typename Compare >
  AVLTree< Key, Value, Compare >::Iterator::Iterator(Node *node, TraversalStrategy strategy):
    current_(node),
    strategy_(strategy)
  {
    if (strategy_ == TraversalStrategy::BREADTH) {
      queue_.push(current_);
    } else {
      if (strategy_ == TraversalStrategy::ASCENDING) {
        while (current_ != nullptr) {
          stack_.push(current_);
          current_ = current_->left_;
        }
      } else {
        while (current_ != nullptr) {
          stack_.push(current_);
          current_ = current_->right_;
        }
      }
      current_ = (stack_.isEmpty()) ? nullptr : stack_.front();
    }
  }

  template< typename Key, typename Value, typename Compare >
  typename AVLTree< Key, Value, Compare >::Iterator& AVLTree< Key, Value, Compare >::Iterator::operator++()
  {
    if (strategy_ == TraversalStrategy::BREADTH) {
      if (current_->left_ != nullptr) {
        queue_.push(current_->left_);
      }
      if (current_->right_ != nullptr) {
        queue_.push(current_->right_);
      }
      queue_.pop();
      current_ = queue_.isEmpty() ? nullptr : queue_.front();
    } else {
      stack_.pop();
      if (strategy_ == TraversalStrategy::ASCENDING) {
        Node *node = current_->right_;
        while (node != nullptr) {
          stack_.push(node);
          node = node->left_;
        }
      } else {
        Node *node = current_->left_;
        while (node != nullptr) {
          stack_.push(node);
          node = node->right_;
        }
      }
      current_ = (stack_.isEmpty()) ? nullptr : stack_.front();
    }
    return *this;
  }

  template< typename Key, typename Value, typename Compare >
  typename AVLTree< Key, Value, Compare >::Iterator AVLTree< Key, Value, Compare >::Iterator::operator++(int)
  {
    Iterator tmp = *this;
    ++(*this);
    return tmp;
  }

  template< typename Key, typename Value, typename Compare >
  typename AVLTree< Key, Value, Compare >::Iterator::reference AVLTree< Key, Value, Compare >::Iterator::operator*()
  {
    return current_->value_;
  }

  template< typename Key, typename Value, typename Compare >
  typename AVLTree< Key, Value, Compare >::Iterator::pointer AVLTree< Key, Value, Compare >::Iterator::operator->()
  {
    return std::pointer_traits< pointer >::pointer_to(**this);
  }

  template< typename Key, typename Value, typename Compare >
  bool AVLTree< Key, Value, Compare >::Iterator::operator==(const Iterator& other)
  {
    return current_ == other.current_;
  }

  template< typename Key, typename Value, typename Compare >
  bool AVLTree< Key, Value, Compare >::Iterator::operator!=(const Iterator& other)
  {
    return current_ != other.current_;
  }

  template< typename Key, typename Value, typename Compare >
  AVLTree< Key, Value, Compare >::AVLTree():
    root_(nullptr),
    size_(0u)
  {}

  template< typename Key, typename Value, typename Compare >
  AVLTree< Key, Value, Compare >::AVLTree(const AVLTree& rhs):
    root_(nullptr),
    size_(0u)
  {
    for (const_iterator it = rhs.cbegin(); it != rhs.cend(); ++it) {
      push(it->first, it->second);
    }
  }

  template< typename Key, typename Value, typename Compare >
  AVLTree< Key, Value, Compare >::AVLTree(AVLTree&& rhs) noexcept:
    root_(nullptr),
    size_(0)
  {
    std::swap(rhs.root_, root_);
    std::swap(rhs.size_, size_);
  }

  template< typename Key, typename Value, typename Compare >
  AVLTree< Key, Value, Compare >::AVLTree(std::initializer_list< value_type > IList) noexcept:
    root_(nullptr),
    size_(0u)
  {
    for (auto item: IList) {
      push(item.first, item.second);
    }
  }

  template< typename Key, typename Value, typename Compare >
  AVLTree< Key, Value, Compare >::~AVLTree()
  {
    clear(root_);
  }

  template< typename Key, typename Value, typename Compare >
  AVLTree< Key, Value, Compare >& AVLTree< Key, Value, Compare >::operator=(const AVLTree& other)
  {
    if (this != &other) {
      (*this) = AVLTree< Key, Value, Compare >(other);
    }
    return *this;
  }

  template< typename Key, typename Value, typename Compare >
  AVLTree< Key, Value, Compare >& AVLTree< Key, Value, Compare >::operator=(AVLTree&& other) noexcept
  {
    if (this != &other) {
      std::swap(root_, other.root_);
      std::swap(size_, other.size_);
    }
    return *this;
  }

  template< typename Key, typename Value, typename Compare >
  typename AVLTree< Key, Value, Compare >::iterator AVLTree< Key, Value, Compare >::begin()
  {
    return iterator(root_);
  }

  template< typename Key, typename Value, typename Compare >
  typename AVLTree< Key, Value, Compare >::iterator AVLTree< Key, Value, Compare >::end()
  {
    return iterator(nullptr);
  }

  template< typename Key, typename Value, typename Compare >
  typename AVLTree< Key, Value, Compare >::const_iterator AVLTree< Key, Value, Compare >::begin() const noexcept
  {
    return const_iterator(root_);
  }

  template< typename Key, typename Value, typename Compare >
  typename AVLTree< Key, Value, Compare >::const_iterator AVLTree< Key, Value, Compare >::end() const noexcept
  {
    return const_iterator(nullptr);
  }

  template< typename Key, typename Value, typename Compare >
  typename AVLTree< Key, Value, Compare >::const_iterator
  AVLTree< Key, Value, Compare >::cbegin(TraversalStrategy strategy) const noexcept
  {
    return const_iterator(root_, strategy);
  }

  template< typename Key, typename Value, typename Compare >
  typename AVLTree< Key, Value, Compare >::const_iterator AVLTree< Key, Value, Compare >::cend() const noexcept
  {
    return const_iterator(nullptr);
  }

  template< typename Key, typename Value, typename Compare >
  const Value& AVLTree< Key, Value, Compare >::get(const Key& key)
  {
    for (const_iterator it = cbegin(); it != cend(); ++it) {
      if (it->first == key) {
        return it->second;
      }
    }
    throw std::logic_error("AVLTree get Error: cannot get value");
  }

  template< typename Key, typename Value, typename Compare >
  void AVLTree< Key, Value, Compare >::push(const Key& key, const Value& value)
  {
    root_ = insert(root_, key, value);
    ++size_;
  }

  template< typename Key, typename Value, typename Compare >
  void AVLTree< Key, Value, Compare >::merge(const AVLTree& other)
  {
    for (const_iterator it = other.cbegin(); it != other.cend(); ++it) {
      if (contains(it->first)) {
        continue;
      }
      push(it->first, it->second);
    }
  }

  template< typename Key, typename Value, typename Compare >
  void AVLTree< Key, Value, Compare >::remove(const Key& key, const Value& value)
  {
    root_ = remove(root_, key);
    --size_;
  }

  template< typename Key, typename Value, typename Compare >
  void AVLTree< Key, Value, Compare >::print() const noexcept
  {
    for (const_iterator it = cbegin(); it != cend(); ++it) {
      std::cout << " " << it->first << " " << it->second;
    }
    std::cout << '\n';
  }

  template< typename Key, typename Value, typename Compare >
  bool AVLTree< Key, Value, Compare >::contains(const Key& key) const noexcept
  {
    return find(key) != nullptr;
  }

  template< typename Key, typename Value, typename Compare >
  bool AVLTree< Key, Value, Compare >::isEmpty() const noexcept
  {
    return root_ == nullptr;
  }

  template< typename Key, typename Value, typename Compare >
  void AVLTree< Key, Value, Compare >::clear()
  {
    clear(root_);
  }

  template< typename Key, typename Value, typename Compare >
  int AVLTree< Key, Value, Compare >::getBalance(AVLTree::Node *node)
  {
    return getHeight(node->right_) - getHeight(node->left_);
  }

  template< typename Key, typename Value, typename Compare >
  int AVLTree< Key, Value, Compare >::getHeight(AVLTree::Node *node)
  {
    return (node == nullptr ? 0 : node->height_);
  }

  template< typename Key, typename Value, typename Compare >
  typename AVLTree< Key, Value, Compare >::Node *AVLTree< Key, Value, Compare >::remove(Node *node, const Key& key)
  {
    if (!node) {
      return nullptr;
    }
    if (comp_(key, node->key_)) {
      node->left_ = remove(node->left_, key);
    } else if (comp_(node->key_, key)) {
      node->right_ = remove(node->right_, key);
    } else {
      Node *left_ = node->left_;
      Node *right_ = node->right_;
      delete node;
      if (right_ == nullptr) {
        return left_;
      }
      Node *min = findMin(right_);
      min->right_ = removeMin(right_);
      min->left_ = left_;
      return balance(min);
    }
    return balance(node);
  }

  template< typename Key, typename Value, typename Compare >
  typename AVLTree< Key, Value, Compare >::Node *AVLTree< Key, Value, Compare >::find(const Key& key) const noexcept
  {
    Node *node = root_;
    while (node && node->value_.first != key) {
      node = (key > node->value_.first) ? node->right_ : node->left_;
    }
    return node;
  }

  template< typename Key, typename Value, typename Compare >
  void AVLTree< Key, Value, Compare >::fixHeight(AVLTree< Key, Value, Compare >::Node *node)
  {
    std::size_t left_height = getHeight(node->left_);
    std::size_t right_height = getHeight(node->right_);
    node->height_ = (left_height > right_height ? left_height : right_height) + 1;
  }

  template< typename Key, typename Value, typename Compare >
  typename AVLTree< Key, Value, Compare >::Node *AVLTree< Key, Value, Compare >::balance(Node *node)
  {
    fixHeight(node);
    if (getBalance(node) == 2) {
      if (getBalance(node->right_) < 0) {
        return doubleLeftRotate(node);
      }
      return rotateLeft(node);
    }
    if (getBalance(node) == -2) {
      if (getBalance(node->left_) > 0) {
        return doubleRightRotate(node);
      }
      return rotateRight(node);
    }
    return node;
  }

  template< typename Key, typename Value, typename Compare >
  typename AVLTree< Key, Value, Compare >::Node *AVLTree< Key, Value, Compare >::findMin(AVLTree::Node *node)
  {
    return (node->left_) ? findMin(node->left_) : node;
  }

  template< typename Key, typename Value, typename Compare >
  typename AVLTree< Key, Value, Compare >::Node *AVLTree< Key, Value, Compare >::removeMin(Node *node)
  {
    if (node->left_ == 0) {
      return node->right_;
    }
    node->left_ = removeMin(node->left_);
    return balance(node);
  }

  template< typename Key, typename Value, typename Compare >
  typename AVLTree< Key, Value, Compare >::Node *AVLTree< Key, Value, Compare >::doubleRightRotate(AVLTree::Node *node)
  {
    node->left_ = rotateLeft(node->left_);
    return rotateRight(node);
  }

  template< typename Key, typename Value, typename Compare >
  typename AVLTree< Key, Value, Compare >::Node *AVLTree< Key, Value, Compare >::doubleLeftRotate(AVLTree::Node *node)
  {
    node->right_ = rotateRight(node->right_);
    return rotateLeft(node);
  }

  template< typename Key, typename Value, typename Compare >
  typename AVLTree< Key, Value, Compare >::Node *AVLTree< Key, Value, Compare >::rotateLeft(AVLTree::Node *node)
  {
    Node *newNode = node->right_;
    node->right_ = newNode->left_;
    newNode->left_ = node;
    fixHeight(node);
    fixHeight(newNode);
    return newNode;
  }

  template< typename Key, typename Value, typename Compare >
  typename AVLTree< Key, Value, Compare >::Node *AVLTree< Key, Value, Compare >::rotateRight(Node *node)
  {
    Node *newNode = node->left_;
    node->left_ = node->right_;
    newNode->right_ = node;
    fixHeight(node);
    fixHeight(newNode);
    return newNode;
  }

  template< typename Key, typename Value, typename Compare >
  typename AVLTree< Key, Value, Compare >::Node *AVLTree< Key, Value, Compare >::insert(Node *node, const Key& key, const Value& value)
  {
    if (!node) {
      return new Node(key, value);
    }
    if (comp_(key, node->value_.first)) {
      node->left_ = insert(node->left_, key, value);
    } else if (comp_(node->value_.first, key)) {
      node->right_ = insert(node->right_, key, value);
    } else {
      node->value_.second = value;
      return node;
    }
    return balance(node);
  }

  template< typename Key, typename Value, typename Compare >
  void AVLTree< Key, Value, Compare >::clear(Node *node)
  {
    if (node == nullptr) {
      return;
    }
    clear(node->left_);
    clear(node->right_);
    delete node;
  }

  template< typename Key, typename Value, typename Compare >
  NodeFunctor AVLTree< Key, Value, Compare >::traverse(NodeFunctor f, TraversalStrategy strategy) const
  {
    for (const_iterator it = cbegin(strategy); it != cend(); ++it) {
      f(*it);
    }
    return f;
  }
}
#endif
