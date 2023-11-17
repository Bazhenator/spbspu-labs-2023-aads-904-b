#ifndef QUERY_MANAGEMENT_H
#define QUERY_MANAGEMENT_H

#include <ErrorMessages.h>

#include "Dictionary.h"

namespace nemchinov
{
  class CommandManagement
  {
  public:
    static void putInDict(Dictionary< std::string, Dictionary< int, std::string > > &, std::string);
    explicit CommandManagement(Dictionary< std::string, Dictionary< int, std::string > > &,
                               std::istream &in = std::cin,
                               std::ostream &out = std::cout);

  private:
    void commandCall(Dictionary< std::string, Dictionary< int, std::string > > &, std::string &);
    static std::string getWord(std::string &);

    struct Print
    {
    public:
      Print() = delete;
      Print(std::istream &, std::ostream &);
      void operator()(Dictionary< std::string, Dictionary< int, std::string > > &, const std::string) const;
    private:
      std::istream &in_;
      std::ostream &out_;
    };

    Print print;
    std::istream &in_;
    std::ostream &out_;
  };
}

#endif
