#include <iostream>
#include "CommandAssistent.h"
#include "DialogBox.h"

int main(int argc, char* argv[]) {
  flusova::DialogBox dialogAssistent(std::cout);
  dialogAssistent.printGreeting();
  std::string inputFirst = "";
  std::string inputSecond = "";
  std::string output = "";
  if (argc == 1) {
    dialogAssistent.printUserInputCase();
  } else if (argc == 3 || argc == 4) {
    dialogAssistent.printFileInputCase();
    if (argc == 3) {
      inputFirst = argv[1];
      output = argv[2];
    } else {
      inputFirst = argv[1];
      inputSecond = argv[2];
      output = argv[3];
    }
  } else {
    std::cerr << "Too many arguments was given! " << '\n';
    dialogAssistent.printExit();
    return 1;
  }
  flusova::CommandAssistent assistent(inputFirst, inputSecond, output);
  const std::string EXIT = "<exit>";
  dialogAssistent.printGetCommand();
  std::getline(std::cin, assistent.command_);
  do {
    if (assistent.command_ == EXIT) {
      dialogAssistent.printExit();
      return 0;
    }
    try {
      assistent.execute();
    } catch (std::runtime_error& e) {
      std::cerr << e.what() << '\n';
      return 1;
    }
    dialogAssistent.printGetCommand();
    assistent.command_ = "";
  } while (std::getline(std::cin, assistent.command_));
}