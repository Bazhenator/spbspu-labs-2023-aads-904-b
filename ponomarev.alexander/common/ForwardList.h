#ifndef FARWARD_LIST_H
#define FARWARD_LIST_H

#include <cstddef>
#include <cassert>
#include <algorithm>
#include <initializer_list>

namespace ponomarev {
  template< typename T >
  class ForwardList {
    public:
      class Iterator;
      class ConstIterator;

      ForwardList();
      ForwardList(const ForwardList< T > &rhs);
      ForwardList(ForwardList< T > &&rhs) noexcept;
      ForwardList(std::initializer_list< T > init);
      ~ForwardList();

      ForwardList &operator=(const ForwardList< T > &rhs);
      ForwardList &operator=(ForwardList< T > &&rhs) noexcept;

      Iterator insertAfter(const Iterator &pos, const T &value);
      Iterator eraseAfter(const Iterator &pos);
      void pushBack(const T &obj);
      void pushFront(const T &obj);
      void popBack();
      void popFront();

      bool isEmpty() const noexcept;
      std::size_t getSize() const noexcept;

      Iterator begin() noexcept;
      ConstIterator begin() const noexcept;
      ConstIterator cbegin() const noexcept;
      Iterator end() noexcept;
      ConstIterator end() const noexcept;
      ConstIterator cend() const noexcept;

      T &back();
      T &front();
      const T &back() const;
      const T &front() const;

      void swap(ForwardList< T > &rhs) noexcept;
    private:
      struct Node {
        explicit Node(const T &data_, Node *next_ = nullptr);
        ~Node() = default;
        T data;
        Node *next;
      };

      Node *head;
      std::size_t size;
  };

  template< typename T >
  class ForwardList< T >::ConstIterator {
    public:
      using iterator_category = std::forward_iterator_tag;
      using value_type        = T;
      using difference_type   = std::ptrdiff_t;
      using pointer           = const T*;
      using reference         = const T&;

      ConstIterator();
      ~ConstIterator() = default;

      ConstIterator &operator++();
      ConstIterator operator++(int);
      reference operator*() const;
      pointer operator->() const;

      bool operator==(const ConstIterator &rhs) const noexcept;
      bool operator!=(const ConstIterator &rhs) const noexcept;

      friend class ForwardList< T >;
    protected:
      typename ForwardList< T >::Node *node;
  };
  template< typename T >
  ForwardList< T >::ConstIterator::ConstIterator():
    node(nullptr)
  {}
  template< typename T >
  typename ForwardList< T >::ConstIterator &ForwardList< T >::ConstIterator::operator++()
  {
    assert(node != nullptr);
    node = node->next;
    return *this;
  }
  template< typename T >
  typename ForwardList< T >::ConstIterator ForwardList< T >::ConstIterator::operator++(int)
  {
    assert(node != nullptr);
    ConstIterator result = *this;
    ++(*this);
    return result;
  }
  template< typename T >
  typename ForwardList< T >::ConstIterator::reference ForwardList< T >::ConstIterator::operator*() const
  {
    assert(node != nullptr);
    return node->data;
  }
  template< typename T >
  typename ForwardList< T >::ConstIterator::pointer ForwardList< T >::ConstIterator::operator->() const
  {
    assert(node != nullptr);
    return std::pointer_traits< pointer >::pointer_to(**this);
  }
  template< typename T >
  bool ForwardList< T >::ConstIterator::operator==(const ConstIterator &rhs) const noexcept
  {
    return node == rhs.node;
  }
  template< typename T >
  bool ForwardList< T >::ConstIterator::operator!=(const ConstIterator &rhs) const noexcept
  {
    return !(*this == rhs);
  }
  template< typename T >
  class ForwardList< T >::Iterator: public ConstIterator {
    public:
      using iterator_category = std::forward_iterator_tag;
      using value_type        = T;
      using difference_type   = std::ptrdiff_t;
      using pointer           = T*;
      using reference         = T&;

      Iterator &operator++();
      Iterator operator++(int);
      reference operator*() const;
      pointer operator->() const;
  };
  template< typename T >
  typename ForwardList< T >::Iterator &ForwardList< T >::Iterator::operator++()
  {
    ConstIterator::operator++();
    return *this;
  }
  template< typename T >
  typename ForwardList< T >::Iterator ForwardList< T >::Iterator::operator++(int)
  {
    Iterator result = *this;
    ConstIterator::operator++();
    return result;
  }
  template< typename T >
  typename ForwardList< T >::Iterator::reference ForwardList< T >::Iterator::operator*() const
  {
    return const_cast< reference >(ConstIterator::operator*());
  }
  template< typename T >
  typename ForwardList< T >::Iterator::pointer ForwardList< T >::Iterator::operator->() const
  {
    return std::pointer_traits< pointer >::pointer_to(**this);
  }
}
template< typename T >
ponomarev::ForwardList< T >::ForwardList():
  head(nullptr),
  size(0)
{}
template< typename T >
ponomarev::ForwardList< T >::ForwardList(const ForwardList< T > &rhs):
  head(nullptr),
  size(0)
{
  for (ConstIterator ptr = rhs.begin(); ptr != rhs.end(); ptr++) {
    pushBack((*ptr));
  }
}
template< typename T >
ponomarev::ForwardList< T >::ForwardList(ForwardList< T > &&rhs) noexcept:
  head(rhs.head),
  size(rhs.size)
{
  rhs.head = nullptr;
  rhs.size = 0;
}
template< typename T >
ponomarev::ForwardList< T >::ForwardList(std::initializer_list< T > init):
  ForwardList()
{
  for (auto it = init.begin(); it != init.end(); it++) {
    pushBack(*it);
  }
}
template< typename T >
ponomarev::ForwardList< T > &ponomarev::ForwardList< T >::operator=(const ForwardList< T > &rhs)
{
  if (this != &rhs) {
    ForwardList< T > temp(rhs);
    swap(temp);
  }
  return *this;
}
template< typename T >
ponomarev::ForwardList< T > &ponomarev::ForwardList< T >::operator=(ForwardList< T > &&rhs) noexcept
{
  if (this != &rhs) {
    swap(rhs);
  }
  return *this;
}
template< typename T >
ponomarev::ForwardList< T >::~ForwardList()
{
  while (size) {
    popFront();
  }
}
template< typename T >
typename ponomarev::ForwardList< T >::Iterator ponomarev::ForwardList< T >::insertAfter(const Iterator &pos, const T &value)
{
  Iterator newNode;
  newNode.node = new Node(value);
  Iterator postIter = pos;
  postIter++;
  (newNode.node)->next = postIter.node;
  (pos.node)->next = newNode.node;
  size++;
  return newNode;
}
template< typename T >
typename ponomarev::ForwardList< T >::Iterator ponomarev::ForwardList< T >::eraseAfter(const Iterator &pos)
{
  Iterator toErase = pos;
  toErase++;
  Iterator postIter = toErase;
  postIter++;
  pos.node->next = (postIter == end() ? nullptr : postIter.node);
  delete toErase.node;
  size--;
  return postIter;
}
template< typename T >
void ponomarev::ForwardList< T >::pushBack(const T &obj)
{
  if (head) {
    Node *tail = head;
    while (tail->next) {
      tail = tail->next;
    }
    tail->next = new Node(obj, nullptr);
  } else {
    head = new Node(obj, nullptr);
  }
  size++;
}
template< typename T >
void ponomarev::ForwardList< T >::pushFront(const T &obj)
{
  Node *temp = new Node(obj, nullptr);
  temp->next = head;
  head = temp;
  size++;
}
template< typename T >
void ponomarev::ForwardList< T >::popBack()
{
  Node *toDelete = head;
  while (toDelete->next) {
    if (!toDelete->next->next) {
      Node *newTail = toDelete;
      toDelete = toDelete->next;
      newTail->next = nullptr;
      break;
    }
    toDelete = toDelete->next;
  }
  delete toDelete;
  toDelete = nullptr;
  size--;
}
template< typename T >
void ponomarev::ForwardList< T >::popFront()
{
  Node *newHead = head->next;
  delete head;
  head = newHead;
  size--;
}
template< typename T >
bool ponomarev::ForwardList< T >::isEmpty() const noexcept
{
  return !size;
}
template< typename T >
typename ponomarev::ForwardList< T >::Iterator ponomarev::ForwardList< T >::begin() noexcept
{
  Iterator begin;
  begin.node = head;
  return begin;
}
template< typename T >
typename ponomarev::ForwardList< T >::ConstIterator ponomarev::ForwardList< T >::begin() const noexcept
{
  ConstIterator begin;
  begin.node = head;
  return begin;
}
template< typename T >
typename ponomarev::ForwardList< T >::ConstIterator ponomarev::ForwardList< T >::cbegin() const noexcept
{
  return begin();
}
template< typename T >
typename ponomarev::ForwardList< T >::Iterator ponomarev::ForwardList< T >::end() noexcept
{
  Iterator end;
  end.node = head;
  Iterator null;
  while (end != null) {
    end++;
  }
  return end;
}
template< typename T >
typename ponomarev::ForwardList< T >::ConstIterator ponomarev::ForwardList< T >::end() const noexcept
{
  ConstIterator end;
  end.node = head;
  ConstIterator null;
  while (end != null) {
    end++;
  }
  return end;
}
template< typename T >
typename ponomarev::ForwardList< T >::ConstIterator ponomarev::ForwardList< T >::cend() const noexcept
{
  return end();
}
template< typename T >
T &ponomarev::ForwardList< T >::back()
{
  return const_cast< T & >(static_cast< const ForwardList & >(*this).back());
}
template< typename T >
T &ponomarev::ForwardList< T >::front()
{
  return head->data;
}
template< typename T >
const T &ponomarev::ForwardList< T >::back() const
{
  Node *tail = head;
  while (tail->next) {
    tail = tail->next;
  }
  return tail->data;
}
template< typename T >
const T &ponomarev::ForwardList< T >::front() const
{
  return head->data;
}
template< typename T >
std::size_t ponomarev::ForwardList< T >::getSize() const noexcept
{
  return size;
}
template< typename T >
void ponomarev::ForwardList< T >::swap(ForwardList< T > &rhs) noexcept
{
  std::swap(head, rhs.head);
  std::swap(size, rhs.size);
}

template< typename T >
ponomarev::ForwardList< T >::Node::Node(const T &data_, Node *next_):
  data(data_),
  next(next_)
{}

#endif
