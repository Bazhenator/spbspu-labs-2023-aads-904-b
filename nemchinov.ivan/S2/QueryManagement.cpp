#include "QueryManagement.h"

namespace nemchinov
{
  void CommandManagement::putInDict(Dictionary< std::string, Dictionary< int, std::string > > &dict, std::string string)
  {
    std::string nameForDict{CommandManagement::getWord(string)};
    dict[nameForDict] = Dictionary< int, std::string >{};
    while (string.length() != 0)
    {
      int i = std::stoi(CommandManagement::getWord(string));
      dict[nameForDict][i] = CommandManagement::getWord(string);
    }
  }

  CommandManagement::CommandManagement(Dictionary< std::string, Dictionary< int, std::string > > &dict,
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

  void CommandManagement::commandCall(Dictionary< std::string, Dictionary< int, std::string > > &dict,
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
        dict[nameOfDict].complement(dict[firstDict], dict[secondDict]);
        return;
      }
      if (command == "intersect")
      {
        dict[nameOfDict].intersect(dict[firstDict], dict[secondDict]);
        return;
      }
      if (command == "union")
      {
        dict[nameOfDict].dictUnion(dict[firstDict], dict[secondDict]);
        return;
      }
      printErrorCommand(out_);
    }
    catch (...)
    {
      printErrorCommand(out_);
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

  CommandManagement::Print::Print(std::istream &in, std::ostream &out) :
          in_(in),
          out_(out)
  {}

  void CommandManagement::Print::operator()(Dictionary< std::string, Dictionary< int, std::string > > &globalDict,
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
    out_ << nameOfDict << " ";
    globalDict[nameOfDict].print(out_);
  }
}
