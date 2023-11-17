#ifndef QUERY_MANAGEMENT_H
#define QUERY_MANAGEMENT_H

#include <iostream>
#include <BinarySearchTree.h>
#include <BinaryTreeIterator.h>
#include <ErrorMessages.h>

#include "BinaryTraverse.h"

namespace nemchinov
{
  class CommandManager
  {
  public:
    explicit CommandManager(const std::string &, const std::string &, std::ostream &);

  private:
    static void putInDict(BinarySearchTree< int, std::string > &, std::string);
    static std::string getWord(std::string &);

    struct KeyValueSum
    {
      KeyValueSum();

      int keySum;
      std::string valueSum;

      void operator()(const int &, const std::string &);
    };

    friend std::ostream &operator<<(std::ostream &out, KeyValueSum &f);

  };

  std::ostream &operator<<(std::ostream &out, CommandManager::KeyValueSum &f);
}

#endif
