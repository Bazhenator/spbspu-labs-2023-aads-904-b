#ifndef IO_FORMAT_GUARD_H
#define IO_FORMAT_GUARD_H

#include <ios>

namespace ponomarev {
  class IOFormatGuard {
   public:
    explicit IOFormatGuard(std::basic_ios< char > &s);
    ~IOFormatGuard();
   private:
    std::basic_ios< char > & s_;
    char fill_;
    std::basic_ios< char >::fmtflags fmt_;
  };
}

#endif
