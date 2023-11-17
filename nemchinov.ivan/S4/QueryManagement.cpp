#include <ErrorMessages.h>

#include "QueryManagement.h"

namespace nemchinov
{
  void CommandManagement::putInDict(BinarySearchTree< std::string, BinarySearchTree< int, std::string>> &dict,
                                    std::string string)
  {
    std::string nameForDict{CommandManagement::getWord(string)};
    dict[nameForDict];
    while (string.length() != 0)
    {
      int i = std::stoi(CommandManagement::getWord(string));
      dict[nameForDict][i] = CommandManagement::getWord(string);
    }
  }

  CommandManagement::CommandManagement(BinarySearchTree< std::string, BinarySearchTree< int, std::string > > &dict,
                                       std::istream &in,
                                       std::ostream &out) :
          print(in, out), in_(in), out_(out)
  {
    std::string inputStr;
    while (getline(in_, inputStr))
    {
      if (inputStr.empty())
      {
        continue;
      }
      commandCall(dict, inputStr);
    }
  }

  std::string CommandManagement::getWord(std::string &string)
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

  void CommandManagement::commandCall(BinarySearchTree< std::string, BinarySearchTree< int, std::string > > &dict,
                                      std::string &inputStr)
  {
    try
    {
      std::string command = getWord(inputStr);
      std::string nameOfDict = getWord(inputStr);

      if (command == "print")
      {
        print(dict, nameOfDict);
        return;
      }

      std::string firstDict = getWord(inputStr);
      std::string secondDict = getWord(inputStr);

      if (!dict.contains(firstDict) || !dict.contains(secondDict))
      {
        printErrorCommand(out_);
        return;
      }
      if (command == "complement")
      {
        complement(dict[nameOfDict], dict[firstDict], dict[secondDict]);
        return;
      }
      if (command == "intersect")
      {

        intersect(dict[nameOfDict], dict[firstDict], dict[secondDict]);
        return;
      }
      if (command == "union")
      {
        dictUnion(dict[nameOfDict], dict[firstDict], dict[secondDict]);
        return;
      }
      printErrorCommand(out_);
    }
    catch (...)
    {
      printErrorCommand(out_);
    }
  }

  CommandManagement::Print::Print(std::istream &in, std::ostream &out) :
          in_(in),
          out_(out)
  {}

  void CommandManagement::Print::operator()(
          BinarySearchTree< std::string, BinarySearchTree< int, std::string > > &globalDict,
          const std::string nameOfDict) const
  {
    if (!globalDict.contains(nameOfDict))
    {
      printErrorCommand(out_);
      return;
    }
    if (globalDict[nameOfDict].size() == 0)
    {
      printEmptyDict(out_);
      return;
    }
    BinarySearchTree< int, std::string > &dict = globalDict[nameOfDict];

    int length = dict.size();
    int i = 0;
    out_ << nameOfDict << " ";
    for (auto it = dict.begin(); it != dict.end(); ++it)
    {
      out_ << it.key() << " " << it.value();
      if (++i < length) out_ << " ";
    }
    std::cout << "\n";
  }

  void CommandManagement::Complement::operator()(BinarySearchTree< int, std::string > &result,
                                                 BinarySearchTree< int, std::string > &first,
                                                 BinarySearchTree< int, std::string > &second)
  {
    if (&result == &first)
    {
      result.delIf([](int key, dictionary &other)
                   { return other.contains(key); }, second);
      return;
    }
    if (&result == &second)
    {
      result.delIf([](int key, dictionary &other)
                   { return other.contains(key); }, first);
      return;
    }
    for (auto it = first.begin(); it != first.end(); ++it)
    {
      if (second.contains(it.key())) continue;
      result[it.key()] = it.value();
    }
    for (auto it = second.begin(); it != second.end(); ++it)
    {
      if (first.contains(it.key())) continue;
      result[it.key()] = it.value();
    }
  }

  void CommandManagement::Intersect::operator()(BinarySearchTree< int, std::string > &result,
                                                BinarySearchTree< int, std::string > &first,
                                                BinarySearchTree< int, std::string > &second)
  {
    if (&result == &first)
    {
      first.delIf([](int key, dictionary &other)
                  { return !other.contains(key); }, second);
      return;
    }
    if (&result == &second)
    {
      first.delIf([](int key, dictionary &other)
                  { return !other.contains(key); }, first);
      return;
    }

    result.clean();

    for (auto it = first.begin(); it != first.end(); ++it)
    {
      if (second.contains(it.key()))
      {
        result[it.key()] = it.value();
      }
    }
  }

  void CommandManagement::DictUnion::operator()(BinarySearchTree< int, std::string > &result,
                                                BinarySearchTree< int, std::string > &first,
                                                BinarySearchTree< int, std::string > &second)
  {
    if (&result != &first && &result != &second) result.clean();
    for (auto it = first.begin(); it != first.end(); ++it)
    {
      result[it.key()] = it.value();
    }
    for (auto it = second.begin(); it != second.end(); ++it)
    {
      if (!result.contains(it.key()))
      {
        result[it.key()] = it.value();
      }
    }
  }
}
