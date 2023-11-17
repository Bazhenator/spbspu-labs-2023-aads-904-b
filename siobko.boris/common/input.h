
#ifndef INPUT_H
#define INPUT_H

#include <deque>
#include <string>
#include <iostream>
#include <fstream>

namespace siobko {
  std::deque< std::string > splitTextLine(const std::string& s, char delim);
  std::deque< std::string > inputTextLines(std::istream& in);
  std::deque< std::string > inputTextLine(std::istream& in);
  std::deque< std::string > inputTextLinesFromFile(std::ifstream& fin);
  std::deque< std::string > inputTextLineFromFile(std::ifstream& fin);
}
#endif
