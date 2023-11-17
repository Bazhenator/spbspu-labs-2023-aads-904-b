#ifndef FORWARDLIST_H
#define FORWARDLIST_H

#include <iostream>
#include <cstddef>

namespace pelipeychenko
{
  template< typename T >
  class ForwardList
  {
  public:
    class ConstIterator;

    class Iterator;

    ForwardList();

    ForwardList(const ForwardList< T > &rhs);

    ForwardList(ForwardList< T > &&rhs) noexcept;

    ForwardList(std::initializer_list< T >);

    ~ForwardList();

    void pushBack(const T &instance);

    void pushFront(const T &instance);

    void remove(const T &instance);

    bool find(const T &instance) const noexcept;

    Iterator insert(const Iterator &pos, const T &instance);

    template< typename...Args >
    Iterator emplace(const Iterator &pos, Args &&... args);

    Iterator insert(const Iterator &pos, const ForwardList< T > &list);

    Iterator erase(const Iterator &pos);

    void dropFront();

    void dropBack();

    const T &getHead() const;

    const T &getTail() const;

    bool isEmpty() const noexcept;

    std::size_t getSize() const noexcept;

    void printElements() const;

    void swap(ForwardList< T > &instance) noexcept;

    ConstIterator cbegin() const noexcept;

    ConstIterator cend() const noexcept;

    Iterator begin() noexcept;

    Iterator end() noexcept;

    bool operator==(const ForwardList< T > &instance);

  private:
    struct Node
    {
      explicit Node(const T &data):
        data_(data),
        next_(nullptr)
      {}

      T data_;
      Node *next_;
    };

    Node *head_;
    std::size_t size_;
  };

  template< typename T >
  ForwardList< T >::ForwardList() :
    head_(nullptr),
    size_(0)
  {}

  template< typename T >
  ForwardList< T >::ForwardList(const ForwardList< T > &rhs) :
    head_(nullptr),
    size_(0)
  {
    for (auto iterator = rhs.cbegin(); iterator != rhs.cend(); iterator++) {
      pushBack(*iterator);
    }
  }

  template< typename T >
  ForwardList< T >::ForwardList(ForwardList< T > &&rhs) noexcept:
    head_(rhs.head_),
    size_(rhs.size_)
  {
    rhs.size_ = 0;
    rhs.head_ = nullptr;
  }

  template< typename T >
  ForwardList< T >::ForwardList(std::initializer_list< T > list):
    head_(nullptr),
    size_(0)
  {
    for (auto i: list) {
      pushBack(i);
    }
  }

  template< typename T >
  ForwardList< T >::~ForwardList()
  {
    while (!isEmpty()) {
      dropFront();
    }
  }

  template< typename T >
  void ForwardList< T >::pushBack(const T &instance)
  {
    if (size_ != 0) {
      Node *element = head_;
      for (std::size_t i = 0; i < size_ - 1; i++) {
        element = element->next_;
      }
      element->next_ = new Node{instance};
    } else {
      head_ = new Node{instance};
    }
    size_++;
  }

  template< typename T >
  void ForwardList< T >::pushFront(const T &instance)
  {
    Node *temp = head_;
    head_ = new Node{instance};
    head_->next_ = temp;
    size_++;
  }

  template< typename T >
  void ForwardList< T >::dropFront()
  {
    Node *oldHead = head_;
    head_ = head_->next_;
    delete oldHead;
    size_--;
  }

  template< typename T >
  void ForwardList< T >::dropBack()
  {
    Node *currentElement = head_;
    while (currentElement->next_->next_) {
      currentElement = currentElement->next_;
    }
    delete currentElement->next_;
    currentElement->next_ = nullptr;
    size_--;
  }

  template< typename T >
  bool ForwardList< T >::isEmpty() const noexcept
  {
    return (size_ == 0);
  }

  template< typename T >
  std::size_t ForwardList< T >::getSize() const noexcept
  {
    return size_;
  }

  template< typename T >
  void ForwardList< T >::printElements() const
  {
    for (auto iterator = cbegin(); iterator != cend(); iterator++) {
      std::cout << " " << *(iterator);
    }
  }

  template< typename T >
  void ForwardList< T >::swap(ForwardList< T > &instance) noexcept
  {
    std::swap(head_, instance.head_);
    std::swap(size_, instance.size_);
  }

  template< typename T >
  typename ForwardList< T >::Iterator ForwardList< T >::begin() noexcept
  {
    Iterator it;
    it.node_ = head_;
    return it;
  }

  template< typename T >
  typename ForwardList< T >::Iterator ForwardList< T >::end() noexcept
  {
    Iterator it;
    it.node_ = head_;
    while (it.node_ != nullptr) {
      it++;
    }
    return it;
  }

  template< typename T >
  typename ForwardList< T >::ConstIterator ForwardList< T >::cbegin() const noexcept
  {
    ConstIterator it;
    it.node_ = head_;
    return it;
  }

  template< typename T >
  typename ForwardList< T >::ConstIterator ForwardList< T >::cend() const noexcept
  {
    ConstIterator it = cbegin();
    while (it.node_ != nullptr) {
      it++;
    }
    return it;
  }

  template< typename T >
  const T &ForwardList< T >::getHead() const
  {
    return head_->data_;
  }

  template< typename T >
  const T &ForwardList< T >::getTail() const
  {
    Node *tail = head_;
    while (tail->next_) {
      tail = tail->next_;
    }
    return tail->data_;
  }

  template< typename T >
  typename ForwardList< T >::Iterator ForwardList< T >::insert(const Iterator &pos, const T &instance)
  {
    Node *newNode = new Node{instance};
    newNode->next_ = (pos.node_)->next_;
    (pos.node_)->next_ = newNode;
    size_++;
    Iterator temp = pos + 1;
    return temp;
  }

  template< typename T >
  typename ForwardList< T >::Iterator
  ForwardList< T >::insert(const ForwardList::Iterator &pos, const ForwardList< T > &list)
  {
    Iterator it = cbegin();
    for (size_t i = 0; i < list.getSize(); i++) {
      insert(pos, *(it++));
    }
    return it;
  }

  template< typename T >
  template< typename...Args >
  typename ForwardList< T >::Iterator ForwardList< T >::emplace(const Iterator &pos, Args &&... args)
  {
    return insert(pos, T(args...));
  }

  template< typename T >
  typename ForwardList< T >::Iterator ForwardList< T >::erase(const ForwardList::Iterator &pos)
  {
    Node *toDelete = (pos.node_)->next_;
    (pos.node_)->next_ = toDelete->next_;
    delete toDelete;
    size_--;
    Iterator temp = pos;
    return temp;
  }

  template< typename T >
  bool ForwardList< T >::find(const T &instance) const noexcept
  {
    if (isEmpty()) {
      return false;
    }
    for (auto it = cbegin(); it != cend(); it++) {
      if (*it == instance) {
        return true;
      }
    }
    return false;
  }

  template< typename T >
  void ForwardList< T >::remove(const T &instance)
  {
    if (head_->data_ == instance) {
      dropFront();
      return;
    }
    for (auto iterator = begin(); iterator != end(); iterator++) {
      if (*(iterator + 1) == instance) {
        erase(iterator);
        return;
      }
    }
  }

  template< typename T >
  bool ForwardList< T >::operator==(const ForwardList< T > &instance)
  {
    if (this == &instance) {
      return true;
    }
    if (size_ != instance.size_) {
      return false;
    }
    for (auto it1 = cbegin(), it2 = instance.cbegin(); it1 != cend(); it1++, it2++) {
      if (*it1 != *it2) {
        return false;
      }
    }
    return true;
  }
}

#endif
