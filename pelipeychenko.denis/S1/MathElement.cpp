#include "MathElement.h"

#include <limits>
#include <iostream>

pelipeychenko::MathElement &pelipeychenko::MathElement::operator=(const pelipeychenko::MathElement &instance)
{
  if (this != &instance) {
    data_ = instance.data_;
  }
  return *this;
}

pelipeychenko::MathElement::MathElement(std::string str):
  data_(std::move(str))
{}

pelipeychenko::MathElement::MathElement(const long long number):
  data_(std::to_string(number))
{}

pelipeychenko::MathElement::MathElement(const char symbol)
{
  data_ += symbol;
}

pelipeychenko::MathType pelipeychenko::MathElement::getType() const
{
  if (data_.empty()) {
    return EMPTY;
  }
  if (data_ == "*" || data_ == "/" || data_ == "+" || data_ == "-" || data_ == "%") {
    return OPERATOR;
  }
  bool isDigit = true;
  if (!isdigit(data_[0]) && data_[0] != '-') {
    isDigit = false;
  } else {
    for (std::size_t i = 1; i < data_.size(); i++) {
      if (!isdigit(data_[i])) {
        isDigit = false;
        break;
      }
    }
  }

  if (isDigit) {
    return OPERAND;
  }
  if (data_ == "(") {
    return OPEN_BRACKET;
  }
  if (data_ == ")") {
    return CLOSE_BRACKET;
  }
  return EMPTY;
}

long long pelipeychenko::MathElement::getNumber() const
{
  if (getType() != OPERAND) {
    throw std::runtime_error("getNumber func got non-digit");
  }
  long long result = std::strtoll(data_.data(), nullptr, 0);
  if (errno != 0) {
    throw std::runtime_error("Underflow exception");
  }
  return result;
}

pelipeychenko::Operators pelipeychenko::MathElement::getOperator() const
{
  if (getType() != OPERATOR) {
    throw std::runtime_error("getOperator func got non-operator");
  }
  if (getData() == "+") {
    return ADDITION;
  } else if (getData() == "-") {
    return SUBTRACTION;
  } else if (getData() == "*") {
    return MULTIPLICATION;
  } else if (getData() == "/") {
    return DIVISION;
  }
  return MODULUS;
}

std::string pelipeychenko::MathElement::getData() const
{
  return data_;
}

long long pelipeychenko::MathElement::evaluate(const MathElement &first, const MathElement &second) const
{
  if (getType() != OPERATOR) {
    throw std::runtime_error("evaluate func got non-operator");
  }
  const long long MAX_NUMBER = std::numeric_limits< long long >::max();
  switch (getOperator()) {
    case ADDITION:
      if (first.getNumber() > MAX_NUMBER - second.getNumber()) {
        throw std::runtime_error("ADDITION Underflow exception");
      }
      return first.getNumber() + second.getNumber();
    case SUBTRACTION:
      if (first.getNumber() < -MAX_NUMBER - 1 + second.getNumber()) {
        throw std::runtime_error("SUBTRACTION Underflow exception");
      }
      return first.getNumber() - second.getNumber();
    case MULTIPLICATION:
      if (first.getNumber() > MAX_NUMBER / second.getNumber()
          || first.getNumber() < (-MAX_NUMBER - 1) / second.getNumber()) {
        throw std::runtime_error("MULTIPLICATION Underflow exception");
      }
      return first.getNumber() * second.getNumber();
    case DIVISION:
      if (second.getNumber() == 0) {
        throw std::runtime_error("Division by zero");
      }
      return first.getNumber() / second.getNumber();
    case MODULUS:
      if (second.getNumber() == 0) {
        throw std::runtime_error("Division by zero");
      }
      return (second.getNumber() + (first.getNumber() % second.getNumber())) % second.getNumber();
  }
  throw (std::runtime_error("something went wrong with evaluate"));
}

std::ostream &pelipeychenko::operator<<(std::ostream &os, const pelipeychenko::MathElement &instance)
{
  return os << instance.getData();
}
