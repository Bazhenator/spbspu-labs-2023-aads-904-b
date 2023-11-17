#ifndef FILE_DICT_H
#define FILE_DICT_H

#include <string>
#include <fstream>
#include <functional>
#include <iosfwd>
#include <BinarySearchTree.h>
#include "ImageClassificator.h"

namespace ponomarev {
  class FileDict {
    public:
      explicit FileDict(std::string logFile = "logs.txt");
      ~FileDict();

      void insert(std::string file, ImageClass clazz);
      const ImageClass &get(std::string file);
      void printLogs(std::ostream &os);
      void clear();
      template< typename Func >
      void traverse(Func functor);
      bool isEmpty();
    private:
      AVLTree< std::string, ImageClass > files_;
      std::fstream logFile_;
  };
  template< typename Func >
  void FileDict::traverse(Func functor)
  {
    for (auto it = files_.begin(); it != files_.end(); it++) {
      functor(it->first, it->second);
    }
  }
}

#endif
