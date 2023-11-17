#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "CommandAnswer.h"

namespace ponomarev {
  template< template< typename, typename, typename > class T, typename Key, typename Value, typename Compare = std::less< Key > >
  class CommandHandler {
    public:
      using type_t = T< Key, Value, Compare >;

      CommandHandler() = delete;
      explicit CommandHandler(std::ostream &os);
      CommandHandler(std::ostream &os, type_t &data);
      ~CommandHandler() = default;

      void inputData(std::istream &is);

      void operator()(std::string command);

    private:
      enum CommandState {
        INVALID,
        EMPTY,
        SUCCESS
      };

      void doPrint(std::vector< std::string > params);
      void doComplement(std::vector< std::string > params);
      void doIntersect(std::vector< std::string > params);
      void doUnion(std::vector< std::string > params);

      void evaluateSuccess();

      type_t data_;
      std::ostream &os_;
      CommandState state_;
  };
  std::vector< std::string > split(std::string line, const char separator);
  template< typename T >
  void inputMapped(std::istream &in, T &data)
  {
    std::string line;
    std::getline(in, line);
    std::vector< std::string > splitedLine = split(line, ' ');
    if (splitedLine.size() % 2 == 0) {
      for (std::size_t i = 0; i < splitedLine.size(); i += 2) {
        data.insert(std::stoi(splitedLine[i]), splitedLine[i + 1]);
      }
    }
  }

  template< template< typename, typename, typename > class T,
            typename Key, typename Value, typename Compare >
  CommandHandler< T, Key, Value, Compare >::CommandHandler(std::ostream &os):
    os_(os),
    state_(SUCCESS)
  {}
  template< template< typename, typename, typename > class T,
            typename Key, typename Value, typename Compare >
  CommandHandler< T, Key, Value, Compare >::CommandHandler(std::ostream &os, type_t &data):
    os_(os),
    state_(SUCCESS),
    data_(data)
  {}
  template< template< typename, typename, typename > class T,
            typename Key, typename Value, typename Compare >
  void CommandHandler< T, Key, Value, Compare >::inputData(std::istream &is)
  {
    while (!is.eof()) {
      std::string name;
      is >> name;
      Value innerData;
      inputMapped(is, innerData);

      data_.insert(name, innerData);
    }
  }
  template< template< typename, typename, typename > class T,
            typename Key, typename Value, typename Compare >
  void CommandHandler< T, Key, Value, Compare >::operator()(std::string command)
  {
    std::vector< std::string > commandParams = split(command, ' ');
    std::string commandName = commandParams[0];
    if (commandParams.size() == 1 || data_.isEmpty()) {
      state_ = CommandState::INVALID;
    } else if (!commandName.compare("print")) {
      doPrint(commandParams);
    } else if (!commandName.compare("complement")) {
      doComplement(commandParams);
    } else if (!commandName.compare("intersect")) {
      doIntersect(commandParams);
    } else if (!commandName.compare("union")) {
      doUnion(commandParams);
    } else {
      state_ = CommandState::INVALID;
    }
    evaluateSuccess();
  }
  template< template< typename, typename, typename > class T,
            typename Key, typename Value, typename Compare >
  void CommandHandler< T, Key, Value, Compare >::doPrint(std::vector< std::string > params)
  {
    if (!data_.contains(params[1])) {
      state_ = CommandState::INVALID;
    } else if (data_.get(params[1]).isEmpty()) {
      state_ = CommandState::EMPTY;
    } else {
      os_ << params[1];
      std::for_each(data_.get(params[1]).begin(), data_.get(params[1]).end(),
                    [&](auto &pair) { os_ << ' ' << pair.first << ' ' << pair.second; });
      os_ << '\n';
    }
  }
  template< template< typename, typename, typename > class T,
            typename Key, typename Value, typename Compare >
  void CommandHandler< T, Key, Value, Compare >::doComplement(std::vector< std::string > params)
  {
    if (params.size() != 4 ||
          !(data_.contains(params[2]) && data_.contains(params[3]))) {
      state_ = CommandState::INVALID;
    } else {
      data_.insert(params[1], complement(data_.get(params[2]),
                                         data_.get(params[3])));
    }
  }
  template< template< typename, typename, typename > class T,
            typename Key, typename Value, typename Compare >
  void CommandHandler< T, Key, Value, Compare >::doIntersect(std::vector< std::string > params)
  {
    if (params.size() != 4 ||
          !(data_.contains(params[2]) && data_.contains(params[3]))) {
      state_ = CommandState::INVALID;
    } else {
      data_.insert(params[1], intersect(data_.get(params[2]),
                                        data_.get(params[3])));
    }
  }
  template< template< typename, typename, typename > class T,
            typename Key, typename Value, typename Compare >
  void CommandHandler< T, Key, Value, Compare >::doUnion(std::vector< std::string > params)
  {
    if (params.size() != 4 ||
          !(data_.contains(params[2]) && data_.contains(params[3]))) {
      state_ = CommandState::INVALID;
    } else {
      data_.insert(params[1], unite(data_.get(params[2]),
                                    data_.get(params[3])));
    }
  }
  template< template< typename, typename, typename > class T,
            typename Key, typename Value, typename Compare >
  void CommandHandler< T, Key, Value, Compare >::evaluateSuccess()
  {
    switch (state_) {
    case CommandState::EMPTY:
      printEmpty(os_);
      break;
    case CommandState::INVALID:
      printInvalidCommand(os_);
      break;
    case CommandState::SUCCESS:
      break;
    }
    state_ = CommandState::SUCCESS;
  }

  template< template< typename, typename, typename > class T,
            typename Key, typename Value, typename Compare >
  T< Key, Value, Compare > complement(const T< Key, Value, Compare > &lhs,
                                      const T< Key, Value, Compare > &rhs);
   template< template< typename, typename, typename > class T,
             typename Key, typename Value, typename Compare >
  T< Key, Value, Compare > intersect(const T< Key, Value, Compare > &lhs,
                                     const T< Key, Value, Compare > &rhs);
  template< template< typename, typename, typename > class T,
            typename Key, typename Value, typename Compare >
  T< Key, Value, Compare > unite(const T< Key, Value, Compare > &lhs,
                                 const T< Key, Value, Compare > &rhs);
}

template< template< typename, typename, typename > class T,
            typename Key, typename Value, typename Compare >
T< Key, Value, Compare > ponomarev::complement(const T< Key, Value, Compare > &lhs,
                                               const T< Key, Value, Compare > &rhs)
{
  T< Key, Value, Compare > newOne = rhs;
  newOne.merge(lhs);
  eraseIf(newOne, [&](const auto &item) { return (lhs.contains(item.first) && rhs.contains(item.first)); });
  return newOne;
}
template< template< typename, typename, typename > class T,
            typename Key, typename Value, typename Compare >
T< Key, Value, Compare > ponomarev::intersect(const T< Key, Value, Compare > &lhs,
                                              const T< Key, Value, Compare > &rhs)
{
  T< Key, Value, Compare > newOne = rhs;
  newOne.merge(lhs);
  eraseIf(newOne, [&](auto &item) { return !(lhs.contains(item.first) && rhs.contains(item.first)); });
  return newOne;
}
template< template< typename, typename, typename > class T,
            typename Key, typename Value, typename Compare >
T< Key, Value, Compare > ponomarev::unite(const T< Key, Value, Compare > &lhs,
                                          const T< Key, Value, Compare > &rhs)
{
  T< Key, Value, Compare > newOne = rhs;
  newOne.merge(lhs);
  return newOne;
}

#endif
