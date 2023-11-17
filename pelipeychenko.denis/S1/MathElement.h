#ifndef MATHELEMENT_H
#define MATHELEMENT_H

#include <string>

namespace pelipeychenko
{
  enum MathType
  {
    OPERAND = 0,
    OPERATOR = 1,
    CLOSE_BRACKET = 2,
    OPEN_BRACKET = 3,
    EMPTY = 4,
  };

  enum Operators
  {
    ADDITION = 0,
    SUBTRACTION = 1,
    MULTIPLICATION = 2,
    DIVISION = 3,
    MODULUS = 4,
  };

  class MathElement
  {
  public:
    MathElement() = default;

    MathElement(const MathElement &instance) = default;

    ~MathElement() = default;

    MathElement &operator=(const MathElement &instance);

    explicit MathElement(std::string str);

    explicit MathElement(long long number);

    explicit MathElement(char symbol);

    MathType getType() const;

    long long getNumber() const;

    Operators getOperator() const;

    std::string getData() const;

    long long evaluate(const MathElement &first, const MathElement &second) const;

  private:
    std::string data_;
  };

  std::ostream &operator<<(std::ostream &os, const MathElement &instance);
}

#endif
