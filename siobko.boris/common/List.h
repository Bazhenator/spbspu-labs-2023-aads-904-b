
#ifndef LIST_H
#define LIST_H

#include <stdexcept>
#include <cstddef>
#include <utility>
#include <cassert>

namespace siobko {
  template< class Ty >
  class List {
  public:
    struct ConstIterator;
    struct Iterator;

  public:
    using size_type = std::size_t;
    using iterator = Iterator;
    using const_iterator = ConstIterator;
    using reference = Ty&;
    using const_reference = const Ty&;
    using value_type = Ty;

    List();
    List(const List& rhs);
    List(List&& rhs) noexcept;
    List(std::initializer_list< value_type > IList);
    ~List();

    List& operator=(const List& other);
    List& operator=(List&& other) noexcept;

    iterator begin() noexcept;
    iterator end() noexcept;
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;

    reference back() noexcept;
    reference front() noexcept;
    const_reference back() const noexcept;
    const_reference front() const noexcept;

    void insert(const Ty& val, const_iterator where);
    void pushBack(const Ty& val);
    void pushFront(const Ty& val);
    void emplaceBack(const Ty& val);
    void emplaceFront(const Ty& val);
    void popBack();
    void popFront();
    bool isEmpty() const noexcept;
    void clear() noexcept;
    size_type size() const noexcept;

  private:
    struct Node {
      explicit Node(const Ty& val, Node *next = nullptr, Node *prev = nullptr) :
        data_(val),
        next_(next),
        prev_(prev)
      {};
      ~Node() = default;

      Ty data_;
      Node *next_;
      Node *prev_;
    };

    Node *head_;
    Node *tail_;
    size_t size_;
  };

  template< class Ty >
  struct List< Ty >::ConstIterator {
    using const_reference = const Ty&;
    using pointer = const Ty *;

    ConstIterator() = default;
    explicit ConstIterator(Node *p);
    ~ConstIterator() = default;

    const_reference operator*() const noexcept;
    pointer operator->() const noexcept;
    ConstIterator& operator++();
    ConstIterator operator++(int);
    ConstIterator& operator--();
    ConstIterator operator--(int);
    bool operator!=(const ConstIterator& other) const noexcept;
    bool operator==(const ConstIterator& other) const noexcept;

    Node *current_;
  };

  template< class Ty >
  List< Ty >::ConstIterator::ConstIterator(Node *p):
    current_(p)
  {}

  template< typename Ty >
  typename List< Ty >::ConstIterator::const_reference List< Ty >::ConstIterator::operator*() const noexcept
  {
    return current_->data_;
  }

  template< typename Ty >
  typename List< Ty >::ConstIterator::pointer List< Ty >::ConstIterator::ConstIterator::operator->() const noexcept
  {
    return std::addressof(current_->data_);
  }

  template< typename Ty >
  typename List< Ty >::ConstIterator& List< Ty >::ConstIterator::operator++()
  {
    current_ = current_->next_;
    return *this;
  }

  template< typename Ty >
  typename List< Ty >::ConstIterator List< Ty >::ConstIterator::operator++(int)
  {
    ConstIterator was(current_);
    ++(*this);
    return was;
  }

  template< typename Ty >
  typename List< Ty >::ConstIterator& List< Ty >::ConstIterator::operator--()
  {
    current_ = current_->prev_;
    return *this;
  }

  template< typename Ty >
  typename List< Ty >::ConstIterator List< Ty >::ConstIterator::operator--(int)
  {
    ConstIterator was(current_);
    --(*this);
    return was;
  }

  template< typename Ty >
  bool List< Ty >::ConstIterator::operator==(const ConstIterator& other) const noexcept
  {
    return current_ == other.current_;
  }

  template< typename Ty >
  bool List< Ty >::ConstIterator::operator!=(const ConstIterator& other) const noexcept
  {
    return current_ != other.current_;
  }

  template< class Ty >
  struct List< Ty >::Iterator {
    using reference = Ty&;
    using pointer = Ty *;

    Iterator() = default;
    explicit Iterator(Node *p);
    ~Iterator() = default;

    reference operator*() const noexcept;
    pointer operator->() const noexcept;
    Iterator& operator++();
    Iterator operator++(int);
    Iterator& operator--();
    Iterator operator--(int);
    bool operator!=(const Iterator& other) const noexcept;
    bool operator==(const Iterator& other) const noexcept;

    Node *current_;
  };

  template< class Ty >
  List< Ty >::Iterator::Iterator(Node *p):
    current_(p)
  {}

  template< typename Ty >
  typename List< Ty >::Iterator::reference List< Ty >::Iterator::operator*() const noexcept
  {
    return current_->data_;
  }

  template< typename Ty >
  typename List< Ty >::Iterator::pointer List< Ty >::Iterator::Iterator::operator->() const noexcept
  {
    return std::pointer_traits< pointer >::pointer_to(**this);
  }

  template< typename Ty >
  typename List< Ty >::Iterator& List< Ty >::Iterator::operator++()
  {
    current_ = current_->next_;
    return *this;
  }

  template< typename Ty >
  typename List< Ty >::Iterator List< Ty >::Iterator::operator++(int)
  {
    Iterator was(current_);
    ++(*this);
    return was;
  }

  template< typename Ty >
  typename List< Ty >::Iterator& List< Ty >::Iterator::operator--()
  {
    current_ = current_->prev_;
    return *this;
  }

  template< typename Ty >
  typename List< Ty >::Iterator List< Ty >::Iterator::operator--(int)
  {
    Iterator was(current_);
    --(*this);
    return was;
  }

  template< typename Ty >
  bool List< Ty >::Iterator::operator==(const Iterator& other) const noexcept
  {
    return current_ == other.current_;
  }

  template< typename Ty >
  bool List< Ty >::Iterator::operator!=(const Iterator& other) const noexcept
  {
    return current_ != other.current_;
  }

  template< class Ty >
  List< Ty >::List():
    head_(nullptr),
    tail_(nullptr),
    size_(0u)
  {}

  template< class Ty >
  List< Ty >::List(const List< Ty >& rhs):
    head_(nullptr),
    tail_(nullptr),
    size_(0u)
  {
    for (const_iterator it = rhs.begin(); it != rhs.end(); ++it) {
      pushBack(*it);
    }
  }

  template< class Ty >
  List< Ty >::List(List< Ty >&& rhs) noexcept:
    head_(nullptr),
    tail_(nullptr),
    size_(0u)
  {
    std::swap(rhs.head_, head_);
    std::swap(rhs.tail_, tail_);
    std::swap(rhs.size_, size_);
  }

  template< class Ty >
  List< Ty >::List(std::initializer_list< value_type > IList):
    head_(nullptr),
    tail_(nullptr),
    size_(0u)
  {
    for (auto item: IList) {
      pushBack(item);
    }
  }

  template< class Ty >
  List< Ty >::~List()
  {
    clear();
  }

  template< class Ty >
  List< Ty >& List< Ty >::operator=(const List& other)
  {
    if (this != other) {
      (*this) = List< Ty >(other);
    }
    return *this;
  }

  template< class Ty >
  List< Ty >& List< Ty >::operator=(List&& other) noexcept
  {
    if (this != &other) {
      std::swap(head_, other.head_);
      std::swap(tail_, other.tail_);
      std::swap(size_, other.size_);
    }
    return *this;
  }

  template< class Ty >
  typename List< Ty >::iterator List< Ty >::begin() noexcept
  {
    return iterator(head_);
  }

  template< class Ty >
  typename List< Ty >::iterator List< Ty >::end() noexcept
  {
    return iterator(nullptr);
  }

  template< class Ty >
  typename List< Ty >::const_iterator List< Ty >::begin() const noexcept
  {
    return const_iterator(head_);
  }

  template< class Ty >
  typename List< Ty >::const_iterator List< Ty >::end() const noexcept
  {
    return const_iterator(nullptr);
  }

  template< class Ty >
  typename List< Ty >::const_iterator List< Ty >::cbegin() const noexcept
  {
    return const_iterator(head_);
  }

  template< class Ty >
  typename List< Ty >::const_iterator List< Ty >::cend() const noexcept
  {
    return const_iterator(nullptr);
  }

  template< class Ty >
  typename List< Ty >::reference List< Ty >::front() noexcept
  {
    return head_->data_;
  }

  template< class Ty >
  typename List< Ty >::reference List< Ty >::back() noexcept
  {
    return tail_->data_;
  }

  template< class Ty >
  typename List< Ty >::const_reference List< Ty >::front() const noexcept
  {
    return head_->data_;
  }

  template< class Ty >
  typename List< Ty >::const_reference List< Ty >::back() const noexcept
  {
    return tail_->data_;
  }

  template< class Ty >
  void List< Ty >::insert(const Ty& val, const_iterator where)
  {
    if (where == head_) {
      pushFront(val);
      return;
    }
    Node *newNode = new Node(val, where.current_, where.current_->prev_);
    where.current_->prev_->next_ = newNode;
    where.current_->prev_ = newNode;
    size_++;
  }

  template< class Ty >
  void List< Ty >::pushBack(const Ty& val)
  {
    Node *newNode = new Node(val, nullptr, tail_);
    if (isEmpty()) {
      tail_ = newNode;
      head_ = newNode;
    } else {
      tail_->next_ = newNode;
      tail_ = newNode;
    }
    size_++;
  }

  template< class Ty >
  void List< Ty >::pushFront(const Ty& val)
  {
    Node *newNode = new Node(val, head_, nullptr);
    if (isEmpty()) {
      head_ = newNode;
      tail_ = newNode;
    } else {
      head_->prev_ = newNode;
      head_ = newNode;
    }
    size_++;
  }

  template< class Ty >
  void List< Ty >::emplaceBack(const Ty& val)
  {
    Node *newNode = new Node(std::forward< Ty >(val));
    size_++;
    if (isEmpty()) {
      head_ = newNode;
      tail_ = newNode;
      return;
    }
    newNode->prev_ = tail_;
    tail_ = newNode;
  }

  template< class Ty >
  void List< Ty >::emplaceFront(const Ty& val)
  {
    Node *newNode = new Node(std::forward< Ty >(val));
    size_++;
    if (isEmpty()) {
      head_ = newNode;
      tail_ = newNode;
      return;
    }
    newNode->next_ = head_;
    head_ = newNode;
  }

  template< class Ty >
  void List< Ty >::popFront()
  {
    if (isEmpty()) {
      throw std::logic_error("List pop front error: List is empty.");
    }

    if (size_ == 1) {
      delete head_;
      head_ = nullptr;
      tail_ = nullptr;
    } else {
      head_ = head_->next_;
      delete head_->prev_;
      head_->prev_ = nullptr;
    }
    size_--;
  }

  template< class Ty >
  void List< Ty >::popBack()
  {
    if (isEmpty()) {
      throw std::logic_error("List pop back error: List is empty.");
    }

    if (size_ == 1) {
      delete tail_;
      head_ = nullptr;
      tail_ = nullptr;
    } else {
      tail_ = tail_->prev_;
      delete tail_->next_;
      tail_->next_ = nullptr;
    }
    size_--;
  }

  template< class Ty >
  bool List< Ty >::isEmpty() const noexcept
  {
    return (head_ == nullptr);
  }

  template< class Ty >
  void List< Ty >::clear() noexcept
  {
    while (!isEmpty()) {
      popBack();
    }
  }

  template< class Ty >
  typename List< Ty >::size_type List< Ty >::size() const noexcept
  {
    return size_;
  }
}
#endif
