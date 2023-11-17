#include "manageWithCStrings.h"

char *ponomarev::input(std::istream &in, std::size_t &capacity)
{
  std::size_t index = 0;
  char *string = new char[capacity];
  in >> std::noskipws;
  do {
    if (index == capacity) {
      char *newStr = nullptr;
      try {
        newStr = new char[capacity + 20];
      } catch (const std::bad_alloc &e) {
        delete[] string;
        throw;
      }
      for (char *i = string, *j = newStr; i != string + index; ++i, ++j) {
        *j = *i;
      }
      delete[] string;
      string = newStr;
      capacity += 20;
    }
    in >> string[index++];
  } while (in && (string[index - 1] != '\n'));
  string[(index ? (index - 1) : 0)] = '\0';
  if (!string[0]) {
    delete[] string;
    throw std::runtime_error("Not empty string expected.");
  }
  return string;
}
