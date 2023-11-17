
#ifndef MATHEXP_H
#define MATHEXP_H

#include <deque>
#include <string>

#include "Queue.h"
#include "Stack.h"
#include "ExpElem.h"

namespace siobko {
  bool isNumber(const std::string& s);

  class MathExp {
  public:
    explicit MathExp(const std::deque< std::string >& exp);
    ~MathExp() = default;

    ExpElem doOperation(long long a, long long b, const ExpElem& oper);
    void convertToPostfix();
    long long calculate();

  private:
    Queue< ExpElem > infixExp;
    Queue< ExpElem > postfixExp;
    Stack< ExpElem > stackOfOperands;
    Stack< ExpElem > stackOfOperators;
  };
}
#endif
