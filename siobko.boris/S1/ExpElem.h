
#ifndef EXPELEM_H
#define EXPELEM_H

#include <cstdint>

namespace siobko {
  enum class ExpElemType {
    OPERAND,
    OPERATION,
    BRACKETS
  };

  class ExpElem {
  public:
    ExpElem() = default;
    explicit ExpElem(char c);
    explicit ExpElem(long long n);
    ~ExpElem() = default;

    ExpElemType getType() const noexcept;
    char getBracket() const;
    long long getOperand() const;
    char getOperation() const;
    static bool isRightHasMorePriority(const ExpElem& L, const ExpElem& R);

  private:
    ExpElemType type_;
    union {
      long long num_;
      char symbol_;
    };
  };
}
#endif
