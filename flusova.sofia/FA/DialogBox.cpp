#include "DialogBox.h"

void flusova::DialogBox::printGreeting() {
  out_ << "<p>" << '\n' << "Hello, Dear friend! It's time to figure out how does the Shannon-Fano algorithm works!" << '\n';
  out_ << "--------------------------------------------------------------------------------------" << '\n';
  out_ << "You can see the whole tools set with entering <get_help> command! Have a nice time  :)" << '\n' << "<p>" << '\n';
}

void flusova::DialogBox::printAskingLengthInput() {
  out_ << "Please, enter the length of string that you are going to generate: ";
}

void flusova::DialogBox::printFileInputCase() {
  out_ << "It seems that you put files name in CML arguments. So, you can encode and merge them." << '\n';
  out_ << "In addition, you can output encoded information in some of them!" << '\n';
}

void flusova::DialogBox::printUserInputCase() {
  out_ << "You didn't put any file name in CML arguments. So, you can use user's input or just generate random string!" << '\n';
}

void flusova::DialogBox::printGetCommand() {
  out_ << "Please, enter the next command: ";
}

void flusova::DialogBox::printGetMessage() {
  out_ << "Please, enter the message that is going to be encoded: ";
}

void flusova::DialogBox::printInvalidCommand() {
  out_ << "<INVALID COMMAND>" << '\n';
}

void flusova::DialogBox::printEmptyInput() {
  out_ << "<EMPTY INPUT>" << '\n';
}

void flusova::DialogBox::printHelp() {
  out_ << "The order of entered commands must be correct: type of input -> encoding/decoding -> type of output." << '\n';
  out_ << "----------------------------------------------------------------------------------------------------" << '\n';
  out_ << "<user_input> - you should use this command, if you're going to input message with keypad." << '\n';
  out_ << "----------------------------------------------------------------------------------------------------" << '\n';
  out_ << "<file_input> - you should use this command, if you're having file with text which is going to be encoded." << '\n';
  out_ << "----------------------------------------------------------------------------------------------------" << '\n';
  out_ << "<generate_random_string> - if you can't think of the input message, you can generate it with the following command." << '\n';
  out_ << "----------------------------------------------------------------------------------------------------" << '\n';
  out_ << "<encode_message> - when program got the message, you can encode it with the following command." << '\n';
  out_ << "----------------------------------------------------------------------------------------------------" << '\n';
  out_ << "<decode_message> - after some actions with your message, you may decode it with the following command." << '\n';
  out_ << "----------------------------------------------------------------------------------------------------" << '\n';
  out_ << "<user_output> - if you want to get the whole information table to your console, you may use this command." << '\n';
  out_ << "----------------------------------------------------------------------------------------------------" << '\n';
  out_ << "<file_output> - if you want to get the whole information table to the certain file, you may use this command." << '\n';
  out_ << "----------------------------------------------------------------------------------------------------" << '\n';
  out_ << "<build_tree> - for best understanding of Shannon-Fano algorithm you can create an alphabet tree." << '\n';
  out_ << "----------------------------------------------------------------------------------------------------" << '\n';
  out_ << "<merge_files> - this command is weird, but if you want to unite files in one, you can use it." << '\n';
  out_ << "----------------------------------------------------------------------------------------------------" << '\n';
  out_ << "<exit> - you can stop the program with following command." << '\n';
}

void flusova::DialogBox::printExit() {
  out_ << "Thank you for using my program! Coming soon." << '\n';
  out_ << "Exit..." << '\n';
}