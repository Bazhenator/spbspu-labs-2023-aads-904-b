#ifndef MATH_EXPRESSION_SOLVER_H
#define MATH_EXPRESSION_SOLVER_H

#include <Queue.h>
#include <Stack.h>

namespace ponomarev {
  struct ExpressionUnit {
    enum DataType {
      NUMBER,
      OPERATION,
      CLOSE_BRACKET,
      OPEN_BRACKET
    };

    ExpressionUnit() = delete;
    ExpressionUnit(const ExpressionUnit &obj);
    ExpressionUnit(ExpressionUnit &&) = delete;
    explicit ExpressionUnit(char *unit);
    ~ExpressionUnit() = default;

    ExpressionUnit &operator=(const ExpressionUnit &) = delete;
    ExpressionUnit &operator=(ExpressionUnit &&) = delete;
    bool operator<(const ExpressionUnit &rhs);

    DataType type;
    long long data;
  };
  class MathExprSolver {
    MathExprSolver() = default;

    void feedExpresion(char *string);
    long long getAnswer();

    Queue< ExpressionUnit > infixQueue;
    Queue< ExpressionUnit > postfixQueue;
    Stack< ExpressionUnit > sideStacker;
    Stack< long long > countingStacker;

    friend long long solveMathExpr(char *string);
  };

  long long solveMathExpr(char *string);
}

#endif
