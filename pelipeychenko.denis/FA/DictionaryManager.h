#ifndef DICTIONARYMANAGER_H
#define DICTIONARYMANAGER_H

#include <string>
#include <iostream>
#include <functional>
#include <ForwardIterators.h>
#include <Queue.h>
#include <BinarySearchTreeIterators.h>

#include "HashTable.h"

namespace pelipeychenko
{
  class DictionaryManager
  {
  public:
    explicit DictionaryManager(std::ostream &out);

    void doCommand(const std::string &command);

    void print(const std::string &key) const;

    void print() const;

    void erase(const std::string &key);

    void merge(const std::string &key, const std::string &value);

    void insert(const std::string &key, const std::string &value);

    void eraseValue(const std::string &key, const std::string &value);

    void emplaceKey(const std::string &oldKey, const std::string &newKey);

    void save(const std::string &filename);

    void clear();

    void load(const std::string &filename);

    void makeUnion(const std::string &firstFile, const std::string &secondFile);

    void makeIntersect(const std::string &firstFile, const std::string &secondFile);

    void makeComplement(const std::string &firstFile, const std::string &secondFile);

  private:
    std::ostream &out_;

    Queue< std::string > wordsQueue;

    HashTable< ForwardList< std::string> > currentDictionary;

    const BinarySearchTree< std::string, std::function< void() > > commands{
      {"PRINT",       [this]{ executePrint(); }},
      {"ERASE",       [this]{ executeErase(); }},
      {"MERGE",       [this]{ executeMerge(); }},
      {"INSERT",      [this]{ executeInsert(); }},
      {"DELETE",      [this]{ executeDelete(); }},
      {"EMPLACE",     [this]{ executeEmplace(); }},
      {"SAVE",        [this]{ executeSave(); }},
      {"CLEAR",       [this]{ executeClear(); }},
      {"LOAD",        [this]{ executeLoad(); }},
      {"UNION",       [this]{ executeUnion(); }},
      {"INTERSECT",   [this]{ executeIntersect(); }},
      {"COMPLEMENT",  [this]{ executeComplement(); }},
    };

    void executePrint();
    void executeErase();
    void executeMerge();
    void executeInsert();
    void executeDelete();
    void executeEmplace();
    void executeSave();
    void executeClear();
    void executeLoad();
    void executeUnion();
    void executeIntersect();
    void executeComplement();

    static void fillDictionaryFromFile(const std::string &fileName, HashTable< ForwardList< std::string > > &dictionary);
  };
}

#endif
