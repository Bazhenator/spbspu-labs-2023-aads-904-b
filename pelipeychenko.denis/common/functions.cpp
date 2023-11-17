#include "functions.h"

pelipeychenko::Queue< std::string > pelipeychenko::split(std::string string)
{
  std::string tempString;
  pelipeychenko::Queue< std::string > queue;
  for (size_t i = 0; i < string.size(); i++) {
    if (string[i] != ' ') {
      tempString += string[i];
    } else {
      queue.push(tempString);
      tempString.clear();
    }
  }
  queue.push(tempString);
  return queue;
}
