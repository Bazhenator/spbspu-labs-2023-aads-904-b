#ifndef LIST_H
#define LIST_H

#include <iostream>
#include <cstddef>

namespace pelipeychenko
{
  template< typename T >
  class List
  {
  public:
    List();

    ~List();

    void pushBack(const T &instance);

    void pushFront(const T &instance);

    void dropFront();

    void dropBack();

    bool isEmpty() const noexcept;

    std::size_t getSize() const noexcept;

    const T &getHead() const;

    const T &getTail() const;

  private:
    struct Node
    {
      explicit Node(const T &data):
        data_(data),
        next_(nullptr),
        prev_(nullptr)
      {}

      T data_;
      Node *next_, *prev_;
    };

    Node *head_, *tail_;
    std::size_t size_;
  };

  template< typename T >
  List< T >::List():
    head_(nullptr),
    tail_(nullptr),
    size_(0)
  {}

  template< typename T >
  List< T >::~List()
  {
    while (!isEmpty()) {
      dropBack();
    }
  }

  template< typename T >
  void List< T >::pushBack(const T &instance)
  {
    Node *temp = new Node(instance);
    if (!isEmpty()) {
      temp->prev_ = tail_;
      tail_->next_ = temp;
      tail_ = temp;
    } else {
      head_ = temp;
      tail_ = temp;
    }
    size_++;
  }

  template< typename T >
  void List< T >::pushFront(const T &instance)
  {
    Node *temp = new Node(instance);
    if (!isEmpty()) {
      temp->next_ = head_;
      head_->prev_ = temp;
      head_ = temp;
    } else {
      head_ = temp;
      tail_ = temp;
    }
    size_++;
  }

  template< typename T >
  void List< T >::dropFront()
  {
    if (size_ != 1) {
      head_ = head_->next_;
      delete head_->prev_;
    } else {
      head_->next_ = nullptr;
      delete head_;
    }
    size_--;
  }

  template< typename T >
  void List< T >::dropBack()
  {
    if (size_ != 1) {
      tail_ = tail_->prev_;
      delete tail_->next_;
    } else {
      tail_->prev_ = nullptr;
      delete tail_;
    }
    size_--;
  }

  template< typename T >
  bool List< T >::isEmpty() const noexcept
  {
    return (size_ == 0);
  }

  template< typename T >
  std::size_t List< T >::getSize() const noexcept
  {
    return size_;
  }

  template< typename T >
  const T &List< T >::getHead() const
  {
    return head_->data_;
  }

  template< typename T >
  const T &List< T >::getTail() const
  {
    return tail_->data_;
  }
}

#endif
