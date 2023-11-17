#include "DictionariesManagment.h"

#include <iostream>

#include "output.h"

namespace siobko {
  Command::Command(std::deque< std::string > commandInfo)
  {
    std::string commandName = commandInfo.front();

    if (commandName == "print" && commandInfo.size() == 2) {
      commandInfo.pop_front();

      type = CommandType::PRINT;
      dataset = commandInfo.front();
      extraDatasets.first = "";
      extraDatasets.second = "";
    } else if (commandInfo.size() == 4) {
      commandInfo.pop_front();

      if (commandName == "complement") {
        type = CommandType::COMPLEMENT;
      } else if (commandName == "intersect") {
        type = CommandType::INTERSECT;
      } else if (commandName == "union") {
        type = CommandType::UNION;
      } else {
        printInvalidCommandErrorMessage(std::cout);
      }

      dataset = commandInfo.front();
      commandInfo.pop_front();

      extraDatasets.first = commandInfo.front();
      commandInfo.pop_front();

      extraDatasets.second = commandInfo.front();
    } else {
      type = CommandType::INVALID;
    }
  }

  void Command::execute(DictionariesManagment *dictionariesManagment) const
  {
    switch (type) {
      case CommandType::INVALID:
        printInvalidCommandErrorMessage(std::cout);
        return;
      case CommandType::PRINT:
        dictionariesManagment->printDictionary(dataset);
        return;
      case CommandType::UNION:
        dictionariesManagment->mergeDictionary(dataset, extraDatasets.first, extraDatasets.second);
        return;
      case CommandType::INTERSECT:
        dictionariesManagment->intersectDictionary(dataset, extraDatasets.first, extraDatasets.second);
        return;
      case CommandType::COMPLEMENT:
        dictionariesManagment->complementDictionary(dataset, extraDatasets.first, extraDatasets.second);
        return;
    }
  }

  void DictionariesManagment::inputDictionary(const std::deque< std::string >& dictionaryInfo)
  {
    const std::string& dictionaryName = dictionaryInfo[0];
    AVLTree< std::string, std::string > dictionary;

    for (size_t i = 1u; i != dictionaryInfo.size(); i += 2u) {
      dictionary.push(dictionaryInfo[i], dictionaryInfo[i + 1u]);
    }

    pushDictionary(dictionaryName, dictionary);
  }

  void DictionariesManagment::pushDictionary(const std::string& title, const AVLTree< std::string, std::string >& dictionary)
  {
    dictionaries_.push(title, dictionary);
  }

  void siobko::DictionariesManagment::inputCommand(const std::deque< std::string >& commandsInfo)
  {
    commands_.emplace_back(commandsInfo);
  }

  void siobko::DictionariesManagment::executeCommands()
  {
    for (const Command& command: commands_) {
      command.execute(this);
    }
  }
  void DictionariesManagment::printDictionary(const std::string& dataset)
  {
    try {
      AVLTree< std::string, std::string > dictionary(dictionaries_.get(dataset));
      if (dictionary.isEmpty()) {
        printEmptyErrorMessage(std::cout);
        return;
      }
      std::cout << dataset;
      dictionary.print();
    } catch (...) {
      printInvalidCommandErrorMessage(std::cout);
      return;
    }
  }
  void DictionariesManagment::complementDictionary(const std::string& newDataset, const std::string& dataset, const std::string& yaDataset)
  {
    AVLTree< std::string, std::string > resultDictionary;

    try {
      AVLTree< std::string, std::string > dataset1(dictionaries_.get(dataset));
      AVLTree< std::string, std::string > dataset2(dictionaries_.get(yaDataset));

      for (AVLTree< std::string, std::string >::iterator it = dataset1.begin(); it != dataset1.end(); ++it) {
        if (!dataset2.contains(it->first)) {
          resultDictionary.push(it->first, it->second);
        }
      }
      for (AVLTree< std::string, std::string >::iterator it = dataset2.begin(); it != dataset2.end(); ++it) {
        if (!dataset1.contains(it->first)) {
          resultDictionary.push(it->first, it->second);
        }
      }
    }
    catch (...) {
      printInvalidCommandErrorMessage(std::cout);
      return;
    }

    dictionaries_.push(newDataset, resultDictionary);
  }
  void DictionariesManagment::intersectDictionary(const std::string& newDataset, const std::string& dataset, const std::string& yaDataset)
  {
    AVLTree< std::string, std::string > resultDictionary;

    try {
      AVLTree< std::string, std::string > dataset1(dictionaries_.get(dataset));
      AVLTree< std::string, std::string > dataset2(dictionaries_.get(yaDataset));

      for (AVLTree< std::string, std::string >::iterator it = dataset1.begin(); it != dataset1.end(); ++it) {
        if (dataset2.contains(it->first)) {
          resultDictionary.push(it->first, it->second);
        }
      }
    }
    catch (...) {
      printInvalidCommandErrorMessage(std::cout);
      return;
    }

    dictionaries_.push(newDataset, resultDictionary);
  }
  void DictionariesManagment::mergeDictionary(const std::string& newDataset, const std::string& dataset, const std::string& yaDataset)
  {
    try {
      AVLTree< std::string, std::string > resultDictionary(dictionaries_.get(dataset));
      resultDictionary.merge(dictionaries_.get(yaDataset));
      pushDictionary(newDataset, resultDictionary);
    }
    catch (...) {
      printInvalidCommandErrorMessage(std::cout);
      return;
    }
  }
}
