#include "CommandHandler.h"

std::vector< std::string > ponomarev::split(std::string line, const char separator)
{
  std::size_t pos;
  std::vector< std::string > tokens;
  while ((pos = line.find(separator)) != std::string::npos) {
    if (!line.substr(0, pos).empty()) {
      tokens.push_back(line.substr(0, pos));
    }
    line.erase(0, pos + 1);
  }
  tokens.push_back(line.substr(0, pos));
  return tokens;
}
