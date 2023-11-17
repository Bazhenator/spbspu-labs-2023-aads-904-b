#ifndef COMMANDS_H
#define COMMANDS_H

#include <string>
#include <functional>
#include <iosfwd>
#include <BinarySearchTree.h>
#include "ImageClassificator.h"
#include "FilesDict.h"

namespace ponomarev {
  class Commands {
    public:
      Commands(std::istream &is, std::ostream &os, std::string logFile);

      void doHelp();
      void doFeed();
      void doFeedPercentage();
      void doShowImage();
      void doViewSession();
      void doClearSession();
      void doShowLast();
      void doAssessLast();
      void doSlideShow();
    private:
      const std::string MODEL_FILE = "src/cats_vs_dogs_model.yml.gz";

      AVLTree< std::string, std::string > descriptions_;
      std::string lastImage_;
      ImageClassificator model_;
      FileDict log_;
      std::istream &is_;
      std::ostream &os_;
  };
  void showClassifiedImage(std::string path, ImageClass clazz);
}

#endif
