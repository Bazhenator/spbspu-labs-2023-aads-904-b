#ifndef PAIR_H
#define PAIR_H

#include <iostream>

namespace pelipeychenko
{
  template< typename first, typename second >
  struct pair
  {
    first first_;
    second second_;

    pair(const pair &rhs):
      first_(rhs.first_),
      second_(rhs.second_)
    {}

    pair(pair &&rhs) noexcept :
      first_(std::move(rhs.first_)),
      second_(std::move(rhs.second_))
    {}

    pair(const first &a, const second &b):
      first_(a),
      second_(b)
    {}

    void swap(pair< first, second > &rhs) noexcept
    {
      std::swap(first_, rhs.first_);
      std::swap(second_, rhs.second_);
    }

    const first &getFirst() const;

    const second &getSecond() const;

    pair< first, second > &operator=(const pair< first, second > &right)
    {
      first_ = right.first_;
      second_ = right.second_;
      return *this;
    }

    pair< first, second > &operator=(pair< first, second > &&right) noexcept
    {
      swap(right);
      return *this;
    }

    bool operator==(const pair< first, second > &rhs) const;

    bool operator!=(const pair< first, second > &rhs) const;

    ~pair() = default;
  };

  template< typename first, typename second >
  bool pair< first, second >::operator==(const pair< first, second > &rhs) const
  {
    return getFirst() == rhs.getFirst() && getSecond() == rhs.getSecond();
  }

  template< typename first, typename second >
  bool pair< first, second >::operator!=(const pair< first, second > &rhs) const
  {
    return getFirst() != rhs.getFirst() || getSecond() != rhs.getSecond();
  }

  template< typename first, typename second >
  const first &pair< first, second >::getFirst() const
  {
    return first_;
  }

  template< typename first, typename second >
  const second &pair< first, second >::getSecond() const
  {
    return second_;
  }

  template< typename first, typename second >
  std::ostream &operator<<(std::ostream &os, const pair< first, second > &instance)
  {
    return os << instance.first_ << " " << instance.second_;
  }
}

#endif
