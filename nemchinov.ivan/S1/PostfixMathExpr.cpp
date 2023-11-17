#include <climits>
#include <Stack.h>

#include "PostfixMathExpr.h"

nemchinov::PostfixMathExpr::PostfixMathExpr(const std::string &infixMathExpr)
{
  PostfixMathExpr::convertToPostfix(infixMathExpr);
}

long long nemchinov::PostfixMathExpr::getResultOfMathExpr() const
{
  return resultOfMathExpr;
}

void nemchinov::PostfixMathExpr::convertToPostfix(const std::string &strInfixMathExpr)
{
  Queue< std::string > infixMathExpr = PostfixMathExpr::split(strInfixMathExpr);
  Stack< MathItem > bufferStack = Stack< MathItem >{};
  Stack< const long long > resultStack = Stack< const long long >{};

  while (!infixMathExpr.isEmpty())
  {
    PostfixMathExpr::MathItem item = PostfixMathExpr::MathItem(infixMathExpr.get());
    infixMathExpr.pop();

    switch (item.type_)
    {
      case MathItem::NUMBER:
        doNumberHandling(resultStack, item);
        break;

      case MathItem::OPEN_BRACKET:
        doOpenBracketHandling(bufferStack, item);
        break;

      case MathItem::CLOSE_BRACKET:
        doCloseBracketHandling(resultStack, bufferStack);
        break;

      case MathItem::OPERATION:
        doOperationHanding(resultStack, bufferStack, item);
        break;
    }
  }
  while (!bufferStack.isEmpty())
  {
    doOperation(resultStack, bufferStack);
  }

  resultOfMathExpr = resultStack.get();
  resultStack.pop();

  if (!resultStack.isEmpty())
  {
    throw std::runtime_error("ERROR : incorrect math expr");
  }
}

void nemchinov::PostfixMathExpr::doNumberHandling(Stack< const long long > &resultStack, MathItem &item)
{
  try
  {
    resultStack.push(stoll(item.data_));
  }
  catch (...)
  {
    throw std::overflow_error("ERROR : overflow");
  }
}

void nemchinov::PostfixMathExpr::doOpenBracketHandling(Stack< MathItem > &bufferStack, MathItem &item)
{
  bufferStack.push(item);
}

void nemchinov::PostfixMathExpr::doCloseBracketHandling(Stack< const long long > &resultStack,
                                                        Stack< MathItem > &bufferStack)
{
  while (bufferStack.get().type_ != MathItem::OPEN_BRACKET)
  {
    doOperation(resultStack, bufferStack);
  }
  bufferStack.pop();
}

void nemchinov::PostfixMathExpr::doOperationHanding(Stack< const long long > &resultStack,
                                                    Stack< MathItem > &bufferStack, MathItem &item)
{
  switch (item.priority_)
  {
    case MathItem::HIGH_PRIORITY:
      if (!bufferStack.isEmpty() && bufferStack.get().priority_ == MathItem::HIGH_PRIORITY)
      {
        doOperation(resultStack, bufferStack);
      }
      bufferStack.push(item);
      break;

    case MathItem::LOW_PRIORITY:
      while (!bufferStack.isEmpty() && bufferStack.get().type_ != MathItem::OPEN_BRACKET)
      {
        doOperation(resultStack, bufferStack);
      }
      bufferStack.push(item);
      break;
  }
}

void nemchinov::PostfixMathExpr::doOperation(Stack< const long long > &resultStack,
                                             Stack< MathItem > &bufferStack)
{
  long long secondOperand, firstOperand;
  try
  {
    secondOperand = resultStack.get();
    resultStack.pop();
    firstOperand = resultStack.get();
    resultStack.pop();
  }
  catch (...)
  {
    throw std::logic_error("ERROR : incorrect math expr");
  }

  resultStack.push(PostfixMathExpr::getResOfOperation(bufferStack.get().data_[0],
                                                      firstOperand, secondOperand));
  bufferStack.pop();

}

long long nemchinov::PostfixMathExpr::getResOfOperation(const char operation,
                                                        const long long firstOperand,
                                                        const long long secondOperand)
{

  switch (operation)
  {
    case '+':
      return returnSum(firstOperand, secondOperand);

    case '-':
      return returnSubtract(firstOperand, secondOperand);

    case '*':
      return returnMultiplication(firstOperand, secondOperand);

    case '/':
      return returnDivision(firstOperand, secondOperand);

    case '%':
      return returnRemainder(firstOperand, secondOperand);

    default:
      throw std::logic_error("ERROR : incorrect operator");
  }
}

long long nemchinov::PostfixMathExpr::returnSum(long long firstOperand, long long secondOperand)
{
  if (LLONG_MAX - firstOperand < secondOperand)
  {
    throw std::overflow_error("ERROR : overflow");
  }
  return firstOperand + secondOperand;
}

long long nemchinov::PostfixMathExpr::returnSubtract(long long firstOperand, long long secondOperand)
{
  if (LLONG_MIN + secondOperand > firstOperand)
  {
    throw std::overflow_error("ERROR : overflow");
  }
  return firstOperand - secondOperand;
}

long long nemchinov::PostfixMathExpr::returnMultiplication(long long firstOperand, long long secondOperand)
{
  if (secondOperand == 0)
  {
    throw std::overflow_error("ERROR : overflow");
  }
  if (firstOperand > 0 && secondOperand > 0 && LLONG_MAX / secondOperand < firstOperand)
  {
    throw std::overflow_error("ERROR : overflow");
  }
  if (firstOperand < 0 && secondOperand < 0 && LLONG_MAX / secondOperand > firstOperand)
  {
    throw std::overflow_error("ERROR : overflow");
  }
  if (firstOperand > 0 ?
      LLONG_MIN / firstOperand > secondOperand :
      LLONG_MIN / secondOperand > firstOperand)
  {
    throw std::overflow_error("ERROR : overflow");
  }
  return firstOperand * secondOperand;
}

long long nemchinov::PostfixMathExpr::returnDivision(long long firstOperand, long long secondOperand)
{
  if (secondOperand == 0)
  {
    throw std::logic_error("ERROR : division by zero");
  }
  return firstOperand / secondOperand;
}

long long nemchinov::PostfixMathExpr::returnRemainder(long long firstOperand, long long secondOperand)
{
  if (secondOperand == 0)
  {
    throw std::logic_error("ERROR : division by zero");
  }
  if (firstOperand % secondOperand < 0)
  {
    return secondOperand + firstOperand % secondOperand;
  }
  return firstOperand % secondOperand;
}

nemchinov::Queue< std::string > nemchinov::PostfixMathExpr::split(const std::string &str)
{
  Queue< std::string > qForResult = Queue< std::string >{};
  std::string buffer;
  for (char symbol: str)
  {
    if (symbol == ' ')
    {
      if (buffer.empty())
      {
        throw std::runtime_error("ERROR : incorrect math expr");
      }
      qForResult.push(buffer);
      buffer = "";
      continue;
    }
    buffer += symbol;
  }
  if (!buffer.empty())
  {
    qForResult.push(buffer);
  }
  return qForResult;
}

nemchinov::PostfixMathExpr::MathItem::MathItem(const std::string &data) :
        type_(PostfixMathExpr::MathItem::getTypeItem(data[0])),
        data_(PostfixMathExpr::MathItem::getMathItem(data)),
        priority_(PostfixMathExpr::MathItem::getPriorityOfItem())
{}

std::string nemchinov::PostfixMathExpr::MathItem::getMathItem(const std::string &data)
{
  std::string mathItem{data};

  switch (type_)
  {
    case OPERATION:
    case OPEN_BRACKET:
    case CLOSE_BRACKET:
      return mathItem;
    case NUMBER:
      for (const char symbol: data)
      {
        if (!isdigit(symbol))
        {
          throw std::logic_error("ERROR : not a math symbol");
        }
      }
      if (mathItem.length() > 1 && mathItem[0] == '0')
      {
        throw std::logic_error("ERROR : the number starts with a zero");
      }
      return mathItem;
  }
  throw std::logic_error("ERROR : not a math symbol");
}

int nemchinov::PostfixMathExpr::MathItem::getTypeItem(const char data)
{
  if (isdigit(data))
  {
    return NUMBER;
  }
  switch (data)
  {
    case '(':
      return OPEN_BRACKET;
    case ')':
      return CLOSE_BRACKET;
    case '+':
    case '-':
    case '*':
    case '/':
    case '%':
      return OPERATION;
    default:
      throw std::logic_error("ERROR : not a math symbol");
  }
}

int nemchinov::PostfixMathExpr::MathItem::getPriorityOfItem()
{
  switch (data_[0])
  {
    case '+':
    case '-':
      return LOW_PRIORITY;
    case '*':
    case '/':
    case '%':
      return HIGH_PRIORITY;
    default:
      return NO_PRIORITY;
  }
}

