#ifndef CMD_MAP_H
#define CMD_MAP_H

#include <string>
#include <functional>
#include <vector>
#include "BST.h"
#include "Foo_Object.h"
#include "MyDictionary.h"

namespace flusova {
  class CMDmap {
  public:

    using functor = flusova::SumKeysAndValues< int, std::string >;
    using map = flusova::MyDictionary< std::string, std::function< void() > >;

    explicit CMDmap(std::string& command) :
      cmd_{
           {"ascending", [this](){ f_ = dict_.traverse_lnr(f_); }},
           {"descending", [this](){ f_ = dict_.traverse_rnl(f_); }},
           {"breadth", [this](){ f_ = dict_.traverse_breadth(f_); }}},
      command_(command)
    {}

    void inputData(std::istream& in);
    std::vector< std::string > split(std::string& str, const char d);
    void doTraverse();

    flusova::BinarySearchTree< int, std::string, std::less< int > > dict_;

  private:

    map cmd_;
    std::string command_;
    std::vector< std::string > line_;
    functor f_;

  };

}

#endif
