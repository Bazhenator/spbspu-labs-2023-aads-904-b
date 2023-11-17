#include <limits>
#include <utility>

#include "QueryManagement.h"

namespace nemchinov
{
  CommandManager::CommandManager(const std::string &stringForDict, const std::string &command, std::ostream &out)
  {
    BinarySearchTree< int, std::string > avlTree{};
    putInDict(avlTree, stringForDict);

    if (command == "ascending")
    {
      KeyValueSum f = avlTree.traverse_lnr(KeyValueSum());
      out << f;
      return;
    } else if (command == "descending")
    {
      KeyValueSum f = avlTree.traverse_rnl(KeyValueSum());
      out << f;
      return;
    } else if (command == "breadth")
    {
      KeyValueSum f = avlTree.traverse_breadth(KeyValueSum());
      out << f;
      return;
    }
    printErrorCommand(out);
    throw std::logic_error("error command");
  }

  void CommandManager::putInDict(BinarySearchTree< int, std::string > &dict, std::string string)
  {
    while (string.length() != 0)
    {
      int i = std::stoi(getWord(string));
      dict[i] = getWord(string);
    }
  }

  std::string CommandManager::getWord(std::string &string)
  {
    std::string result;
    int i = string.find(' ');
    result = string.substr(0, i);
    if (i < 0)
    {
      string = "";
    } else
    {
      ++i;
      string = string.substr(i, string.length());
    }
    return result;
  }

  void CommandManager::KeyValueSum::operator()(const int &key, const std::string &value)
  {
    keySum += key;
    valueSum += ' ' + value;
  }

  CommandManager::KeyValueSum::KeyValueSum() :
          keySum(),
          valueSum()
  {
  }

  std::ostream &operator<<(std::ostream &out, CommandManager::KeyValueSum &f)
  {
    out << f.keySum << f.valueSum;
    out << "\n";
    return out;
  }
}
