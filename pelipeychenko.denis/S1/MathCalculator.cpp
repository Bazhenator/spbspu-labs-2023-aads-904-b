#include "MathCalculator.h"

void pelipeychenko::MathCalculator::inputExpressions(std::string string)
{
  std::string element;
  char symbol;
  for (size_t i = 0; i < string.size(); i++) {
    symbol = string[i];
    if (symbol != ' ') {
      element += symbol;
    } else {
      MathElement temp(element);
      expression_.push(temp);
      element.clear();
    }
  }
  if (!element.empty()) {
    MathElement temp(element);
    expression_.push(temp);
  }
}

void pelipeychenko::MathCalculator::convertToPostfix()
{
  while (!expression_.isEmpty()) {
    MathElement symbol = expression_.getHead();
    expression_.drop();
    if (symbol.getType() == OPEN_BRACKET) {
      stack_.push(symbol);
    } else if (symbol.getType() == CLOSE_BRACKET) {
      MathElement lastElem = stack_.getHead();
      stack_.drop();
      while (lastElem.getType() != OPEN_BRACKET) {
        postfix_.push(lastElem);
        lastElem = stack_.getHead();
        stack_.drop();
      }
    } else if (symbol.getType() == OPERAND) {
      postfix_.push(symbol);
    } else if (symbol.getType() == OPERATOR) {
      while (!stack_.isEmpty() && stack_.getHead().getType() == OPERATOR && !checkIfLessPriorityThenHead(symbol)) {
        postfix_.push(stack_.getHead());
        stack_.drop();
      }
      stack_.push(symbol);
    }
  }
  while (!stack_.isEmpty()) {
    postfix_.push(stack_.getHead());
    stack_.drop();
  }
}

long long pelipeychenko::MathCalculator::calculateResult()
{
  Queue <MathElement> subQueue;
  MathElement mathElement, firstOperand, secondOperand;
  bool firstOperandIsDefined = false;
  bool secondOperandIsDefined = false;
  while (!(postfix_.getSize() == 1 && postfix_.getHead().getType() == OPERAND)) {
    mathElement = postfix_.getHead();
    postfix_.drop();
    if (mathElement.getType() == OPERAND && !firstOperandIsDefined) {
      firstOperand = mathElement;
      firstOperandIsDefined = true;
    } else if (mathElement.getType() == OPERAND && !secondOperandIsDefined) {
      secondOperand = mathElement;
      secondOperandIsDefined = true;
    } else if (mathElement.getType() == OPERAND) {
      subQueue.push(firstOperand);
      firstOperand = secondOperand;
      secondOperand = mathElement;
    } else if (mathElement.getType() == OPERATOR) {
      if (!(firstOperandIsDefined && secondOperandIsDefined)) {
        throw std::runtime_error("wrong expression");
      }

      long long number = mathElement.evaluate(firstOperand, secondOperand);
      subQueue.push(MathElement(number));

      while (!postfix_.isEmpty()) {
        subQueue.push(postfix_.getHead());
        postfix_.drop();
      }
      while (!subQueue.isEmpty()) {
        postfix_.push(subQueue.getHead());
        subQueue.drop();
      }
      firstOperandIsDefined = false;
      secondOperandIsDefined = false;
    }
  }
  long long result = postfix_.getHead().getNumber();
  postfix_.drop();
  return result;
}

bool pelipeychenko::MathCalculator::checkIfLessPriorityThenHead(const MathElement &symbol)
{
  bool res = (symbol.getOperator() <= 1 && stack_.getHead().getOperator() >= 2);
  return res;
}

void pelipeychenko::MathCalculator::executeCalculator(std::istream &in)
{
  pelipeychenko::Stack< long long > answers;
  std::string line;
  while (!in.eof() && std::getline(in, line)) {
    if (line.empty()) {
      continue;
    }
    inputExpressions(line);
    convertToPostfix();
    answers.push(calculateResult());
  }
  if (!answers.isEmpty()) {
    std::cout << answers.getHead();
    answers.drop();
  }
  while (!answers.isEmpty()) {
    std::cout << " " << answers.getHead();
    answers.drop();
  }
  std::cout << "\n";
}
