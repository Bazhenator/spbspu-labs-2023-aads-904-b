
#ifndef DICTIONARIES_MANAGMENT_H
#define DICTIONARIES_MANAGMENT_H

#include <deque>
#include <string>

#include "Dictionary.h"

namespace siobko {
  class DictionariesManagment;

  struct Command {
    enum CommandType {
      PRINT,
      COMPLEMENT,
      INTERSECT,
      UNION,
      INVALID
    };

    Command() = default;
    explicit Command(std::deque< std::string > commandInfo);
    ~Command() = default;
    void execute(DictionariesManagment *dictionariesManagment) const;

    CommandType type;
    std::string dataset;
    std::pair< std::string, std::string > extraDatasets;
  };

  class DictionariesManagment {
  public:
    void inputDictionary(const std::deque< std::string >& dictionaryInfo);
    void inputCommand(const std::deque< std::string >& commandsInfo);
    void pushDictionary(const std::string& name, const Dictionary< std::string, std::string >& dictionary);
    void printDictionary(const std::string& dataset);
    void complementDictionary(const std::string& newDataset, const std::string& dataset, const std::string& yaDataset);
    void intersectDictionary(const std::string& newDataset, const std::string& dataset, const std::string& yaDataset);
    void mergeDictionary(const std::string& newDataset, const std::string& dataset, const std::string& yaDataset);
    void executeCommands();

  private:
    Dictionary< std::string, Dictionary< std::string, std::string > > dictionaries_;
    std::deque< Command > commands_;
  };
}
#endif
