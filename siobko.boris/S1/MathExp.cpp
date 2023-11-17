#include "MathExp.h"

#include <cmath>
#include <limits>
#include <stdexcept>
#include <iostream>

namespace siobko {
  bool isNumber(const std::string& s)
  {
    if (!std::isdigit(s[0])) {
      return false;
    }
    for (size_t i = 1u; i < s.size(); ++i) {
      if (!std::isdigit(s[i]) || s[i] == 0) {
        return false;
      }
    }
    return true;
  }

  MathExp::MathExp(const std::deque< std::string >& exp)
  {
    for (std::string expElem : exp) {
      if (isNumber(expElem)) {
        infixExp.push(ExpElem(stoll(expElem)));
      } else {
        infixExp.push(ExpElem(expElem[0]));
      }
    }
  }

  ExpElem MathExp::doOperation(long long a, long long b, const ExpElem& oper)
  {
    if (oper.getType() != ExpElemType::OPERATION) {
      throw std::invalid_argument("Error: invalid argument provided.");
    }

    ExpElem result(0LL);

    switch (oper.getOperation()) {
      case '+':
        if (std::numeric_limits<long long>::max() - a < b) {
          throw std::overflow_error("Error: Integer overflow.");
        }
        result = ExpElem(a + b);
        break;
      case '-':
        if (std::numeric_limits<long long>::max() - std::abs(a) < -std::abs(b)) {
          throw std::overflow_error("Error: Integer overflow.");
        }
        result = ExpElem(a - b);
        break;
      case '*':
        if (std::numeric_limits<long long>::max() / std::abs(a) < std::abs(b)) {
          throw std::overflow_error("Error: Integer overflow.");
        }
        result = ExpElem(a * b);
        break;
      case '/':
        if (b == 0) {
          throw std::invalid_argument("Error: division by zero.");
        }
        result = ExpElem(a / b);
        break;
      case '%':
        if (b == 0) {
          throw std::invalid_argument("Error: division by zero.");
        }
        result = ExpElem((a % b < 0) ? b + a % b : a % b);
        break;
    }

    return result;
  }

  void MathExp::convertToPostfix()
  {
    while (!infixExp.isEmpty()) {
      ExpElem elem = infixExp.front();
      infixExp.pop();

      switch (elem.getType()) {
        case ExpElemType::OPERAND:
          postfixExp.push(elem);
          break;
        case ExpElemType::OPERATION:
          while (!stackOfOperators.isEmpty() && stackOfOperators.front().getType() == ExpElemType::OPERATION) {
            if (ExpElem::isRightHasMorePriority(stackOfOperators.front(), elem)) {
              continue;
            }
            postfixExp.push(stackOfOperators.front());
            stackOfOperators.pop();
          }

          stackOfOperators.push(elem);
          break;
        case ExpElemType::BRACKETS:
          if (elem.getBracket() == '(') {
            stackOfOperators.push(elem);
            break;
          }

          while (!stackOfOperators.isEmpty()) {
            if (stackOfOperators.front().getType() == ExpElemType::BRACKETS && stackOfOperators.front().getBracket() == '(') {
              stackOfOperators.pop();
              break;
            }
            postfixExp.push(stackOfOperators.front());
            stackOfOperators.pop();
          }
          break;
      }
    }
    while (!stackOfOperators.isEmpty()) {
      postfixExp.push(stackOfOperators.front());
      stackOfOperators.pop();
    }
  }

  long long MathExp::calculate()
  {
    while (!postfixExp.isEmpty()) {
      ExpElem elem = postfixExp.front();
      postfixExp.pop();

      if (elem.getType() == ExpElemType::BRACKETS) {
        throw std::logic_error("ERROR: brackets is not expected.");
      }

      if (elem.getType() == ExpElemType::OPERAND) {
        stackOfOperands.push(elem);
        continue;
      }

      long long b = stackOfOperands.front().getOperand();
      stackOfOperands.pop();

      long long a = stackOfOperands.front().getOperand();
      stackOfOperands.pop();

      stackOfOperands.push(doOperation(a, b, elem));
    }
    return stackOfOperands.front().getOperand();
  }
}


