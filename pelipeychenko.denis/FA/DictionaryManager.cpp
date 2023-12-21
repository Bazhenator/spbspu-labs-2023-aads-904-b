#include "DictionaryManager.h"
#include <fstream>
#include <string>
#include <functions.h>
#include <outputFunctions.h>

namespace pelipeychenko
{
  DictionaryManager::DictionaryManager(std::ostream &out):
    out_(out)
  {}

  void DictionaryManager::doCommand(const std::string &command)
  {
    wordsQueue = split(command);
    std::string commandName = wordsQueue.getHead();
    wordsQueue.drop();
    auto it = commands.find(commandName);
    if (it != commands.cend()) {
      it->second_();
    } else {
      printInvalidCommand(out_);
    }
    while (!wordsQueue.isEmpty()) {
      wordsQueue.drop();
    }
  }

  void DictionaryManager::print(const std::string &key) const
  {
    ForwardList< std::string > words;
    try {
      words = currentDictionary.get(key);
    } catch (std::runtime_error &e) {
      printInvalidArguments(out_);
      return;
    }
    for (const auto &it: words) {
      out_ << it << " ";
    }
    out_ << "\n";
  }

  void DictionaryManager::print() const
  {
    if (currentDictionary.isEmpty()) {
      printEmpty(out_);
      return;
    }
    auto keys = currentDictionary.getKeys();
    for (const auto &key: keys) {
      out_ << key << " ";
      print(key);
    }
  }

  void DictionaryManager::erase(const std::string &key)
  {
    currentDictionary.erase(key);
  }

  void DictionaryManager::merge(const std::string &key, const std::string &value)
  {
    if (!currentDictionary.find(key)) {
      ForwardList< std::string > values{value};
      currentDictionary.merge(key, values);
    } else {
      insert(key, value);
    }
  }

  void DictionaryManager::insert(const std::string &key, const std::string &value)
  {
    auto words = currentDictionary.get(key);
    words.pushBack(value);

    currentDictionary.erase(key);
    currentDictionary.merge(key, words);
  }

  void DictionaryManager::eraseValue(const std::string &key, const std::string &value)
  {
    auto words = currentDictionary.get(key);
    words.remove(value);

    currentDictionary.erase(key);
    currentDictionary.merge(key, words);
  }

  void DictionaryManager::emplaceKey(const std::string &oldKey, const std::string &newKey)
  {
    auto words = currentDictionary.get(oldKey);
    currentDictionary.erase(oldKey);
    currentDictionary.merge(newKey, words);
  }

  void DictionaryManager::save(const std::string &filename)
  {
    std::ofstream file;
    file.open(filename);

    if (!file.is_open()) {
      throw std::runtime_error("Error with file writing");
    }

    auto keys = currentDictionary.getKeys();
    for (const auto &key: keys) {
      std::string line;
      line += key + " ";
      for (const auto &word: currentDictionary.get(key)) {
        line += word + " ";
      }
      line.resize(line.length() - 1);
      file << line << "\n";
    }
    file.close();
  }

  void DictionaryManager::clear()
  {
    currentDictionary.clear();
  }

  void DictionaryManager::load(const std::string &fileName)
  {
    fillDictionaryFromFile(fileName, currentDictionary);
  }

  void DictionaryManager::makeUnion(const std::string &firstFile, const std::string &secondFile)
  {
    using map = HashTable< ForwardList< std::string > >;

    map firstDictionary;
    fillDictionaryFromFile(firstFile, firstDictionary);
    map secondDictionary;
    fillDictionaryFromFile(secondFile, secondDictionary);

    auto firstKeys = firstDictionary.getKeys();
    map result;
    for (const auto &key: firstKeys) {
      result.merge(key, firstDictionary.get(key));
    }

    auto secondKeys = secondDictionary.getKeys();
    for (const auto &key: secondKeys) {
      if (!firstDictionary.find(key)) {
        result.merge(key, secondDictionary.get(key));
      }
    }
    clear();
    currentDictionary = result;
  }

  void DictionaryManager::makeIntersect(const std::string &firstFile, const std::string &secondFile)
  {
    using map = HashTable< ForwardList< std::string > >;

    map firstDictionary;
    fillDictionaryFromFile(firstFile, firstDictionary);
    map secondDictionary;
    fillDictionaryFromFile(secondFile, secondDictionary);

    auto firstKeys = firstDictionary.getKeys();
    map result;
    for (const auto &key: firstKeys) {
      if (secondDictionary.find(key)) {
        result.merge(key, firstDictionary.get(key));
      }
    }
    clear();
    currentDictionary = result;
  }

  void DictionaryManager::makeComplement(const std::string &firstFile, const std::string &secondFile)
  {
    using map = HashTable< ForwardList< std::string > >;

    map firstDictionary;
    fillDictionaryFromFile(firstFile, firstDictionary);
    map secondDictionary;
    fillDictionaryFromFile(secondFile, secondDictionary);

    auto firstKeys = firstDictionary.getKeys();
    map result;
    for (const auto &key: firstKeys) {
      if (!secondDictionary.find(key)) {
        result.merge(key, firstDictionary.get(key));
      }
    }
    clear();
    currentDictionary = result;
  }

  void DictionaryManager::executePrint()
  {
    if (wordsQueue.getSize() == 0) {
      print();
      return;
    } else if (wordsQueue.getSize() != 1) {
      printInvalidArguments(out_);
    }
    std::string keyName = wordsQueue.getHead();
    wordsQueue.drop();
    print(keyName);
  }

  void DictionaryManager::executeErase()
  {
    if (wordsQueue.getSize() != 1) {
      printInvalidArguments(out_);
      return;
    }
    erase(wordsQueue.getHead());
    wordsQueue.drop();
  }

  void DictionaryManager::executeMerge()
  {
    if (wordsQueue.getSize() < 2) {
      printInvalidArguments(out_);
      return;
    }
    std::string keyName = wordsQueue.getHead();
    wordsQueue.drop();
    ForwardList< std::string > values;
    while (!wordsQueue.isEmpty()) {
      values.pushBack(wordsQueue.getHead());
      wordsQueue.drop();
    }
    currentDictionary.merge(keyName, values);
  }

  void DictionaryManager::executeInsert()
  {
    if (wordsQueue.getSize() != 2) {
      printInvalidArguments(out_);
      return;
    }
    std::string keyName = wordsQueue.getHead();
    wordsQueue.drop();
    std::string newWord = wordsQueue.getHead();
    wordsQueue.drop();
    insert(keyName, newWord);
  }

  void DictionaryManager::executeDelete()
  {
    if (wordsQueue.getSize() != 2) {
      printInvalidArguments(out_);
      return;
    }
    std::string keyName = wordsQueue.getHead();
    wordsQueue.drop();
    std::string wordToRemove = wordsQueue.getHead();
    wordsQueue.drop();

    eraseValue(keyName, wordToRemove);
  }

  void DictionaryManager::executeEmplace()
  {
    if (wordsQueue.getSize() != 2) {
      printInvalidArguments(out_);
      return;
    }
    std::string oldKey = wordsQueue.getHead();
    wordsQueue.drop();
    std::string newKey = wordsQueue.getHead();
    wordsQueue.drop();
    emplaceKey(oldKey, newKey);
  }

  void DictionaryManager::executeSave()
  {
    if (wordsQueue.getSize() != 1) {
      printInvalidArguments(out_);
      return;
    }
    std::string fileName = wordsQueue.getHead();
    wordsQueue.drop();
    save(fileName);
  }

  void DictionaryManager::executeClear()
  {
    if (wordsQueue.getSize() != 0) {
      printInvalidArguments(out_);
      return;
    }
    clear();
  }

  void DictionaryManager::executeLoad()
  {
    if (wordsQueue.getSize() != 1) {
      printInvalidArguments(out_);
      return;
    }
    std::string fileName(wordsQueue.getHead());
    wordsQueue.drop();
    load(fileName);
  }

  void DictionaryManager::executeUnion()
  {
    if (wordsQueue.getSize() != 2) {
      printInvalidArguments(out_);
      return;
    }

    std::string firstFile = wordsQueue.getHead();
    wordsQueue.drop();
    std::string secondFile = wordsQueue.getHead();
    wordsQueue.drop();

    makeUnion(firstFile, secondFile);
  }

  void DictionaryManager::executeIntersect()
  {
    if (wordsQueue.getSize() != 2) {
      printInvalidArguments(out_);
      return;
    }

    std::string firstFile = wordsQueue.getHead();
    wordsQueue.drop();
    std::string secondFile = wordsQueue.getHead();
    wordsQueue.drop();

    makeIntersect(firstFile, secondFile);
  }

  void DictionaryManager::executeComplement()
  {
    if (wordsQueue.getSize() != 2) {
      printInvalidArguments(out_);
      return;
    }

    std::string firstFileName = wordsQueue.getHead();
    wordsQueue.drop();
    std::string secondFileName = wordsQueue.getHead();
    wordsQueue.drop();

    makeComplement(firstFileName, secondFileName);
  }

  void DictionaryManager::fillDictionaryFromFile(const std::string &fileName,
                                                 HashTable< ForwardList< std::string >> &dictionary)
  {
    std::ifstream in;
    in.open(fileName);
    if (!in.is_open()) {
      throw std::runtime_error("Error with file reading");
    }
    while (!in.eof()) {
      std::string line;
      std::getline(in, line);
      if (line.empty()) {
        continue;
      }
      auto words = split(line);
      std::string key = words.getHead();
      words.drop();
      if (dictionary.find(key)) {
        dictionary.erase(key);
      }

      ForwardList< std::string > values;
      while (!words.isEmpty()) {
        values.pushBack(words.getHead());
        words.drop();
      }
      dictionary.merge(key, values);
    }
  }
}
