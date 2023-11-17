#ifndef INPUT_DATA_H
#define INPUT_DATA_H
#include "Queue.h"
#include "Stack.h"
#include "Tokenizer.h"
#include <string>

namespace flusova {
  class manageExpression
  {
  public:
    void parseExpression(const std::string& input);
    short checkOperatorsPriority(flusova::Token token) const;
    void convertToPolish(const std::string& newString);
    void calculatePolish(flusova::Stack< long long >& stackResult);
    void printResult(flusova::Stack< long long >& stackResult);

  private:
    flusova::Queue< Token > newExpr_;
    flusova::Queue< Token > postfQueue_;
    flusova::Stack< Token > stack_;
  };
}

#endif
