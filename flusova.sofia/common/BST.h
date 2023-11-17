#ifndef BST_H
#define BST_H

#include <initializer_list>
#include <cstddef>
#include <limits>
#include <utility>
#include <queue>
#include "Stack.h"

namespace flusova {
  template< typename Key, typename Value, typename Comparator = std::less< Key > >
  class BinarySearchTree {
  public:

    class ConstIterator;
    class Iterator;

    BinarySearchTree() : root_(nullptr) {}
    BinarySearchTree(const BinarySearchTree< Key, Value, Comparator >& src);
    BinarySearchTree(BinarySearchTree< Key, Value, Comparator >&& src) noexcept: root_(src.root_) {}
    BinarySearchTree(std::initializer_list< std::pair< const Key, Value > > pairs);
    ~BinarySearchTree() { clearTree(root_); }
    BinarySearchTree& operator=(const BinarySearchTree< Key, Value, Comparator >& src);
    BinarySearchTree& operator=(BinarySearchTree< Key, Value, Comparator >&& src) noexcept;
    Value& operator[](const Key& key);
    void insert(const Key& key, const Value& value);
    void insert(std::initializer_list< std::pair< Key, Value > > list);
    void merge(const BinarySearchTree< Key, Value, Comparator >& source);
    void erase(const Key& key);
    template< typename ... Args >
    std::pair< Iterator, bool > emplace(Args&& ... args);
    template< typename Ty >
    std::pair< Iterator, bool > insert(Ty&& value);
    template< typename IT >
    void insert(IT begin, IT end);
    bool isEmpty() const noexcept;
    Iterator insert(Iterator pos, const std::pair< Key, Value >& val);
    Iterator erase(Iterator pos);
    Iterator erase(Iterator begin, Iterator end);
    Iterator erase(ConstIterator begin, ConstIterator end);
    Iterator lower_bound(const Key& key);
    Iterator upper_bound(const Key& key);
    Iterator begin();
    Iterator end();
    Iterator find(const Key& key);
    ConstIterator cbegin() const;
    ConstIterator cend() const;
    ConstIterator find(const Key& key) const;

    template< typename F >
    F traverse_lnr(F f) const;
    template< typename F >
    F traverse_rnl(F f) const;
    template< typename F >
    F traverse_breadth(F f) const;

  private:

    struct Node {
      explicit Node(const Key& key, const Value& value, Node* left = nullptr, Node* right = nullptr) :
              data_(std::pair< Key, Value >(key, value)),
              height_(1),
              left_(left),
              right_(right)
      {}

      std::pair< Key, Value > data_;
      int height_;
      Node* left_;
      Node* right_;
    };

    int getHeight(Node* node) const;
    int getDifference(Node* node);
    void setHeight(Node* node);
    Node* leftSmallRotation(Node* node);
    Node* rightSmallRotation(Node* node);
    Node* balanceTree(Node* root);
    Node* insert(const Key& key, const Value& value, Node* root);
    Node* removeMin(Node* node);
    Node* findMin(Node* node);
    Node* remove(Node* node, const Key& key);
    void clearTree(Node* node);
    void swap(BinarySearchTree< Key, Value, Comparator >& src) noexcept;

    Node* root_;

  };

  template< typename Key, typename Value, typename Comparator >
  flusova::BinarySearchTree< Key, Value, Comparator > complement(BinarySearchTree< Key, Value, Comparator > dictL,
                                                                 BinarySearchTree< Key, Value, Comparator > dictR);
  template< typename Key, typename Value, typename Comparator >
  flusova::BinarySearchTree< Key, Value, Comparator > intersect(BinarySearchTree< Key, Value, Comparator > dictL,
                                                                BinarySearchTree< Key, Value, Comparator > dictR);
  template< typename Key, typename Value, typename Comparator >
  flusova::BinarySearchTree< Key, Value, Comparator > unite(BinarySearchTree< Key, Value, Comparator > dictL,
                                                            BinarySearchTree< Key, Value, Comparator > dictR);
}

template< typename Key, typename Value, typename Comparator >
flusova::BinarySearchTree< Key, Value, Comparator >::BinarySearchTree(
        const BinarySearchTree< Key, Value, Comparator >& src) :
        BinarySearchTree() {
  ConstIterator begin = src.cbegin();
  ConstIterator end = src.cend();
  for (; begin != end; begin++) {
    insert((*begin).first, (*begin).second);
  }
}

template< typename Key, typename Value, typename Comparator >
flusova::BinarySearchTree< Key, Value, Comparator >::BinarySearchTree(
        std::initializer_list< std::pair< const Key, Value > > pairs) :
        BinarySearchTree() {
  for (const auto& pair: pairs) {
    insert(pair.first, pair.second);
  }
}

template< typename Key, typename Value, typename Comparator >
flusova::BinarySearchTree< Key, Value, Comparator >&
flusova::BinarySearchTree< Key, Value, Comparator >::operator=(const BinarySearchTree< Key, Value, Comparator >& src) {
  if (this != &src) {
    BinarySearchTree< Key, Value, Comparator > temp(src);
    swap(temp);
  }
  return *this;
}

template< typename Key, typename Value, typename Comparator >
flusova::BinarySearchTree< Key, Value, Comparator >&
flusova::BinarySearchTree< Key, Value, Comparator >::operator=(
        BinarySearchTree< Key, Value, Comparator >&& src) noexcept {
  if (this != &src) {
    swap(src);
  }
  return *this;
}

template< typename Key, typename Value, typename Comparator >
Value& flusova::BinarySearchTree< Key, Value, Comparator >::operator[](const Key& key) {
  std::pair< Key, Value > pair;
  pair.first = key;
  Iterator it = find(key);
  if (it == end()) {
    insert(pair.first, pair.second);
    it = find(key);
  }
  return (*it).second;
}

template< typename Key, typename Value, typename Comparator >
void flusova::BinarySearchTree< Key, Value, Comparator >::insert(const Key& key, const Value& value) {
  root_ = insert(key, value, root_);
}

template< typename Key, typename Value, typename Comparator >
void flusova::BinarySearchTree< Key, Value, Comparator >::insert(std::initializer_list< std::pair< Key, Value > > list) {
  for (const auto& val: list) {
    insert(val.first, val.second);
  }
}

template< typename Key, typename Value, typename Comparator >
void flusova::BinarySearchTree< Key, Value, Comparator >::merge(
        const flusova::BinarySearchTree< Key, Value, Comparator >& src) {
  ConstIterator begin = src.cbegin();
  ConstIterator end = src.cend();
  for (; begin != end; begin++) {
    insert((*begin).first, (*begin).second);
  }
}

template< typename Key, typename Value, typename Comparator >
void flusova::BinarySearchTree< Key, Value, Comparator >::erase(const Key& key) {
  root_ = remove(root_, key);
}

template< typename Key, typename Value, typename Comparator >
template< typename ... Args >
std::pair< typename flusova::BinarySearchTree< Key, Value, Comparator >::Iterator, bool >
flusova::BinarySearchTree< Key, Value, Comparator >::emplace(Args&& ... args) {
  std::pair< Key, Value > val(std::forward< Args >(args)...);
  Iterator it = lower_bound(val.first);
  std::pair< Iterator, bool > pair(it, false);
  Comparator c;
  if ((c(val.first, (*it).first)) || (it == end())) {
    it = insert((*it).first, val.second);
    pair.second = true;
    return pair;
  }
  return pair;
}

template< typename Key, typename Value, typename Comparator >
template< typename Ty >
std::pair< typename flusova::BinarySearchTree< Key, Value, Comparator >::Iterator, bool >
flusova::BinarySearchTree< Key, Value, Comparator >::insert(Ty&& val) {
  return emplace(std::forward< Ty >(val));
}

template< typename Key, typename Value, typename Comparator >
template< typename IT >
void flusova::BinarySearchTree< Key, Value, Comparator >::insert(IT begin, IT end) {
  IT it = begin;
  while (begin != end) {
    insert(*it);
    it++;
  }
}

template< typename Key, typename Value, typename Comparator >
bool flusova::BinarySearchTree< Key, Value, Comparator >::isEmpty() const noexcept {
  return (root_ == nullptr);
}

template< typename Key, typename Value, typename Comparator >
typename flusova::BinarySearchTree< Key, Value, Comparator >::Iterator
flusova::BinarySearchTree< Key, Value, Comparator >::insert(BinarySearchTree::Iterator pos,
                                                            const std::pair< Key, Value >& val) {
  insert(val.first, val.second);
  Iterator it = this->begin();
  Iterator end = this->end();
  for (; it != end; it++) {
    if ((*it).first == val.first) {
      return it;
    }
  }
  return it;
}

template< typename Key, typename Value, typename Comparator >
typename flusova::BinarySearchTree< Key, Value, Comparator >::Iterator
flusova::BinarySearchTree< Key, Value, Comparator >::erase(BinarySearchTree::Iterator pos) {
  if (pos == end()) {
    return end();
  }
  Iterator it = pos;
  it++;
  erase((*pos).first);
  return it;
}

template< typename Key, typename Value, typename Comparator >
typename flusova::BinarySearchTree< Key, Value, Comparator >::Iterator
flusova::BinarySearchTree< Key, Value, Comparator >::erase(BinarySearchTree::Iterator begin,
                                                           BinarySearchTree::Iterator end) {
  Iterator it = begin;
  while (it != end) {
    Iterator curr = it;
    it++;
    erase(curr);
  }
  return it;
}

template< typename Key, typename Value, typename Comparator >
typename flusova::BinarySearchTree< Key, Value, Comparator >::Iterator
flusova::BinarySearchTree< Key, Value, Comparator >::erase(BinarySearchTree::ConstIterator begin,
                                                           BinarySearchTree::ConstIterator end) {
  ConstIterator it = begin;
  while (it != end) {
    ConstIterator curr = it;
    it++;
    erase(curr);
  }
  return it;
}

template< typename Key, typename Value, typename Comparator >
typename flusova::BinarySearchTree< Key, Value, Comparator >::Iterator
flusova::BinarySearchTree< Key, Value, Comparator >::lower_bound(const Key& key) {
  Iterator it = this->begin();
  Iterator end = this->end();
  Comparator c;
  while (it != end) {
    if (!c(key, (*it).first)) {
      return it;
    }
    it++;
  }
  return it;
}

template< typename Key, typename Value, typename Comparator >
typename flusova::BinarySearchTree< Key, Value, Comparator >::Iterator
flusova::BinarySearchTree< Key, Value, Comparator >::upper_bound(const Key& key) {
  Iterator it = lower_bound(key);
  if (key == (*it).first) {
    it++;
  }
  return it;
}

template< typename Key, typename Value, typename Comparator >
typename flusova::BinarySearchTree< Key, Value, Comparator >::Iterator
flusova::BinarySearchTree< Key, Value, Comparator >::begin() {
  return Iterator(root_);
}

template< typename Key, typename Value, typename Comparator >
typename flusova::BinarySearchTree< Key, Value, Comparator >::Iterator
flusova::BinarySearchTree< Key, Value, Comparator >::end() {
  return Iterator(nullptr);
}

template< typename Key, typename Value, typename Comparator >
typename flusova::BinarySearchTree< Key, Value, Comparator >::Iterator
flusova::BinarySearchTree< Key, Value, Comparator >::find(const Key& key) {
  Iterator it = this->begin();
  Iterator end = this->end();
  for (; it != end; it++) {
    if ((*it).first == key) {
      return it;
    }
  }
  return end;
}

template< typename Key, typename Value, typename Comparator >
typename flusova::BinarySearchTree< Key, Value, Comparator >::ConstIterator
flusova::BinarySearchTree< Key, Value, Comparator >::cbegin() const {
  return ConstIterator(root_);
}

template< typename Key, typename Value, typename Comparator >
typename flusova::BinarySearchTree< Key, Value, Comparator >::ConstIterator
flusova::BinarySearchTree< Key, Value, Comparator >::cend() const {
  return ConstIterator(nullptr);
}

template< typename Key, typename Value, typename Comparator >
typename flusova::BinarySearchTree< Key, Value, Comparator >::ConstIterator
flusova::BinarySearchTree< Key, Value, Comparator >::find(const Key& key) const {
  ConstIterator it = this->cbegin();
  ConstIterator end = this->cend();
  for (; it != end; it++) {
    if ((*it).first == key) {
      return it;
    }
  }
  return end;
}

template< typename Key, typename Value, typename Comparator >
template< typename F >
F flusova::BinarySearchTree< Key, Value, Comparator >::traverse_lnr(F f) const {
    Stack< Node* > stack;
    Node* current = root_;
    while (current || !stack.isEmpty()) {
        while (current) {
            stack.pushTop(current);
            current = current->left_;
        }
        if (!stack.isEmpty()) {
            current = stack.getHead();
            stack.popTop();
            f(current->data_.first, current->data_.second);
            current = current->right_;
        }
    }
    return f;
}

template< typename Key, typename Value, typename Comparator >
template< typename F >
F flusova::BinarySearchTree< Key, Value, Comparator >::traverse_rnl(F f) const {
    Stack< Node* > stack;
    Node* current = root_;
    while (current || !stack.isEmpty()) {
        while (current) {
            stack.pushTop(current);
            current = current->right_;
        }
        if (!stack.isEmpty()) {
            current = stack.getHead();
            stack.popTop();
            f(current->data_.first, current->data_.second);
            current = current->left_;
        }
    }

    return f;
}

template< typename Key, typename Value, typename Comparator >
template< typename F >
F flusova::BinarySearchTree< Key, Value, Comparator >::traverse_breadth(F f) const {
    if (!root_) {
        return f;
    }
    std::queue< Node* > queue;
    queue.push(root_);
    while (!queue.empty()) {
        Node* current = queue.front();
        queue.pop();
        f(current->data_.first, current->data_.second);
        if (current->left_) {
            queue.push(current->left_);
        }
        if (current->right_) {
            queue.push(current->right_);
        }
    }
    return f;
}

template< typename Key, typename Value, typename Comparator >
int flusova::BinarySearchTree< Key, Value, Comparator >::getHeight(
        flusova::BinarySearchTree< Key, Value, Comparator >::Node* node) const {
  if (node != nullptr) {
    return node->height_;
  }
  return 0;
}

template< typename Key, typename Value, typename Comparator >
int flusova::BinarySearchTree< Key, Value, Comparator >::getDifference(
        flusova::BinarySearchTree< Key, Value, Comparator >::Node* node) {
  return getHeight(node->right_) - getHeight(node->left_);
}

template< typename Key, typename Value, typename Comparator >
void flusova::BinarySearchTree< Key, Value, Comparator >::setHeight(
        flusova::BinarySearchTree< Key, Value, Comparator >::Node* node) {
  int leftHeight = getHeight(node->left_);
  int rightHeight = getHeight(node->right_);
  node->height_ = (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
}

template< typename Key, typename Value, typename Comparator >
typename flusova::BinarySearchTree< Key, Value, Comparator >::Node*
flusova::BinarySearchTree< Key, Value, Comparator >::leftSmallRotation(
        flusova::BinarySearchTree< Key, Value, Comparator >::Node* node) {
  Node* root = node->right_;
  node->right_ = root->left_;
  root->left_ = node;
  setHeight(node);
  setHeight(root);
  return root;
}

template< typename Key, typename Value, typename Comparator >
typename flusova::BinarySearchTree< Key, Value, Comparator >::Node*
flusova::BinarySearchTree< Key, Value, Comparator >::rightSmallRotation(
        flusova::BinarySearchTree< Key, Value, Comparator >::Node* node) {
  Node* root = node->left_;
  node->left_ = root->right_;
  root->right_ = node;
  setHeight(node);
  setHeight(root);
  return root;
}

template< typename Key, typename Value, typename Comparator >
typename flusova::BinarySearchTree< Key, Value, Comparator >::Node*
flusova::BinarySearchTree< Key, Value, Comparator >::balanceTree(
        flusova::BinarySearchTree< Key, Value, Comparator >::Node* root) {
  setHeight(root);
  if (getDifference(root) == 2) {
    if (getDifference(root->right_) < 0) {
      root->right_ = rightSmallRotation(root->right_);
    }
    return leftSmallRotation(root);
  }
  if (getDifference(root) == -2) {
    if (getDifference(root->left_) > 0) {
      root->left_ = leftSmallRotation(root->left_);
    }
    return rightSmallRotation(root);
  }
  return root;
}

template< typename Key, typename Value, typename Comparator >
typename flusova::BinarySearchTree< Key, Value, Comparator >::Node*
flusova::BinarySearchTree< Key, Value, Comparator >::removeMin(Node* node) {
  if (!node->left_) {
    return node->right_;
  }
  node->left_ = removeMin(node->left_);
  return balanceTree(node);
}

template< typename Key, typename Value, typename Comparator >
typename flusova::BinarySearchTree< Key, Value, Comparator >::Node*
flusova::BinarySearchTree< Key, Value, Comparator >::findMin(Node* node) {
  if (node->left_) {
    return findMin(node->left_);
  } else {
    return node;
  }
}

template< typename Key, typename Value, typename Comparator >
typename flusova::BinarySearchTree< Key, Value, Comparator >::Node*
flusova::BinarySearchTree< Key, Value, Comparator >::remove(
        flusova::BinarySearchTree< Key, Value, Comparator >::Node* node, const Key& key) {
  Comparator c;
  if (node == nullptr) {
    return node;
  }
  if (c(key, node->data_.first)) {
    node->left_ = remove(node->left_, key);
  } else if (c(node->data_.first, key)) {
    node->right_ = remove(node->right_, key);
  } else {
    Node* left = node->left_;
    Node* right = node->right_;
    delete node;
    if (right == nullptr) {
      return left;
    }
    Node* temp = findMin(right);
    temp->right_ = removeMin(right);
    temp->left_ = left;
    return balanceTree(temp);
  }
  return balanceTree(node);
}

template< typename Key, typename Value, typename Comparator >
typename flusova::BinarySearchTree< Key, Value, Comparator >::Node*
flusova::BinarySearchTree< Key, Value, Comparator >::insert(const Key& key, const Value& value,
                                                            flusova::BinarySearchTree< Key, Value, Comparator >::Node* root) {
  if (!root) {
    return new Node(key, value);
  }
  Comparator c;
  if (c(key, root->data_.first)) {
    root->left_ = insert(key, value, root->left_);
  } else if (c(root->data_.first, key)) {
    root->right_ = insert(key, value, root->right_);
  } else {
    root->data_.second = value;
    return root;
  }
  return balanceTree(root);
}

template< typename Key, typename Value, typename Comparator >
void flusova::BinarySearchTree< Key, Value, Comparator >::clearTree(
        flusova::BinarySearchTree< Key, Value, Comparator >::Node* node) {
  if (node) {
    clearTree(node->left_);
    clearTree(node->right_);
    delete node;
  }
}

template< typename Key, typename Value, typename Comparator >
void flusova::BinarySearchTree< Key, Value, Comparator >::swap(
        BinarySearchTree< Key, Value, Comparator >& src) noexcept {
  std::swap(root_, src.root_);
}

template< typename Key, typename Value, typename Comparator >
flusova::BinarySearchTree< Key, Value, Comparator >
flusova::complement(flusova::BinarySearchTree< Key, Value, Comparator > dictL,
                    flusova::BinarySearchTree< Key, Value, Comparator > dictR) {
  flusova::BinarySearchTree< Key, Value, Comparator > complemented;
  for (typename flusova::BinarySearchTree< Key, Value, Comparator >::Iterator it = dictL.begin();
       it != dictL.end(); ++it) {
    if (dictR.find((*it).first) == dictR.end()) {
      complemented[(*it).first] = (*it).second;
    }
  }
  return complemented;
}

template< typename Key, typename Value, typename Comparator >
flusova::BinarySearchTree< Key, Value, Comparator >
flusova::intersect(flusova::BinarySearchTree< Key, Value, Comparator > dictL,
                   flusova::BinarySearchTree< Key, Value, Comparator > dictR) {
  flusova::BinarySearchTree< Key, Value, Comparator > intersected;
  for (typename flusova::BinarySearchTree< Key, Value, Comparator >::Iterator it = dictL.begin();
       it != dictL.end(); ++it) {
    if (dictR.find((*it).first) != dictR.end()) {
      intersected[(*it).first] = (*it).second;
    }
  }
  return intersected;
}

template< typename Key, typename Value, typename Comparator >
flusova::BinarySearchTree< Key, Value, Comparator >
flusova::unite(flusova::BinarySearchTree< Key, Value, Comparator > dictL,
               flusova::BinarySearchTree< Key, Value, Comparator > dictR) {
  flusova::BinarySearchTree< Key, Value, Comparator > united = dictR;
  united.merge(dictL);
  return united;
}

#endif
