#include "ExpElem.h"

#include <iostream>

namespace siobko {
  ExpElem::ExpElem(char c)
  {
    if (c == '(' || c == ')') {
      type_ = siobko::ExpElemType::BRACKETS;
      symbol_ = c;
    } else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%') {
      type_ = siobko::ExpElemType::OPERATION;
      symbol_ = c;
    } else {
      throw std::domain_error("ERROR: invalid expression value.");
    }
  }

  ExpElem::ExpElem(long long n):
    type_(siobko::ExpElemType::OPERAND),
    num_(n)
  {}

  ExpElemType ExpElem::getType() const noexcept
  {
    return type_;
  }

  char ExpElem::getBracket() const
  {
    if (type_ != ExpElemType::BRACKETS) {
      throw std::invalid_argument("ERROR: cannot get bracket.");
    }
    return symbol_;
  }

  long long ExpElem::getOperand() const
  {
    if (type_ != ExpElemType::OPERAND) {
      throw std::invalid_argument("ERROR: cannot get operand.");
    }
    return num_;
  }

  char ExpElem::getOperation() const
  {
    if (type_ != ExpElemType::OPERATION) {
      throw std::invalid_argument("ERROR: cannot get operation.");
    }
    return symbol_;
  }

  bool ExpElem::isRightHasMorePriority(const ExpElem& L, const ExpElem& R)
  {
    if (L.type_ != ExpElemType::OPERATION || R.type_ != ExpElemType::OPERATION) {
      throw std::invalid_argument("ERROR: func must take operations.");
    }
    return (L.symbol_ == '+' || L.symbol_ == '-') && (L.symbol_ == '/' || L.symbol_ == '*' || L.symbol_ == '%');
  }
}


