#ifndef POSTFIX_NOTATION_OF_MATH_EXPR_H
#define POSTFIX_NOTATION_OF_MATH_EXPR_H

#include <string>
#include <Queue.h>
#include <Stack.h>

namespace nemchinov
{
  class PostfixMathExpr
  {
  public:
    explicit PostfixMathExpr(const std::string &);
    ~PostfixMathExpr() = default;

    PostfixMathExpr(const PostfixMathExpr &) = delete;
    PostfixMathExpr &operator=(const PostfixMathExpr &) = delete;

    PostfixMathExpr(PostfixMathExpr &&) = delete;
    PostfixMathExpr &operator=(PostfixMathExpr &&) = delete;

    long long getResultOfMathExpr() const;

  private:
    long long resultOfMathExpr{};

    class MathItem;

    void doNumberHandling(Stack< const long long > &, PostfixMathExpr::MathItem &);
    void doOpenBracketHandling(Stack< MathItem > &, PostfixMathExpr::MathItem &);
    void doCloseBracketHandling(Stack< const long long > &, Stack< MathItem > &);
    void doOperationHanding(Stack< const long long > &, Stack< MathItem > &, PostfixMathExpr::MathItem &);
    void doOperation(Stack< const long long > &, Stack< MathItem > &);

    void convertToPostfix(const std::string &);

    long long getResOfOperation(char, long long, long long);
    long long returnSum(long long, long long);
    long long returnSubtract(long long, long long);
    long long returnMultiplication(long long, long long);
    long long returnDivision(long long, long long);
    long long returnRemainder(long long, long long);

    Queue< std::string > split(const std::string &);
  };

  class PostfixMathExpr::MathItem
  {
  public:
    enum Type
    {
      NUMBER,
      OPERATION,
      OPEN_BRACKET,
      CLOSE_BRACKET
    };

    enum Priority
    {
      LOW_PRIORITY,
      HIGH_PRIORITY,
      NO_PRIORITY
    };

    const int type_;
    const std::string data_;
    const int priority_;

    explicit MathItem(const std::string &);
    ~MathItem() = default;

    MathItem(const MathItem &) = default;
    MathItem &operator=(const MathItem &) = delete;

    MathItem(MathItem &&) = default;
    MathItem &operator=(MathItem &&) = delete;

    std::string getMathItem(const std::string &);
    int getTypeItem(const char);
    int getPriorityOfItem();
  };
}

#endif
