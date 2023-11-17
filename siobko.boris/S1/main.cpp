#include <iostream>
#include <algorithm>

#include "input.h"
#include "MathExp.h"

int main(int argc, char* argv[])
{
  std::deque< std::string > expressions;
  std::deque< long long > answers;

  expressions = siobko::inputTextLines(std::cin);

  if (argc == 2) {
    const char* filename = argv[1];
    std::ifstream fin(filename);
    expressions = siobko::inputTextLinesFromFile(fin);
  }

  for (const std::string& expression : expressions) {
    if (expression.empty()) {
      continue;
    }

    try {
      siobko::MathExp exp(siobko::splitTextLine(expression, ' '));
      exp.convertToPostfix();
      answers.push_back(exp.calculate());
    } catch (const std::exception& err) {
      std::cerr << err.what() << '\n';
      return 1;
    }
  }

  reverse(answers.begin(), answers.end());
  if (!answers.empty()) {
    for (size_t i = 0; i < answers.size() - 1; ++i) {
      std::cout << answers[i] << " ";
    }
    std::cout << answers[answers.size() - 1];
  }
  std::cout << '\n';
}
