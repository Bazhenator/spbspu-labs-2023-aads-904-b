#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include <iterator>
#include <functional>
#include <stdexcept>
#include <initializer_list>

namespace nemchinov
{
  template< typename Key, typename Value, typename Compare = std::less< Key > >
  class BinarySearchTree
  {
    struct TreeNode;
    using avlTree = BinarySearchTree< Key, Value, Compare >;

  public:
    BinarySearchTree();
    BinarySearchTree(const std::initializer_list< std::pair< Key, Value > > &);

    ~BinarySearchTree();

    BinarySearchTree(const BinarySearchTree &);
    BinarySearchTree &operator=(const BinarySearchTree &);

    BinarySearchTree(BinarySearchTree &&) noexcept;
    BinarySearchTree &operator=(BinarySearchTree &&) noexcept;

    template< typename ValueIt >
    class Iterator;

    using iterator = Iterator< Value >;
    using const_iterator = Iterator< const Value >;

    bool isEmpty() const noexcept;
    std::size_t size() const noexcept;
    void insert(const Key &, const Value &);
    void erase(const Key &);
    void erase(const Key &, const Key &);
    Value &operator[](const Key &);
    bool contains(const Key &);
    iterator erase(iterator &);
    iterator erase(iterator &, iterator &);
    void delIf(bool (*checkFoo)(Key, avlTree &), avlTree &other);

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
    iterator lowerBound(const Key &);
    iterator upperBound(const Key &);
    void clean();

    template< typename F >
    F traverse_lnr(F f) const;
    template< typename F >
    F traverse_rnl(F f) const;
    template< typename F >
    F traverse_breadth(F f) const;

  private:
    unsigned char getHeight(TreeNode *);
    void setHeight(TreeNode *);
    char getBalanceFactor(TreeNode *);
    TreeNode *rotateRight(TreeNode *);
    TreeNode *rotateLeft(TreeNode *);
    TreeNode *balance(TreeNode *);
    TreeNode *insert(TreeNode *, TreeNode *, const std::pair< Key, Value > &, const Compare &);
    const Key &getKey(TreeNode *);
    Value &getValue(TreeNode *);
    TreeNode *pop(TreeNode *, const Key &);
    TreeNode *getMin(TreeNode *);
    TreeNode *extractMin(TreeNode *);

    struct BaseTreeNode
    {
      explicit BaseTreeNode(TreeNode *parent) :
              left_(nullptr), right_(nullptr), parent_(parent)
      {}

      TreeNode *left_;
      TreeNode *right_;
      TreeNode *parent_;
    };

    struct TreeNode : BaseTreeNode
    {
      explicit TreeNode(const std::pair< Key, Value > &data, TreeNode *parent) :
              BaseTreeNode(parent), data_(data), height_(1)
      {}

      std::pair< Key, Value > data_;
      unsigned char height_;
    };

    BaseTreeNode *fake_;
    std::size_t size_;
  };

  template< typename Key, typename Value, typename Compare >
  BinarySearchTree< Key, Value, Compare >::BinarySearchTree() :
          fake_(new BaseTreeNode(nullptr)), size_(0)
  {}

  template< typename Key, typename Value, typename Compare >
  BinarySearchTree< Key, Value, Compare >::BinarySearchTree(const std::initializer_list< std::pair< Key, Value>> &data)
          : BinarySearchTree()
  {
    for (const std::pair< Key, Value > &pair : data)
    {
      this->insert(pair.first, pair.second);
    }
  }

  template< typename Key, typename Value, typename Compare >
  BinarySearchTree< Key, Value, Compare >::~BinarySearchTree()
  {
    while (fake_->parent_)
    {
      erase(getKey(fake_->parent_));
    }
    delete fake_;
    fake_ = nullptr;
  }

  template< typename Key, typename Value, typename Compare >
  BinarySearchTree< Key, Value, Compare >::BinarySearchTree(const BinarySearchTree &other) : BinarySearchTree()
  {
    for (const_iterator it = other.begin(); it != other.end(); ++it)
    {
      this->insert(it.key(), it.value());
    }
  }

  template< typename Key, typename Value, typename Compare >
  BinarySearchTree< Key, Value, Compare > &
  BinarySearchTree< Key, Value, Compare >::operator=(const BinarySearchTree &other)
  {
    if (this != &other)
    {
      (*this) = BinarySearchTree< Key, Value, Compare >(other);
    }
    return *this;
  }

  template< typename Key, typename Value, typename Compare >
  BinarySearchTree< Key, Value, Compare >::BinarySearchTree(BinarySearchTree &&other) noexcept
  {
    size_ = other.size_;
    fake_ = other.fake_;
    other.fake_ = nullptr;
  }

  template< typename Key, typename Value, typename Compare >
  BinarySearchTree< Key, Value, Compare > &
  BinarySearchTree< Key, Value, Compare >::operator=(BinarySearchTree &&other) noexcept
  {
    size_ = other.size_;
    fake_ = other.fake_;
    other.fake_ = nullptr;
    return *this;
  }

  template< typename Key, typename Value, typename Compare >
  bool BinarySearchTree< Key, Value, Compare >::isEmpty() const noexcept
  {
    return size_ == 0;
  }

  template< typename Key, typename Value, typename Compare >
  std::size_t BinarySearchTree< Key, Value, Compare >::size() const noexcept
  {
    return size_;
  }

  template< typename Key, typename Value, typename Compare >
  void BinarySearchTree< Key, Value, Compare >::insert(const Key &key, const Value &value)
  {
    if (contains(key)) return;
    fake_->parent_ = insert(nullptr, fake_->parent_, std::make_pair(key, value), Compare());
    ++size_;

    if (getHeight(fake_->parent_) == 1)
    {
      fake_->left_ = fake_->parent_, fake_->right_ = fake_->parent_;
    }
    if (getKey(fake_->left_) > key)
    {
      fake_->left_ = fake_->left_->left_;
      return;
    }
    if (getKey(fake_->right_) < key)
    {
      fake_->right_ = fake_->right_->right_;
      return;
    }
  }

  template< typename Key, typename Value, typename Compare >
  void BinarySearchTree< Key, Value, Compare >::erase(const Key &key)
  {
    if (!contains(key)) return;
    if (key == getKey(fake_->left_))
    {
      iterator newLeft = iterator(fake_->left_, fake_);
      ++newLeft;
      fake_->left_ = newLeft.itNode_;
      if (!newLeft.itNode_) fake_->right_ = nullptr;
    } else if (key == getKey(fake_->right_))
    {
      iterator newRight = iterator(fake_->right_, fake_);
      --newRight;
      fake_->right_ = newRight.itNode_;
    }
    fake_->parent_ = pop(fake_->parent_, key);
    --size_;
  }

  template< typename Key, typename Value, typename Compare >
  void BinarySearchTree< Key, Value, Compare >::erase(const Key &firstKey, const Key &secondKey)
  {
    Compare cmp{};
    iterator fIt = lowerBound(firstKey);
    if (fIt.key() != firstKey) throw std::logic_error("Erase error : incorrect begin key");
    iterator sIt = lowerBound(secondKey);
    if (fIt.key() != firstKey) throw std::logic_error("Erase error : incorrect end key");
    if (cmp(secondKey, firstKey)) return;
    erase(fIt, sIt);
  }

  template< typename Key, typename Value, typename Compare >
  Value &BinarySearchTree< Key, Value, Compare >::operator[](const Key &key)
  {
    iterator it = lowerBound(key);
    if (it != end() && it.key() == key) return *it;
    insert(key, Value{});
    return *lowerBound(key);
  }

  template< typename Key, typename Value, typename Compare >
  bool BinarySearchTree< Key, Value, Compare >::contains(const Key &key)
  {
    iterator it = lowerBound(key);
    if (it.itNode_) return it.key() == key;
    return false;
  }

  template< typename Key, typename Value, typename Compare >
  typename BinarySearchTree< Key, Value, Compare >::iterator
  BinarySearchTree< Key, Value, Compare >::erase(BinarySearchTree::iterator &it)
  {
    const Key key = it.key();
    iterator nextIt = ++it;
    erase(key);
    return nextIt;
  }

  template< typename Key, typename Value, typename Compare >
  typename BinarySearchTree< Key, Value, Compare >::iterator
  BinarySearchTree< Key, Value, Compare >::erase(BinarySearchTree::iterator &begin_, BinarySearchTree::iterator &end_)
  {
    if (begin_ == end() || end_ == end() || begin_.key() > end_.key())
    {
      throw std::logic_error("Erase error : incorrect range");
    }
    while (begin_ != end_)
    {
      begin_ = erase(begin_);
    }
    return erase(begin_);
  }

  template< typename Key, typename Value, typename Compare >
  void BinarySearchTree< Key, Value, Compare >::delIf(bool (*checkFoo)(Key, avlTree &), avlTree &other)
  {
    for (iterator it = begin(); it != end();)
    {
      if (checkFoo(it.key(), other))
      {
        it = erase(it);
      } else ++it;
    }
  }

  template< typename Key, typename Value, typename Compare >
  typename BinarySearchTree< Key, Value, Compare >::iterator BinarySearchTree< Key, Value, Compare >::begin()
  {
    return iterator(fake_->left_, fake_);
  }

  template< typename Key, typename Value, typename Compare >
  typename BinarySearchTree< Key, Value, Compare >::iterator BinarySearchTree< Key, Value, Compare >::end()
  {
    return iterator(nullptr, fake_);
  }

  template< typename Key, typename Value, typename Compare >
  typename BinarySearchTree< Key, Value, Compare >::const_iterator
  BinarySearchTree< Key, Value, Compare >::begin() const
  {
    return const_iterator(fake_->left_, fake_);
  }

  template< typename Key, typename Value, typename Compare >
  typename BinarySearchTree< Key, Value, Compare >::const_iterator BinarySearchTree< Key, Value, Compare >::end() const
  {
    return const_iterator(nullptr, fake_);
  }

  template< typename Key, typename Value, typename Compare >
  typename BinarySearchTree< Key, Value, Compare >::iterator
  BinarySearchTree< Key, Value, Compare >::lowerBound(const Key &key)
  {
    Compare cmp{};
    TreeNode *root = fake_->parent_;
    if (!root) return end();
    while (true)
    {
      if (key == getKey(root)) return iterator(root, fake_);
      while (cmp(key, getKey(root)))
      {
        if (!root->left_) return iterator(root, fake_);
        root = root->left_;
      }
      while (cmp(getKey(root), key))
      {
        if (!root->right_) return iterator(root, fake_);
        root = root->right_;
      }
    }
  }

  template< typename Key, typename Value, typename Compare >
  typename BinarySearchTree< Key, Value, Compare >::iterator
  BinarySearchTree< Key, Value, Compare >::upperBound(const Key &key)
  {
    Compare cmp{};
    TreeNode *root = fake_->parent_;
    if (!root) return end();
    if (key == getKey(fake_->right_)) return end();
    while (true)
    {
      if (key == getKey(root))
      {
        return ++iterator(root, fake_);
      }
      while (cmp(key, getKey(root)))
      {
        if (!root->left_) return iterator(root, fake_);
        root = root->left_;
      }
      while (cmp(getKey(root), key))
      {
        if (!root->right_) return iterator(root, fake_);
        root = root->right_;
      }
    }
  }

  template< typename Key, typename Value, typename Compare >
  void BinarySearchTree< Key, Value, Compare >::clean()
  {
    if (size_ > 0)
      erase(getKey(fake_->left_), getKey(fake_->right_));
  }

  template< typename Key, typename Value, typename Compare >
  unsigned char BinarySearchTree< Key, Value, Compare >::getHeight(BinarySearchTree::TreeNode *node)
  {
    return node ? node->height_ : 0;
  }

  template< typename Key, typename Value, typename Compare >
  void BinarySearchTree< Key, Value, Compare >::setHeight(TreeNode *node)
  {
    if (!node) throw std::logic_error("Set height error : nullptr node");
    unsigned char leftH = getHeight(node->left_);
    unsigned char rightH = getHeight(node->right_);
    node->height_ = (leftH > rightH ? leftH : rightH) + 1;
  }

  template< typename Key, typename Value, typename Compare >
  char BinarySearchTree< Key, Value, Compare >::getBalanceFactor(BinarySearchTree::TreeNode *node)
  {
    if (!node) throw std::logic_error("Balance factor error : nullptr node");
    return getHeight(node->right_) - getHeight(node->left_);
  }

  template< typename Key, typename Value, typename Compare >
  typename BinarySearchTree< Key, Value, Compare >::TreeNode *
  BinarySearchTree< Key, Value, Compare >::rotateRight(TreeNode *root)
  {
    if (!root || !root->left_) throw std::logic_error("Rotate left error : nullptr node");
    TreeNode *newRoot = root->left_;
    newRoot->parent_ = root->parent_;
    root->left_ = newRoot->right_;
    if (root->left_)
    {
      root->left_->parent_ = root;
    }
    newRoot->right_ = root;
    root->parent_ = newRoot;
    setHeight(root);
    setHeight(newRoot);
    return newRoot;
  }

  template< typename Key, typename Value, typename Compare >
  typename BinarySearchTree< Key, Value, Compare >::TreeNode *
  BinarySearchTree< Key, Value, Compare >::rotateLeft(TreeNode *root)
  {
    if (!root || !root->right_) throw std::logic_error("Rotate left error : nullptr node");
    TreeNode *newRoot = root->right_;
    newRoot->parent_ = root->parent_;
    root->right_ = newRoot->left_;
    if (root->right_)
    {
      root->right_->parent_ = root;
    }
    newRoot->left_ = root;
    root->parent_ = newRoot;
    setHeight(root);
    setHeight(newRoot);
    return newRoot;
  }

  template< typename Key, typename Value, typename Compare >
  typename BinarySearchTree< Key, Value, Compare >::TreeNode *
  BinarySearchTree< Key, Value, Compare >::balance(TreeNode *root)
  {
    setHeight(root);
    if (getBalanceFactor(root) == 2)
    {
      if (getBalanceFactor(root->right_) < 0)
        root->right_ = rotateRight(root->right_);
      root = rotateLeft(root);
    } else if (getBalanceFactor(root) == -2)
    {
      if (getBalanceFactor(root->left_) > 0)
        root->left_ = rotateLeft(root->left_);
      root = rotateRight(root);
    }
    return root;
  }

  template< typename Key, typename Value, typename Compare >
  typename BinarySearchTree< Key, Value, Compare >::TreeNode *
  BinarySearchTree< Key, Value, Compare >::insert(TreeNode *rootParent, TreeNode *root,
                                                  const std::pair< Key, Value > &data,
                                                  const Compare &cmp)
  {
    if (!root) return new TreeNode(data, rootParent);
    if (cmp(data.first, getKey(root)))
      root->left_ = insert(root, root->left_, data, cmp);
    else
      root->right_ = insert(root, root->right_, data, cmp);
    return balance(root);
  }

  template< typename Key, typename Value, typename Compare >
  const Key &BinarySearchTree< Key, Value, Compare >::getKey(TreeNode *root)
  {
    return root->data_.first;
  }

  template< typename Key, typename Value, typename Compare >
  Value &BinarySearchTree< Key, Value, Compare >::getValue(TreeNode *root)
  {
    return root->data_.second;
  }

  template< typename Key, typename Value, typename Compare >
  typename BinarySearchTree< Key, Value, Compare >::TreeNode *
  BinarySearchTree< Key, Value, Compare >::pop(TreeNode *root, const Key &key)
  {
    if (!root) return nullptr;
    if (key < getKey(root))
    {
      root->left_ = pop(root->left_, key);
      if (root->left_ && root->left_->parent_) root->left_->parent_ = root;
    } else if (key > getKey(root))
    {
      root->right_ = pop(root->right_, key);
      if (root->right_ && root->right_->parent_) root->right_->parent_ = root;
    } else
    {
      TreeNode *leftNode = root->left_;
      TreeNode *rightNode = root->right_;
      delete root;
      if (!rightNode) return leftNode;
      TreeNode *minNode = getMin(rightNode);
      minNode->right_ = extractMin(rightNode);
      minNode->left_ = leftNode;
      if (leftNode) leftNode->parent_ = minNode;
      if (minNode->right_) minNode->right_->parent_ = minNode;
      return balance(minNode);
    }
    return balance(root);
  }

  template< typename Key, typename Value, typename Compare >
  typename BinarySearchTree< Key, Value, Compare >::TreeNode *
  BinarySearchTree< Key, Value, Compare >::getMin(TreeNode *root)
  {
    while (root->left_) root = root->left_;
    return root;
  }

  template< typename Key, typename Value, typename Compare >
  typename BinarySearchTree< Key, Value, Compare >::TreeNode *
  BinarySearchTree< Key, Value, Compare >::extractMin(TreeNode *root)
  {
    if (!root->left_) return root->right_;
    root->left_ = extractMin(root->left_);
    return balance(root);
  }
}

#endif
