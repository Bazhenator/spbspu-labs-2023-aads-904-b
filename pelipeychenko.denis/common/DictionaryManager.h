#ifndef DICTIONARYMANAGER_H
#define DICTIONARYMANAGER_H

#include <string>
#include <iostream>
#include "functions.h"
#include "outputFunctions.h"
#include "Queue.h"

namespace pelipeychenko
{
  template< typename Key, typename Value, template< class, class, class > class Container,
    template< typename > class Comparator = std::less >
  class DictionaryManager
  {
  public:
    DictionaryManager() = default;

    ~DictionaryManager() = default;

    void inputDictionaries(std::istream &in);

    void doCommands(std::istream &in);

    void print(std::string) const;

    void print() const;

    void dictUnion(std::string newName, std::string first, std::string second);

    void dictComplement(std::string newName, std::string first, std::string second);

    void dictIntersect(std::string newName, std::string first, std::string second);

    bool checkCommandCorrectness(const std::string &command) const;

  private:

    Container< std::string, Container< Key, Value, Comparator< Key > >, Comparator< std::string > > dictionaries_;
  };

  template< typename Key, typename Value, template< class, class, class > class Container,
    template< typename > class Comparator >
  void DictionaryManager< Key, Value, Container, Comparator >::inputDictionaries(std::istream &in)
  {
    in >> std::noskipws;
    do {
      Container< Key, Value, Comparator< int > > tempDictionary;
      std::string string;
      std::getline(in, string);
      pelipeychenko::Queue< std::string > queue = split(string);
      std::string name = queue.getHead();
      queue.drop();
      Key key;
      Value value;

      while (!queue.isEmpty()) {
        key = std::stoi(queue.getHead());
        queue.drop();
        value = queue.getHead();
        queue.drop();
        tempDictionary.merge(key, value);
      }

      dictionaries_.merge(name, tempDictionary);
    } while (!in.eof());
  }

  template< typename Key, typename Value, template< class, class, class > class Container,
    template< typename > class Comparator >
  void DictionaryManager< Key, Value, Container, Comparator >::doCommands(std::istream &in)
  {

    std::string string;
    while (!in.eof() && string != "\n" && std::getline(in, string)) {
      if (!checkCommandCorrectness(string)) {
        printInvalidCommand(std::cout);
        continue;
      }
      pelipeychenko::Queue< std::string > queue = split(string);
      std::string command = queue.getHead();
      queue.drop();
      std::string newName = queue.getHead();
      queue.drop();
      if (command == "print") {
        print(newName);
        continue;
      }
      std::string first = queue.getHead();
      queue.drop();
      std::string second = queue.getHead();
      queue.drop();
      if (command == "union") {
        dictUnion(newName, first, second);
      } else if (command == "intersect") {
        dictIntersect(newName, first, second);
      } else if (command == "complement") {
        dictComplement(newName, first, second);
      }
    }
  }

  template< typename Key, typename Value, template< class, class, class > class Container,
    template< typename > class Comparator >
  void DictionaryManager< Key, Value, Container, Comparator >::print(std::string name) const
  {
    if (dictionaries_.get(name).isEmpty()) {
      printEmpty(std::cout);
    } else {
      std::cout << name;
      dictionaries_.get(name).inlinePrint();
      std::cout << "\n";
    }
  }

  template< typename Key, typename Value, template< class, class, class > class Container,
    template< typename > class Comparator >
  void DictionaryManager< Key, Value, Container, Comparator >::print() const
  {
    for (auto iterator = dictionaries_.cbegin(); iterator != dictionaries_.cend(); iterator++) {
      std::cout << iterator->first_;
      iterator->second_.inlinePrint();
    }
  }

  template< typename Key, typename Value, template< class, class, class > class Container,
    template< typename > class Comparator >
  void
  DictionaryManager< Key, Value, Container, Comparator >::dictUnion(std::string newName, std::string first,
                                                                    std::string second)
  {
    Container< Key, Value, Comparator< int > > newDictionary;
    auto firstDict = dictionaries_.get(first);
    for (auto iterator = firstDict.cbegin(); iterator != firstDict.cend(); iterator++) {
      newDictionary.merge(iterator->first_, iterator->second_);
    }

    auto secondDict = dictionaries_.get(second);
    for (auto iterator = secondDict.cbegin(); iterator != secondDict.cend(); iterator++) {
      if (newDictionary.find(iterator->first_) == newDictionary.end()) {
        newDictionary.merge(iterator->first_, iterator->second_);
      }
    }
    dictionaries_.merge(newName, newDictionary);
  }

  template< typename Key, typename Value, template< class, class, class > class Container,
    template< typename > class Comparator >
  void DictionaryManager< Key, Value, Container, Comparator >::dictComplement(std::string newName, std::string first,
                                                                              std::string second)
  {
    Container< Key, Value, Comparator< int > > newDictionary;
    auto firstDict = dictionaries_.get(first);
    auto secondDict = dictionaries_.get(second);
    for (auto iterator = firstDict.cbegin(); iterator != firstDict.cend(); iterator++) {
      if (secondDict.find(iterator->first_) == secondDict.end()) {
        newDictionary.merge(iterator->first_, iterator->second_);
      }
    }
    dictionaries_.merge(newName, newDictionary);
  }

  template< typename Key, typename Value, template< class, class, class > class Container,
    template< typename > class Comparator >
  void
  DictionaryManager< Key, Value, Container, Comparator >::dictIntersect(std::string newName, std::string first,
                                                                        std::string second)
  {
    Container< Key, Value, Comparator< int > > newDictionary;
    auto firstDict = dictionaries_.get(first);
    auto secondDict = dictionaries_.get(second);
    for (auto iterator = firstDict.cbegin(); iterator != firstDict.cend(); iterator++) {
      if (secondDict.find(iterator->first_) != secondDict.end()) {
        newDictionary.merge(iterator->first_, iterator->second_);
      }
    }

    dictionaries_.merge(newName, newDictionary);
  }

  template< typename Key, typename Value, template< class, class, class > class Container,
    template< typename > class Comparator >
  bool DictionaryManager< Key, Value, Container, Comparator >::checkCommandCorrectness(const std::string &command) const
  {
    pelipeychenko::Queue< std::string > queue = split(command);
    if (queue.getSize() != 2 && queue.getSize() != 4) {
      return false;
    }
    std::string commandName = queue.getHead();
    queue.drop();
    if (commandName == "print") {
      return dictionaries_.find(queue.getHead()) != dictionaries_.cend();
    }

    if (commandName != "union" && commandName != "complement" && commandName != "intersect") {
      return false;
    }
    queue.drop();
    std::string first = queue.getHead();
    queue.drop();
    std::string second = queue.getHead();
    if (dictionaries_.find(first) == dictionaries_.cend() || dictionaries_.find(second) == dictionaries_.cend()) {
      return false;
    }
    return true;
  }
}

#endif
