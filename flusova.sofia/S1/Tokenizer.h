#ifndef TOKENIZER_H
#define TOKENIZER_H
#include <iostream>

namespace flusova{
  enum TokenType {
    PLUS,
    MINUS,
    MULTIPLICATION,
    MOD,
    DIV,
    LEFT_PAREN,
    RIGHT_PAREN,
    OPERAND,
    NONE
  };
  struct Token {
  public:
    Token(char* token);
    ~Token() = default;

    bool isOperand() const noexcept;
    bool isOperator() noexcept;
    bool isParenthesis() const noexcept;
    flusova::TokenType getTokenType() const noexcept;
    char getParenthesis() const noexcept;
    char getOperator() noexcept;
    long long getOperand() const noexcept;

  private:
    char* token_;
    flusova::TokenType type_;
    long long numData_;
    char charData_;
    flusova::TokenType getOperatorType() noexcept;
  };
}

#endif
