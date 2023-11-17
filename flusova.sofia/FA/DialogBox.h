#ifndef DIALOG_BOX_H
#define DIALOG_BOX_H

#include <fstream>

namespace flusova {
  struct DialogBox {

    explicit DialogBox(std::ostream& out) : out_(out) {}

    void printGreeting();
    void printAskingLengthInput();
    void printFileInputCase();
    void printUserInputCase();
    void printGetCommand();
    void printGetMessage();
    void printInvalidCommand();
    void printEmptyInput();
    void printHelp();
    void printExit();

    std::ostream& out_;

  };
}

#endif