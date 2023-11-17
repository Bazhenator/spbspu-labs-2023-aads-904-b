#ifndef COMMON_MANAGE_WITH_C_STRING_H
#define COMMON_MANAGE_WITH_C_STRING_H

#include <cstddef>
#include <iostream>

namespace ponomarev {
  char *input(std::istream &in, std::size_t &capacity);
}

#endif
