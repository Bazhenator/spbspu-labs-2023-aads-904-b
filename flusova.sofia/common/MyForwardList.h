#ifndef MY_FORWARD_LIST
#define MY_FORWARD_LIST

#include <cstdlib>
#include <iostream>
#include <cassert>

namespace flusova {
  template< typename T >
  class MyForwardList {
  public:

    class MyConstIterator;
    class MyIterator;

    MyForwardList();
    MyForwardList(const MyForwardList< T >& rhs);
    MyForwardList(const std::initializer_list< T >& list);
    MyForwardList(MyForwardList< T >&&) noexcept;
    ~MyForwardList();
    MyForwardList< T >& operator=(const MyForwardList& rhs);
    MyForwardList< T >& operator=(MyForwardList< T >&&) noexcept;
    void pushBack(const T& toPush);
    void pushFront(const T& toPush);
    void popBack();
    void popFront();
    void swap(flusova::MyForwardList< T >& toSwap) noexcept;
    void clear() noexcept;
    bool isEmpty() const noexcept;
    std::size_t getSize() const noexcept;
    const T& getTop() const;
    const T& getBack() const;
    T& getTop();
    T& getBack();
    MyIterator eraseAfter(const MyIterator& prev);
    MyIterator insertAfter(const MyIterator& prev, const T& data);
    MyIterator begin() noexcept;
    MyIterator end() noexcept;
    MyConstIterator cbegin() const noexcept;
    MyConstIterator cend() const noexcept;
    MyConstIterator begin() const noexcept;
    MyConstIterator end() const noexcept;

  private:

    struct Node {
      explicit Node(const T& data, Node* next = nullptr) : data_(data), next_(next) {};
      ~Node() = default;
      T data_;
      Node* next_;
    };

    Node* head_;
    std::size_t size_;

  };
}

template< typename T >
class flusova::MyForwardList< T >::MyConstIterator {
public:

  MyConstIterator() : traversable_(nullptr) {}
  explicit MyConstIterator(typename MyForwardList< T >::Node* head) : traversable_(head) {}
  ~MyConstIterator() = default;
  MyConstIterator& operator++();
  MyConstIterator operator++(int);
  const T& operator*() const;
  const T* operator->() const;
  bool operator==(const MyConstIterator& equal) const;
  bool operator!=(const MyConstIterator& equal) const;

  friend class MyForwardList< T >;

private:

  typename MyForwardList< T >::Node* traversable_;

};

template< typename T >
typename flusova::MyForwardList< T >::MyConstIterator& flusova::MyForwardList< T >::MyConstIterator::operator++() {
  assert(traversable_ != nullptr);
  traversable_ = traversable_->next_;
  return *this;
}

template< typename T >
typename flusova::MyForwardList< T >::MyConstIterator flusova::MyForwardList< T >::MyConstIterator::operator++(int) {
  assert(traversable_ != nullptr);
  MyConstIterator it(traversable_);
  traversable_ = traversable_->next_;
  return it;
}

template< typename T >
const T& flusova::MyForwardList< T >::MyConstIterator::operator*() const {
  assert(traversable_ != nullptr);
  return traversable_->data_;
}

template< typename T >
const T* flusova::MyForwardList< T >::MyConstIterator::operator->() const {
  assert(traversable_ != nullptr);
  return &(traversable_->data_);
}

template< typename T >
bool flusova::MyForwardList< T >::MyConstIterator::operator==(const MyConstIterator& equal) const {
  return traversable_ == equal.traversable_;
}

template< typename T >
bool flusova::MyForwardList< T >::MyConstIterator::operator!=(const MyConstIterator& equal) const {
  return !(*this == equal);
}

template< typename T >
class flusova::MyForwardList< T >::MyIterator : public MyConstIterator {
public:
  MyIterator& operator++();
  MyIterator operator++(int);
  T& operator*() const;
  T* operator->() const;
};

template< typename T >
typename flusova::MyForwardList< T >::MyIterator& flusova::MyForwardList< T >::MyIterator::operator++() {
  MyConstIterator::operator++();
  return *this;
}
template< typename T >

typename flusova::MyForwardList< T >::MyIterator flusova::MyForwardList< T >::MyIterator::operator++(int) {
  MyIterator it = *this;
  MyConstIterator::operator++();
  return it;
}

template< typename T >
T& flusova::MyForwardList< T >::MyIterator::operator*() const {
  return const_cast< T& >(MyConstIterator::operator*());
}

template< typename T >
T* flusova::MyForwardList< T >::MyIterator::operator->() const {
  return const_cast< T* >(MyConstIterator::operator->());
}

template< typename T >
flusova::MyForwardList< T >::MyForwardList() :
        head_(nullptr),
        size_(0)
{}

template< typename T >
flusova::MyForwardList< T >::MyForwardList(const MyForwardList& other) : head_(nullptr), size_(0) {
  Node* current = other.head_;
  while (current) {
    pushBack(current->data_);
    current = current->next_;
  }
}

template< typename T >
flusova::MyForwardList< T >::MyForwardList(const std::initializer_list< T >& list) : MyForwardList() {
  for (const T& val: list) {
    this->pushFront(val);
  }
}

template< typename T >
flusova::MyForwardList< T >::~MyForwardList() {
  while (size_) {
    popFront();
  }
}

template< typename T >
flusova::MyForwardList< T >& flusova::MyForwardList< T >::operator=(const MyForwardList< T >& obj) {
  if (this != &obj) {
    MyForwardList< T > temp(obj);
    swap(temp);
  }
  return *this;
}

template< typename T >
flusova::MyForwardList< T >& flusova::MyForwardList< T >::operator=(MyForwardList< T >&& obj) noexcept {
  if (this != &obj) {
    swap(obj);
  }
  return *this;
}

template< typename T >
void flusova::MyForwardList< T >::pushBack(const T& toPush) {
  if (head_ == nullptr) {
    head_ = new Node(toPush);
  } else {
    Node* current = this->head_;
    while (current->next_ != nullptr) {
      current = current->next_;
    }
    current->next_ = new Node(toPush);
  }
  size_++;
}

template< typename T >
void flusova::MyForwardList< T >::pushFront(const T& toPush) {
  head_ = new Node(toPush, head_);
  ++size_;
}

template< typename T >
void flusova::MyForwardList< T >::popBack() {
  if (head_->next_) {
    Node* newTail = head_;
    for (std::size_t i = 0; i < (size_ - 2); ++i) {
      newTail = newTail->next_;
    }
    Node* oldTail = newTail->next_;
    delete oldTail;
    newTail->next_ = nullptr;
  } else {
    delete head_;
    head_ = nullptr;
  }
  --size_;
}

template< typename T >
void flusova::MyForwardList< T >::popFront() {
  Node* temp = head_->next_;
  delete head_;
  head_ = temp;
  --size_;
}

template< typename T >
void flusova::MyForwardList< T >::swap(MyForwardList< T >& toSwap) noexcept {
  std::swap(this->head_, toSwap.head_);
  std::swap(this->size_, toSwap.size_);
}

template< typename T >
void flusova::MyForwardList< T >::clear() noexcept {
  while (head_) {
    Node* temp = head_;
    head_ = head_->next_;
    delete temp;
  }
  size_ = 0;
}

template< typename T >
bool flusova::MyForwardList< T >::isEmpty() const noexcept {
  return size_ == 0;
}

template< typename T >
std::size_t flusova::MyForwardList< T >::getSize() const noexcept {
  return size_;
}

template< typename T >
T& flusova::MyForwardList< T >::getTop() {
  return head_->data_;
}

template< typename T >
T& flusova::MyForwardList< T >::getBack() {
  const MyForwardList< T >& constList = static_cast<const MyForwardList&>(*this);
  const T& constElement = constList.getBack();
  T& element = const_cast<T&>(constElement);
  return element;
}

template< typename T >
const T& flusova::MyForwardList< T >::getTop() const {
  return head_->data_;
}

template< typename T >
const T& flusova::MyForwardList< T >::getBack() const {
  Node* curEl = head_;
  for (std::size_t i = 0; i < flusova::MyForwardList< T >::getSize() - 1; ++i) {
    curEl = curEl->next_;
  }
  return curEl->data_;
}

template< typename T >
typename flusova::MyForwardList< T >::MyIterator flusova::MyForwardList< T >::begin() noexcept {
  MyIterator it;
  it.traversable_ = head_;
  return it;
}

template< typename T >
typename flusova::MyForwardList< T >::MyIterator flusova::MyForwardList< T >::end() noexcept {
  MyIterator it;
  it.traversable_ = head_;
  while (it.traversable_ != nullptr) {
    it++;
  }
  return it;
}

template< typename T >
typename flusova::MyForwardList< T >::MyConstIterator flusova::MyForwardList< T >::cbegin() const noexcept {
  return begin();
}

template< typename T >
typename flusova::MyForwardList< T >::MyConstIterator flusova::MyForwardList< T >::cend() const noexcept {
  return end();
}

template< typename T >
typename flusova::MyForwardList< T >::MyConstIterator flusova::MyForwardList< T >::begin() const noexcept {
  MyConstIterator it;
  it.traversable_ = head_;
  return it;
}

template< typename T >
typename flusova::MyForwardList< T >::MyConstIterator flusova::MyForwardList< T >::end() const noexcept {
  MyConstIterator it;
  it.traversable_ = head_;
  while (it.traversable_ != nullptr) {
    it++;
  }
  return it;
}

template< typename T >
typename flusova::MyForwardList< T >::MyIterator
flusova::MyForwardList< T >::insertAfter(const MyIterator& prev, const T& data) {
  MyIterator newIt;
  newIt.traversable_ = new Node(data);
  MyIterator prevIt = prev;
  prevIt++;
  (newIt.traversable_)->next_ = prevIt.traversable_;
  (prev.traversable_)->next_ = newIt.traversable_;
  size_++;
  return newIt;
}

template< typename T >
typename flusova::MyForwardList< T >::MyIterator flusova::MyForwardList< T >::eraseAfter(const MyIterator& prev) {
  MyIterator prevIt = prev;
  MyIterator temp;
  if (prevIt.traversable_ == nullptr || prevIt.traversable_->next_ == nullptr) {
    return temp;
  }
  Node* toDelete = prevIt.traversable_->next_;
  prevIt.traversable_->next_ = toDelete->next_;
  MyIterator it;
  it.traversable_ = toDelete->next_;
  delete toDelete;
  size_--;
  return it;
}

#endif
