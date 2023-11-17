#include "Tokenizer.h"
#include "Tokenizer.h"
#include <iostream>
#include <cstring>

flusova::Token::Token(char* token) :
  token_(token),
  type_(NONE),
  numData_(0),
  charData_('a')
{
  if (isParenthesis()) {
    if (*token_ == '(') {
      type_ = RIGHT_PAREN;
    } else {
      type_ = LEFT_PAREN;
    }
    charData_ = *token_;
  }
  if (isOperand()) {
    type_ = OPERAND;
    numData_ = std::atoll(token_);
  }
  if (isOperator()) {
    type_ = getOperatorType();
    charData_ = *token_;
  }
}
bool flusova::Token::isOperand() const noexcept {
  if (type_ != NONE) {
    return (type_ == OPERAND);
  }
  std::size_t length = strlen(token_);
  for (size_t i = 0; i < length; i++) {
    if (!isdigit(token_[i])) {
      return false;
    }
  }
  return true;
}
bool flusova::Token::isOperator() noexcept {
  if (*token_ == '+' || *token_ == '-' || *token_ == '*' || *token_ == '%' || *token_ == '/') {
    return true;
  }
  return false;
}
bool flusova::Token::isParenthesis() const noexcept {
  if (type_ != NONE) {
    return (type_ == LEFT_PAREN || type_ == RIGHT_PAREN);
  }
  if (*token_ == '(' || *token_ == ')') {
    return true;
  }
  return false;
}
flusova::TokenType flusova::Token::getOperatorType() noexcept {
  switch (*token_) {
    case '+':
      return PLUS;
    case '-':
      return MINUS;
    case '*':
      return MULTIPLICATION;
    case '/':
      return DIV;
    case '%':
      return MOD;
    default :
      break;
  }
  return NONE;
}
flusova::TokenType flusova::Token::getTokenType() const noexcept {
  return type_;
}
char flusova::Token::getParenthesis() const noexcept {
  return charData_;
}
char flusova::Token::getOperator() noexcept {
  return charData_;
}
long long flusova::Token::getOperand() const noexcept {
  return numData_;
}
