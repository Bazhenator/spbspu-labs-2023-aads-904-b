#ifndef FOO_OBJECT_H
#define FOO_OBJECT_H

#include <iostream>
#include <string>
#include <limits>

namespace flusova {
  template < typename Key, typename Value, typename Comparator = std::less< Key > >
  struct SumKeysAndValues {

    SumKeysAndValues() : keysSum_(Key()), valuesSum_(Value()), result_("") {}

    void operator()(const Key& key, const Value& value);
    std::string& getSumString();

    Key keysSum_;
    Value valuesSum_;
    std::string result_;

  };
}

template < typename Key, typename Value, typename Comparator >
void flusova::SumKeysAndValues< Key, Value, Comparator >::operator()(const Key& key, const Value& value) {
  static const int MAX_INT = std::numeric_limits< int >::max();
  static const int MIN_INT = std::numeric_limits< int >::min();

  if (key > 0) {
    if (keysSum_ > MAX_INT - key) {
      throw std::runtime_error("Overflow.");
    }
  } else {
    if (keysSum_ < MIN_INT - key) {
      throw std::runtime_error("Underflow.");
    }
  }
  keysSum_ += key;
  valuesSum_ += value + ' ';
}

template < typename Key, typename Value, typename Comparator >
std::string& flusova::SumKeysAndValues< Key, Value, Comparator >::getSumString() {
  result_ = std::to_string(keysSum_) + " " + valuesSum_.substr(0, valuesSum_.length() - 1);
  return result_;
}

#endif
