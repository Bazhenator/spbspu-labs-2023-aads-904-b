#include "CMDmap.h"
#include <iostream>
#include <vector>
#include "outputLiterals.h"

void flusova::CMDmap::inputData(std::istream& in) {
  std::string line;
  std::getline(in, line);
  line_ = split(line, ' ');
  if (line_.size() % 2 == 0) {
    for (std::size_t i = 0; i < line_.size(); i += 2) {
      dict_.insert(std::stoi(line_[i]), line_[i + 1]);
    }
  }
}

std::vector< std::string > flusova::CMDmap::split(std::string& str, const char d) {
  std::vector< std::string > splitted;
  std::size_t start;
  while ((start = str.find(d)) != std::string::npos) {
    if (!str.substr(0,start).empty()) {
      splitted.push_back(str.substr(0, start));
    }
    str.erase(0, start + 1);
  }
  splitted.push_back(str.substr(0, start));
  return splitted;
}

void flusova::CMDmap::doTraverse() {
  if (dict_.isEmpty()) {
    flusova::printEmpty(std::cout);
    return;
  }
  auto it = cmd_.find(command_);
  if (it != cmd_.end()) {
    try {
      it->second();
      std::cout << f_.getSumString() << '\n';
    } catch (const std::runtime_error &e) {
      throw;
    }
  } else {
    flusova::printInvalidCmd(std::cout);
    return;
  }
}
