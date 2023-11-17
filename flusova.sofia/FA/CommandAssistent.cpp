#include "CommandAssistent.h"

flusova::CommandAssistent::CommandAssistent(std::string inputFirstName, std::string inputSecondName, std::string outputName) :
        cmd_{{"<user_input>", [this]{ this->message_ = this->encoder_.userInput(std::cin); }},
             {"<file_input>", [this]{ this->message_ = this->encoder_.readFile(inputFirstName_); }},
             {"<encode_message>", [this]{ this->encoder_.encrypt(message_); }},
             {"<decode_message>", [this]{ this->encoder_.decrypt(message_); }},
             {"<user_output>", [this]{ this->encoder_.printEncodingTable(this->message_); }},
             {"<file_output>", [this]{ flusova::writeFile(outputName_, this->encoder_); }},
             {"<generate_random_string>", [this]{ int length = 0; dialog_.printAskingLengthInput();
               std::cin >> length; this->message_ = flusova::generateRandomKey(length); }},
             {"<build_tree>", [this]{ this->encoder_.buildEncodingTable(message_); this->encoder_.printFanoTree(); }},
             {"<merge_files>", [this]{ flusova::mergeFiles(inputFirstName_,
                                                           inputSecondName_, outputName_); }},
             {"<get_help>", [this]{ this->dialog_.printHelp(); }}
        },
        command_(""),
        inputFirstName_(inputFirstName),
        inputSecondName_(inputSecondName),
        outputName_(outputName),
        dialog_(std::cout)
{}

void flusova::CommandAssistent::execute() {
  if (command_.empty()) {
    dialog_.printEmptyInput();
    return;
  }
  auto it = cmd_.find(command_);
  if (it != cmd_.end()) {
    try {
      it->second();
    } catch (const std::runtime_error &e) {
      throw;
    }
  } else {
    dialog_.printInvalidCommand();
    return;
  }
}