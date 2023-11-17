#ifndef DMANAGER_H
#define DMANAGER_H

#include <fstream>
#include <vector>
#include <string>
#include "outputLiterals.h"
#include "BST.h"

namespace flusova {
  template< template< typename, typename, typename > typename T, typename Key, typename Value, typename Comparator = std::less< Key > >
  class DictManager {
  public:

    explicit DictManager(std::ostream& out) : out_(out) {}
    void inputDictionary(std::istream& in);
    void inputData(std::istream& in);
    void manageDictionary(std::string& input);
    bool isEmpty();
    std::vector< std::string > split(std::string& str, const char d);

    T< Key, Value, Comparator > dict_;

  private:

    void doPrint();
    void doComplement();
    void doIntersect();
    void doUnite();
    std::vector< std::string > line_;
    std::ostream& out_;

  };
}

template< template< typename, typename, typename > typename T, typename Key, typename Value, typename Comparator >
void flusova::DictManager< T, Key, Value, Comparator >::inputDictionary(std::istream& in) {
  while (!in.eof()) {
    std::string str = "";
    std::getline(in, str);
    Value inputData;
    std::vector< std::string > storage = split(str, ' ');
    for (size_t i = 1; i < storage.size(); i += 2) {
      int key = std::stoi(storage[i]);
      inputData[key] = storage[i + 1];
    }
    dict_[storage[0]] = inputData;
  }
}

template< template< typename, typename, typename > typename T, typename Key, typename Value, typename Comparator >
void flusova::DictManager< T, Key, Value, Comparator >::inputData(std::istream& in) {
  std::string line;
  std::getline(in, line);
  line_ = split(line, ' ');
  if (line_.size() % 2 == 0) {
    for (std::size_t i = 0; i < line_.size(); i += 2) {
      dict_.insert(std::stoi(line_[i]), line_[i + 1]);
    }
  }
}

template< template< typename, typename, typename > typename T, typename Key, typename Value, typename Comparator >
void flusova::DictManager< T, Key, Value, Comparator >::manageDictionary(std::string& input) {
  line_ = split(input, ' ');
  if (line_[0] == "print") {
    doPrint();
  } else if (line_[0] == "complement") {
    doComplement();
  } else if (line_[0] == "intersect") {
    doIntersect();
  } else if (line_[0] == "union") {
    doUnite();
  } else {
    flusova::printInvalidCmd(out_);
  }
}

template< template< typename, typename, typename > typename T, typename Key, typename Value, typename Comparator >
bool flusova::DictManager< T, Key, Value, Comparator >::isEmpty() {
  return dict_.isEmpty();
}

template< template< typename, typename, typename > typename T, typename Key, typename Value, typename Comparator >
std::vector< std::string >
flusova::DictManager< T, Key, Value, Comparator >::split(std::string& str, const char d) {
  std::vector< std::string > splitted;
  std::size_t start;
  while ((start = str.find(d)) != std::string::npos) {
    if (!str.substr(0,start).empty()) {
      splitted.push_back(str.substr(0, start));
    }
    str.erase(0, start + 1);
  }
  splitted.push_back(str.substr(0, start));
  return splitted;
}

template< template< typename, typename, typename > typename T, typename Key, typename Value, typename Comparator >
void flusova::DictManager< T, Key, Value, Comparator >::doPrint() {
  if ((dict_.find(line_[1]) == dict_.end())) {
    flusova::printInvalidCmd(out_);
  } else if (dict_[line_[1]].isEmpty()) {
    flusova::printEmpty(out_);
  } else {
    std::string name = line_[1];
    out_ << name;
    auto it = dict_[name].begin();
    while (it != dict_[name].end()) {
      out_ << " " << (*it).first << " " << (*it).second;
      it++;
    }
    out_ << "\n";
  }
}

template< template< typename, typename, typename > typename T, typename Key, typename Value, typename Comparator >
void flusova::DictManager< T, Key, Value, Comparator >::doComplement() {
  if ((line_.size() != 4) || ((dict_.find(line_[2]) == dict_.end())
                              || (dict_.find(line_[3]) == dict_.end()))) {
    flusova::printInvalidCmd(out_);
  } else {
    dict_[line_[1]] = flusova::complement(dict_[line_[2]], dict_[line_[3]]);
  }
}

template< template< typename, typename, typename > typename T, typename Key, typename Value, typename Comparator >
void flusova::DictManager< T, Key, Value, Comparator >::doIntersect() {
  if ((line_.size() != 4) || ((dict_.find(line_[2]) == dict_.end())
                              || (dict_.find(line_[3]) == dict_.end()))) {
    flusova::printInvalidCmd(out_);
  } else {
    dict_[line_[1]] = flusova::intersect(dict_[line_[2]], dict_[line_[3]]);
  }
}

template< template< typename, typename, typename > typename T, typename Key, typename Value, typename Comparator >
void flusova::DictManager< T, Key, Value, Comparator >::doUnite() {
  if ((line_.size() != 4) || ((dict_.find(line_[2]) == dict_.end())
                              || (dict_.find(line_[3]) == dict_.end()))) {
    flusova::printInvalidCmd(out_);
  } else {
    dict_[line_[1]] = flusova::unite(dict_[line_[2]], dict_[line_[3]]);
  }
}

#endif
