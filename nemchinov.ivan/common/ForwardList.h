#ifndef ForwardList_H
#define ForwardList_H

#include <stdexcept>
#include <iterator>
#include <initializer_list>

namespace nemchinov
{
  template< typename T >
  class ForwardList
  {
  public:
    ForwardList();
    ForwardList(const std::initializer_list< T > &);
    ~ForwardList();

    ForwardList(const ForwardList &);
    ForwardList &operator=(const ForwardList &);
    ForwardList(ForwardList &&) = delete;
    ForwardList &operator=(ForwardList &&) = delete;

    bool isEmpty() const noexcept;
    std::size_t size() const noexcept;
    void pushHead(const T &);
    void pushTail(const T &);
    void popTail();
    const T &getHead() const;
    const T &getTail() const;

  private:
    struct Node
    {
      T data_;
      Node *next_;

      explicit Node(const T &);
      Node(const T &, Node *);
    };

    Node *tail_;
    Node *head_;
    std::size_t size_{};

  public:
    class Iterator;

    class ConstIterator;

    using iterator = Iterator;
    using const_iterator = ConstIterator;

    void insert(iterator &, const T &);
    void pop(iterator &);
    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
  };

  template< typename T >
  class ForwardList< T >::Iterator
  {
    friend class ForwardList< T >;

  public:
    using iterator_type = T;
    using iterator_category = std::forward_iterator_tag;
    using value_type = iterator_type;
    using difference_type = ptrdiff_t;
    using reference = iterator_type &;
    using pointer = iterator_type *;
    using node = Node;
    using iter = Iterator;

    explicit Iterator(node *ptr);
    ~Iterator() = default;

    Iterator(const Iterator &it);
    Iterator &operator=(const Iterator &) = delete;

    Iterator(Iterator &&) noexcept = default;
    Iterator &operator=(Iterator &&) = delete;

    node *itNode();
    bool operator!=(iter const &other) const;
    bool operator==(iter const &other) const;
    reference operator*();
    pointer operator->();
    iter &operator++();
    iter &operator++(int);
    bool operator>(iter other) const;
    bool operator<(iter other) const;
    bool operator>=(iter other) const;
    bool operator<=(iter other) const;

  private:
    node *itNode_;
  };

  template< typename T >
  class ForwardList< T >::ConstIterator
  {
    friend class ForwardList< T >;

  public:
    using iterator_type = const T;
    using iterator_category = std::forward_iterator_tag;
    using value_type = iterator_type;
    using difference_type = ptrdiff_t;
    using reference = iterator_type &;
    using pointer = iterator_type *;
    using node = const Node;
    using iter = ConstIterator;

    explicit ConstIterator(node *ptr);
    ~ConstIterator() = default;

    ConstIterator(const ConstIterator &it);
    ConstIterator &operator=(const ConstIterator &) = delete;

    ConstIterator(ConstIterator &&) noexcept = default;
    ConstIterator &operator=(ConstIterator &&) = delete;

    node *itNode();
    bool operator!=(iter const &other) const;
    bool operator==(iter const &other) const;
    reference operator*();
    pointer operator->();
    iter &operator++();
    iter &operator++(int);
    bool operator>(iter other) const;
    bool operator<(iter other) const;
    bool operator>=(iter other) const;
    bool operator<=(iter other) const;

  private:
    node *itNode_;
  };

  template< typename T >
  ForwardList< T >::ForwardList():
          tail_(nullptr), head_(nullptr), size_(0)
  {
  }

  template< typename T >
  ForwardList< T >::ForwardList(const std::initializer_list< T > &initList) : ForwardList()
  {
    for (const T &value : initList)
    {
      this->pushHead(value);
    }
  }

  template< typename T >
  ForwardList< T >::~ForwardList()
  {
    while (!isEmpty())
    {
      popTail();
    }
  }

  template< typename T >
  ForwardList< T >::ForwardList(const ForwardList &other)
  {
    if (!other.isEmpty())
    {
      for (auto &it : other)
      {
        this->pushHead(it);
      }
    }
    this->tail_ = nullptr;
    this->head_ = nullptr;
  }

  template< typename T >
  ForwardList< T > &ForwardList< T >::operator=(const ForwardList &other)
  {
    if (!other.isEmpty())
    {
      for (const auto &it : other)
      {
        this->pushHead(it);
      }
    } else
    {
      this->tail_ = nullptr;
      this->head_ = nullptr;
    }
    return *this;
  }

  template< typename T >
  bool ForwardList< T >::isEmpty() const noexcept
  {
    return !size_;
  }

  template< typename T >
  std::size_t ForwardList< T >::size() const noexcept
  {
    return size_;
  }

  template< typename T >
  void ForwardList< T >::pushHead(const T &data)
  {
    if (isEmpty())
    {
      head_ = new Node(data, nullptr);
      tail_ = head_;
      ++size_;
      return;
    }
    Node *newHead = new Node(data, nullptr);
    head_->next_ = newHead;
    head_ = newHead;
    ++size_;
  }

  template< typename T >
  void ForwardList< T >::pushTail(const T &data)
  {
    if (isEmpty())
    {
      pushHead(data);
      return;
    }
    Node *newTail = new Node(data, tail_);
    tail_ = newTail;
    ++size_;
  }

  template< typename T >
  void ForwardList< T >::insert(ForwardList::iterator &insertionPoint, const T &data)
  {
    if (insertionPoint.itNode_ == tail_) return pushTail(data);
    if (insertionPoint.itNode_ == nullptr) return pushHead(data);
    Node *next = insertionPoint.itNode();
    Node *newNode = new Node(data, next);

    iterator it = begin();
    while (it.itNode_->next_ != next)
    {
      ++it;
    }

    it.itNode()->next_ = newNode;
    ++size_;
  }

  template< typename T >
  void ForwardList< T >::pop(ForwardList::iterator &popPoint)
  {
    if (popPoint.itNode_ == tail_)
    {
      popTail();
      popPoint.itNode_ = tail_;
      return;
    }

    if (isEmpty())
    {
      throw std::logic_error("Forward list pop error: forward list is empty");
    }
    if (popPoint.itNode_ == nullptr) throw std::out_of_range("Pop error : out of range");

    Node *next = popPoint.itNode()->next_;

    iterator it = begin();
    while (it.itNode_->next_ != popPoint.itNode())
    {
      ++it;
    }
    it.itNode()->next_ = next;

    delete popPoint.itNode();
    popPoint.itNode_ = it.itNode();
    --size_;
  }

  template< typename T >
  void ForwardList< T >::popTail()
  {
    if (isEmpty())
    {
      throw std::logic_error("Forward list pop tail error: forward list is empty");
    }
    if (tail_ != head_)
    {
      Node *newTail = tail_->next_;
      delete tail_;
      tail_ = newTail;
    } else
    {
      delete tail_;
      tail_ = nullptr;
      head_ = nullptr;
    }
    --size_;
  }

  template< typename T >
  const T &ForwardList< T >::getHead() const
  {
    if (ForwardList< T >::isEmpty())
    {
      throw std::logic_error("ERROR : fList is empty");
    }
    return head_->data_;
  }

  template< typename T >
  const T &ForwardList< T >::getTail() const
  {
    if (ForwardList< T >::isEmpty())
    {
      throw std::logic_error("ERROR : fList is empty");
    }
    return tail_->data_;
  }

  template< typename T >
  typename ForwardList< T >::iterator ForwardList< T >::begin()
  {
    return iterator(tail_);
  }

  template< typename T >
  typename ForwardList< T >::iterator ForwardList< T >::end()
  {
    return iterator(nullptr);
  }

  template< typename T >
  typename ForwardList< T >::const_iterator ForwardList< T >::begin() const
  {
    return const_iterator(tail_);
  }

  template< typename T >
  typename ForwardList< T >::const_iterator ForwardList< T >::end() const
  {
    return const_iterator(nullptr);
  }

  template< typename T >
  ForwardList< T >::Node::Node(const T &data):
          Node(data, nullptr)
  {
  }

  template< typename T >
  ForwardList< T >::Node::Node(const T &data, Node *next):
          data_(data), next_(next)
  {
  }

  template< typename T >
  ForwardList< T >::Iterator::Iterator(Node *ptr): itNode_(ptr)
  {
  }

  template< typename T >
  ForwardList< T >::Iterator::Iterator(const Iterator &it) : itNode_(it.itNode_)
  {
  }

  template< typename T >
  typename ForwardList< T >::Iterator::node *ForwardList< T >::Iterator::itNode()
  {
    return itNode_;
  }

  template< typename T >
  bool ForwardList< T >::Iterator::operator!=(iter const &other) const
  {
    return itNode_ != other.itNode_;
  }

  template< typename T >
  bool ForwardList< T >::Iterator::operator==(iter const &other) const
  {
    return itNode_ == other.itNode_;
  }

  template< typename T >
  typename ForwardList< T >::Iterator::reference ForwardList< T >::Iterator::operator*()
  {
    if (itNode_ == nullptr) throw std::runtime_error("Error : bad iterator");
    return itNode_->data_;
  }

  template< typename T >
  typename ForwardList< T >::Iterator::pointer ForwardList< T >::Iterator::operator->()
  {
    return &itNode_->data_;
  }

  template< typename T >
  typename ForwardList< T >::Iterator &ForwardList< T >::Iterator::operator++()
  {
    if (itNode_ == nullptr) throw std::out_of_range("Error iterator++ : out of range");
    itNode_ = itNode_->next_;
    return *this;
  }

  template< typename T >
  typename ForwardList< T >::Iterator &ForwardList< T >::Iterator::operator++(int)
  {
    return this->operator++();
  }

  template< typename T >
  bool ForwardList< T >::Iterator::operator>(iter other) const
  {
    if (this->itNode_ == nullptr && other.itNode_ != nullptr) return true;
    if (this->itNode_ != nullptr && other.itNode_ == nullptr) return false;
    iterator this_ = *this;
    while (this_.itNode_ != nullptr)
    {
      if (this_ == other) return false;
      ++this_;
    }
    return true;
  }

  template< typename T >
  bool ForwardList< T >::Iterator::operator<(iter other) const
  {
    return !this->operator>(other) && *this != other;
  }

  template< typename T >
  bool ForwardList< T >::Iterator::operator>=(iter other) const
  {
    return this == other || this > other;
  }

  template< typename T >
  bool ForwardList< T >::Iterator::operator<=(iter other) const
  {
    return this == other || this < other;
  }

  template< typename T >
  ForwardList< T >::ConstIterator::ConstIterator(node *ptr): itNode_(ptr)
  {
  }

  template< typename T >
  ForwardList< T >::ConstIterator::ConstIterator(const ConstIterator &it) : itNode_(it.itNode_)
  {
  }

  template< typename T >
  typename ForwardList< T >::ConstIterator::node *ForwardList< T >::ConstIterator::itNode()
  {
    return itNode_;
  }

  template< typename T >
  bool ForwardList< T >::ConstIterator::operator!=(iter const &other) const
  {
    return itNode_ != other.itNode_;
  }

  template< typename T >
  bool ForwardList< T >::ConstIterator::operator==(iter const &other) const
  {
    return itNode_ == other.itNode_;
  }

  template< typename T >
  typename ForwardList< T >::ConstIterator::reference ForwardList< T >::ConstIterator::operator*()
  {
    if (itNode_ == nullptr) throw std::runtime_error("Error : bad iterator");
    return itNode_->data_;
  }

  template< typename T >
  typename ForwardList< T >::ConstIterator::pointer ForwardList< T >::ConstIterator::operator->()
  {
    return &itNode_->data_;
  }

  template< typename T >
  typename ForwardList< T >::ConstIterator &ForwardList< T >::ConstIterator::operator++()
  {
    if (itNode_ == nullptr) throw std::runtime_error("Error : bad iterator");
    itNode_ = itNode_->next_;
    return *this;
  }

  template< typename T >
  typename ForwardList< T >::ConstIterator &ForwardList< T >::ConstIterator::operator++(int)
  {
    return this->operator++();
  }

  template< typename T >
  bool ForwardList< T >::ConstIterator::operator>(iter other) const
  {
    if (this->itNode_ == nullptr && other.itNode_ != nullptr) return true;
    if (this->itNode_ != nullptr && other.itNode_ == nullptr) return false;
    iterator this_ = *this;
    while (this_.itNode_ != nullptr)
    {
      if (this_ == other) return false;
      ++this_;
    }
    return true;
  }

  template< typename T >
  bool ForwardList< T >::ConstIterator::operator<(iter other) const
  {
    return !this->operator>(other) && *this != other;
  }

  template< typename T >
  bool ForwardList< T >::ConstIterator::operator>=(iter other) const
  {
    return this == other || this > other;
  }

  template< typename T >
  bool ForwardList< T >::ConstIterator::operator<=(iter other) const
  {
    return this == other || this < other;
  }
}

#endif
