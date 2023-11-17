#ifndef QUERY_MANAGEMENT_H
#define QUERY_MANAGEMENT_H

#include <iostream>
#include <BinarySearchTree.h>
#include <BinaryTreeIterator.h>

namespace nemchinov
{
  class CommandManagement
  {
    using dictionary = BinarySearchTree< int, std::string >;
  public:
    static void putInDict(BinarySearchTree< std::string, BinarySearchTree< int, std::string>> &, std::string);
    explicit CommandManagement(BinarySearchTree< std::string, BinarySearchTree< int, std::string > > &,
            std::istream &in = std::cin, std::ostream &out = std::cout);

  private:
    void commandCall(BinarySearchTree< std::string, BinarySearchTree< int, std::string > > &, std::string &);
    static std::string getWord(std::string &);

    struct Print
    {
    public:
      Print() = delete;
      Print(std::istream &, std::ostream &);
      void operator()(BinarySearchTree< std::string, BinarySearchTree< int, std::string > > &, const std::string) const;
    private:
      std::istream &in_;
      std::ostream &out_;
    };

    struct Complement
    {
    public:
      void operator()(BinarySearchTree< int, std::string > &,
                      BinarySearchTree< int, std::string > &,
                      BinarySearchTree< int, std::string > &);
    };

    struct Intersect
    {
    public:
      void operator()(BinarySearchTree< int, std::string > &,
                      BinarySearchTree< int, std::string > &,
                      BinarySearchTree< int, std::string > &);
    };

    struct DictUnion
    {
    public:
      void operator()(BinarySearchTree< int, std::string > &,
                      BinarySearchTree< int, std::string > &,
                      BinarySearchTree< int, std::string > &);
    };

    Print print;
    Complement complement{};
    Intersect intersect{};
    DictUnion dictUnion{};
    std::istream &in_;
    std::ostream &out_;
  };
}

#endif
