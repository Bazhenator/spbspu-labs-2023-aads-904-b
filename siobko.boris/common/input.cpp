#include "input.h"

#include <sstream>

namespace siobko {
  std::deque< std::string > splitTextLine(const std::string& s, char delim)
  {
    std::deque< std::string > result;
    std::stringstream ss(s);
    std::string item;

    while (getline(ss, item, delim)) {
      result.push_back(item);
    }

    return result;
  }

  std::deque< std::string > inputTextLines(std::istream& in)
  {
    std::deque< std::string > textLines;
    std::string textLine;

    while (!in.eof()) {
      getline(in, textLine);
      textLines.push_back(textLine);
    }

    return textLines;
  }

  std::deque< std::string > inputTextLine(std::istream& in)
  {
    std::string textLine;
    getline(in, textLine);
    return splitTextLine(textLine, ' ');
  }

  std::deque< std::string > inputTextLinesFromFile(std::ifstream& fin)
  {
    std::deque< std::string > textLines;
    std::string textLine;

    while (!fin.eof()) {
      getline(fin, textLine);
      textLines.push_back(textLine);
    }

    return textLines;
  }

  std::deque< std::string > inputTextLineFromFile(std::ifstream& fin)
  {
    std::string textLine;
    getline(fin, textLine);
    return splitTextLine(textLine, ' ');
  }
}
