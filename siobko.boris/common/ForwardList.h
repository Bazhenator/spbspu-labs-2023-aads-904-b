
#ifndef FORWARD_LIST_H
#define FORWARD_LIST_H

#include <stdexcept>
#include <utility>
#include <cassert>

namespace siobko {
  template< class Ty >
  class ForwardList {
  public:
    struct ConstIterator;
    struct Iterator;

  public:
    using iterator = Iterator;
    using const_iterator = ConstIterator;
    using reference = Ty&;
    using const_reference = const Ty&;
    using value_type = Ty;
    using size_type = std::size_t;

    ForwardList();
    ForwardList(const ForwardList& rhs);
    ForwardList(ForwardList&& rhs) noexcept;
    ForwardList(std::initializer_list< value_type > IList);
    ~ForwardList();

    ForwardList& operator=(const ForwardList& other);
    ForwardList& operator=(ForwardList&& other) noexcept;

    iterator begin() noexcept;
    iterator end() noexcept;
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;

    reference front() noexcept;
    const_reference front() const noexcept;

    void insertAfter(const Ty& val, iterator where);
    void pushFront(const Ty& val);
    void emplaceFront(const Ty& val);
    void popFront();
    bool isEmpty() const noexcept;
    void clear() noexcept;
    size_type size() const noexcept;

  private:
    struct Node {
      explicit Node(const Ty& value, Node *next = nullptr) :
        value_(value),
        next_(next)
      {};
      ~Node() = default;

      Ty value_;
      Node *next_;
    };

    void pushBack(const Ty& val);

    Node *head_;
    std::size_t size_;
  };


  template< class Ty >
  class ForwardList< Ty >::ConstIterator {
  public:
    using const_reference = const Ty&;
    using pointer = const Ty *;

    ConstIterator() = default;
    explicit ConstIterator(Node *rhs);
    ~ConstIterator() = default;

    const_reference operator*() const noexcept;
    pointer operator->() const noexcept;
    ConstIterator& operator++();
    ConstIterator operator++(int);
    bool operator!=(const ConstIterator& other) const noexcept;
    bool operator==(const ConstIterator& other) const noexcept;

    Node *current_;
  };

  template< typename Ty >
  ForwardList< Ty >::ConstIterator::ConstIterator(Node *rhs):
    current_(rhs)
  {}

  template< typename Ty >
  typename ForwardList< Ty >::ConstIterator::const_reference ForwardList< Ty >::ConstIterator::operator*() const noexcept
  {
    return current_->value_;
  }

  template< typename Ty >
  typename ForwardList< Ty >::ConstIterator::pointer ForwardList< Ty >::ConstIterator::ConstIterator::operator->() const noexcept
  {
    return std::pointer_traits< pointer >::pointer_to(**this);
  }

  template< typename Ty >
  typename ForwardList< Ty >::ConstIterator& ForwardList< Ty >::ConstIterator::operator++()
  {
    current_ = current_->next_;
    return *this;
  }

  template< typename Ty >
  typename ForwardList< Ty >::ConstIterator ForwardList< Ty >::ConstIterator::operator++(int)
  {
    ConstIterator tmp(current_);
    ++(*this);
    return tmp;
  }

  template< typename Ty >
  bool ForwardList< Ty >::ConstIterator::operator!=(const ConstIterator& other) const noexcept
  {
    return current_ != other.current_;
  }

  template< typename Ty >
  bool ForwardList< Ty >::ConstIterator::operator==(const ConstIterator& other) const noexcept
  {
    return current_ == other.current_;
  }

  template< class Ty >
  class ForwardList< Ty >::Iterator {
  public:
    using reference = Ty&;
    using pointer = Ty *;

    Iterator() = default;
    explicit Iterator(Node *rhs);
    ~Iterator() = default;

    reference operator*() const noexcept;
    pointer operator->() const noexcept;
    Iterator& operator++();
    Iterator operator++(int);
    bool operator!=(const Iterator& other) const noexcept;
    bool operator==(const Iterator& other) const noexcept;

    Node *current_;
  };

  template< typename Ty >
  ForwardList< Ty >::Iterator::Iterator(Node *rhs):
    current_(rhs)
  {}

  template< typename Ty >
  typename ForwardList< Ty >::Iterator::reference ForwardList< Ty >::Iterator::operator*() const noexcept
  {
    return current_->value_;
  }

  template< typename Ty >
  typename ForwardList< Ty >::Iterator::pointer ForwardList< Ty >::Iterator::Iterator::operator->() const noexcept
  {
    return std::pointer_traits< pointer >::pointer_to(**this);
  }

  template< typename Ty >
  typename ForwardList< Ty >::Iterator& ForwardList< Ty >::Iterator::operator++()
  {
    current_ = current_->next_;
    return *this;
  }

  template< typename Ty >
  typename ForwardList< Ty >::Iterator ForwardList< Ty >::Iterator::operator++(int)
  {
    Iterator tmp(current_);
    ++(*this);
    return tmp;
  }

  template< typename Ty >
  bool ForwardList< Ty >::Iterator::operator!=(const Iterator& other) const noexcept
  {
    return current_ != other.current_;
  }

  template< typename Ty >
  bool ForwardList< Ty >::Iterator::operator==(const Iterator& other) const noexcept
  {
    return current_ == other.current_;
  }

  template< class Ty >
  ForwardList< Ty >::ForwardList():
    head_(nullptr),
    size_(0u)
  {}

  template< class Ty >
  ForwardList< Ty >::ForwardList(const ForwardList< Ty >& rhs):
    head_(nullptr),
    size_(0u)
  {
    for (const_iterator it = rhs.begin(); it != rhs.end(); ++it) {
      pushBack(*it);
    }
  }

  template< class Ty >
  ForwardList< Ty >::ForwardList(ForwardList< Ty >&& rhs) noexcept:
    head_(nullptr),
    size_(0u)
  {
    std::swap(rhs.head_, head_);
    std::swap(rhs.size_, size_);
  }

  template< class Ty >
  ForwardList< Ty >::ForwardList(std::initializer_list< value_type > IList):
    head_(nullptr),
    size_(0u)
  {
    for (auto item: IList) {
      pushFront(item);
    }
  }

  template< class Ty >
  ForwardList< Ty >::~ForwardList()
  {
    clear();
  }

  template< class Ty >
  ForwardList< Ty >& ForwardList< Ty >::operator=(const ForwardList& other)
  {
    if (this != &other) {
      (*this) = ForwardList< Ty >(other);
    }
    return *this;
  }

  template< class Ty >
  ForwardList< Ty >& ForwardList< Ty >::operator=(ForwardList&& other) noexcept
  {
    if (this != &other) {
      std::swap(head_, other.head_);
      std::swap(size_, other.size_);
    }
    return *this;
  }

  template< class Ty >
  typename ForwardList< Ty >::iterator ForwardList< Ty >::begin() noexcept
  {
    return iterator(head_);
  }

  template< class Ty >
  typename ForwardList< Ty >::iterator ForwardList< Ty >::end() noexcept
  {
    return iterator(nullptr);
  }

  template< class Ty >
  typename ForwardList< Ty >::const_iterator ForwardList< Ty >::begin() const noexcept
  {
    return const_iterator(head_);
  }

  template< class Ty >
  typename ForwardList< Ty >::const_iterator ForwardList< Ty >::end() const noexcept
  {
    return const_iterator(nullptr);
  }

  template< class Ty >
  typename ForwardList< Ty >::const_iterator ForwardList< Ty >::cbegin() const noexcept
  {
    return const_iterator(head_);
  }

  template< class Ty >
  typename ForwardList< Ty >::const_iterator ForwardList< Ty >::cend() const noexcept
  {
    return const_iterator(nullptr);
  }

  template< class Ty >
  typename ForwardList< Ty >::reference ForwardList< Ty >::front() noexcept
  {
    return head_->value_;
  }

  template< class Ty >
  typename ForwardList< Ty >::const_reference ForwardList< Ty >::front() const noexcept
  {
    return head_->value_;
  }

  template< class Ty >
  void ForwardList< Ty >::insertAfter(const Ty& val, iterator where)
  {
    iterator it = begin();
    size_++;

    while (it != end() || it == where) {
      if (it == where) {
        Node *newNode = new Node(val);
        newNode->next_ = where.current_->next_;
        where.current_->next_ = newNode;
        return;
      }
      ++it;
    }

    throw std::logic_error("ForwardList insert error: cannot find iterator.");
  }

  template< class Ty >
  void ForwardList< Ty >::pushFront(const Ty& val)
  {
    head_ = new Node(val, head_);
    size_++;
  }

  template< class Ty >
  void ForwardList< Ty >::emplaceFront(const Ty& val)
  {
    head_ = new Node(std::forward< Ty >(val), head_);
  }

  template< class Ty >
  void ForwardList< Ty >::popFront()
  {
    if (size_ == 0) {
      throw std::logic_error("pop front error: ForwardList is empty.");
    }
    Node *tmp = head_->next_;
    delete head_;
    head_ = tmp;
    --size_;
  }

  template< class Ty >
  bool ForwardList< Ty >::isEmpty() const noexcept
  {
    return head_ == nullptr;
  }

  template< class Ty >
  void ForwardList< Ty >::clear() noexcept
  {
    while (!isEmpty()) {
      popFront();
    }
  }

  template< class Ty >
  typename ForwardList< Ty >::size_type ForwardList< Ty >::size() const noexcept
  {
    return size_;
  }

  template< class Ty >
  void ForwardList< Ty >::pushBack(const Ty& val)
  {
    size_++;
    if (head_ == nullptr) {
      head_ = new Node(val);
      return;
    }
    Node *current = this->head_;
    while (current->next_ != nullptr) {
      current = current->next_;
    }
    current->next_ = new Node(val);
  }
}
#endif
