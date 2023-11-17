#include "MathExpressionSolver.h"
#include <cctype>
#include <cstring>
#include <cstdlib>
#include <stdexcept>
#include <limits>

ponomarev::ExpressionUnit::ExpressionUnit(const ExpressionUnit &obj):
  type(obj.type),
  data(obj.data)
{}
ponomarev::ExpressionUnit::ExpressionUnit(char *unit)
{
  bool isNumber = true;
  for (std::size_t i = 0; i < strlen(unit); i++) {
    if (!isdigit(unit[i])) {
      isNumber = false;
      break;
    }
  }
  if (isNumber) {
    data = std::strtoll(unit, nullptr, 10);
    if (errno == ERANGE) {
      throw std::runtime_error("Overflow");
    }
    type = NUMBER;
  } else if (')' == *unit) {
    type = CLOSE_BRACKET;
    data = *unit;
  } else if ('(' == *unit) {
    type = OPEN_BRACKET;
    data = *unit;
  } else if (strchr("+-*/%", *unit)) {
    type = OPERATION;
    data = *unit;
  } else {
    throw std::runtime_error("Expression is not correct.");
  }
}
bool ponomarev::ExpressionUnit::operator<(const ExpressionUnit &rhs)
{
  if (type == OPERATION) {
    return strchr("+-", data) && strchr("*/%", rhs.data);
  }
  return data < rhs.data;
}

void ponomarev::MathExprSolver::feedExpresion(char *string)
{
  char *token = std::strtok(string, " ");
  while (token) {
    try {
      infixQueue.push(ExpressionUnit(token));
    } catch (const std::runtime_error &e) {
      throw;
    }
    token = std::strtok(nullptr, " ");
  }

  while (!infixQueue.isEmpty()) {
    ExpressionUnit symbol = infixQueue.getNext();
    switch (symbol.type) {
    case ExpressionUnit::NUMBER:
      postfixQueue.push(infixQueue.getNext());
      infixQueue.pop();
      break;
    case ExpressionUnit::OPERATION:
      if (!sideStacker.isEmpty() && (sideStacker.getTop().type == ExpressionUnit::OPERATION) && !(sideStacker.getTop() < symbol)) {
        postfixQueue.push(sideStacker.getTop());
        sideStacker.pop();
      }
      sideStacker.push(infixQueue.getNext());
      infixQueue.pop();
      break;
    case ExpressionUnit::OPEN_BRACKET:
      sideStacker.push(infixQueue.getNext());
      infixQueue.pop();
      break;
    case ExpressionUnit::CLOSE_BRACKET:
      while (sideStacker.getTop().type != ExpressionUnit::OPEN_BRACKET) {
        postfixQueue.push(sideStacker.getTop());
        sideStacker.pop();
      }
      sideStacker.pop();
      infixQueue.pop();
      break;
    }
  }
  while (!sideStacker.isEmpty()) {
    if (sideStacker.getTop().type == ExpressionUnit::OPEN_BRACKET) {
      throw std::runtime_error("Expression is not correct.");
    }
    postfixQueue.push(sideStacker.getTop());
    sideStacker.pop();
  }
}
long long ponomarev::MathExprSolver::getAnswer()
{
  const long long MAX_LONG = std::numeric_limits< long long >::max();
  while (!postfixQueue.isEmpty()) {
    ExpressionUnit symbol = postfixQueue.getNext();
    if (symbol.type != ExpressionUnit::NUMBER) {
      long long newgetTop = countingStacker.getTop();
      countingStacker.pop();
      switch (symbol.data) {
      case '+':
        if (newgetTop > MAX_LONG - countingStacker.getTop()) {
          throw std::runtime_error("Overflow");
        }
        newgetTop += countingStacker.getTop();
        break;
      case '-':
        if (newgetTop < -MAX_LONG - 1 + countingStacker.getTop()) {
          throw std::runtime_error("Underflow");
        }
        newgetTop = countingStacker.getTop() - newgetTop;
        break;
      case '*':
        if (newgetTop > MAX_LONG / countingStacker.getTop() || newgetTop < (-MAX_LONG - 1) / countingStacker.getTop()) {
          throw std::runtime_error("Under/Overflow");
        }
        newgetTop *= countingStacker.getTop();
        break;
      case '/':
        newgetTop = countingStacker.getTop() / newgetTop;
        break;
      case '%':
        long long temp = newgetTop;
        newgetTop = countingStacker.getTop() % newgetTop;
        if (newgetTop < 0) {
          newgetTop += temp;
        }
        break;
      }
      countingStacker.pop();
      countingStacker.push(newgetTop);
    } else {
      countingStacker.push(symbol.data);
    }
    postfixQueue.pop();
  }
  return countingStacker.getTop();
}

long long ponomarev::solveMathExpr(char *string)
{
  MathExprSolver solution;
  try {
    solution.feedExpresion(string);
  } catch (const std::runtime_error &e) {
    throw;
  }
  return solution.getAnswer();
}
