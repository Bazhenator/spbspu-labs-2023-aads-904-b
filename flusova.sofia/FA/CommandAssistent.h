#ifndef COMMAND_ASSISTENT_H
#define COMMAND_ASSISTENT_H

#include <string>
#include "MyDictionary.h"
#include "DialogBox.h"
#include "ShannonFanoCodes.h"

namespace flusova {
  class CommandAssistent {
  public:

    using map = flusova::MyDictionary< std::string, std::function< void() > >;

    explicit CommandAssistent(std::string inputFirstName, std::string inputSecondName, std::string outputName);

    void execute();

    flusova::ShannonFano encoder_;
    std::string command_;

  private:

    map cmd_;
    std::string message_;
    std::string inputFirstName_;
    std::string inputSecondName_;
    std::string outputName_;
    flusova::DialogBox dialog_;

  };
}

#endif