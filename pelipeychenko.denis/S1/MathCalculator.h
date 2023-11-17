#ifndef MATHCALCULATOR_H
#define MATHCALCULATOR_H

#include <cstring>
#include "../common/Stack.h"
#include "../common/Queue.h"
#include "MathElement.h"

namespace pelipeychenko
{
  class MathCalculator
  {
  public:
    MathCalculator() = default;

    void inputExpressions(std::string in);

    void convertToPostfix();

    long long calculateResult();

    bool checkIfLessPriorityThenHead(const MathElement &symbol);

    void executeCalculator(std::istream &in);

  private:
    Queue< MathElement > expression_;
    Stack< MathElement > stack_;
    Queue< MathElement > postfix_;
  };
}

#endif
