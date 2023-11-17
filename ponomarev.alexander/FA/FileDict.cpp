#include "FilesDict.h"

#include <algorithm>
#include <iterator>
#include <iomanip>
#include "IOFormatGuard.h"

namespace ponomarev {
  FileDict::FileDict(std::string logFile):
    logFile_(logFile)
  {
    if (!logFile_.is_open()) {
      logFile_.close();
      logFile_.open(logFile, std::ios::out | std::ios::trunc);
      return;
    }

    while (!logFile_.eof()) {
      logFile_.clear();

      std::string filePath;
      logFile_ >> filePath;
      ImageClass clazz = ImageClass::Undefined();
      logFile_ >> clazz;

      if (logFile_) {
        files_.insert({ filePath, clazz });
      }
    }
    logFile_.close();
    logFile_.open(logFile, std::ios::out | std::ios::trunc);
  }
  FileDict::~FileDict()
  {
    auto it = files_.begin();
    while (it != files_.end()) {
      logFile_ << it->first << ' ' << it->second.toString();
      it++;
      if (it != files_.end()) {
        logFile_ << ' ';
      }
    }
    logFile_.close();
  }
  void FileDict::insert(std::string file, ImageClass clazz)
  {
    files_[file] = clazz;
  }
  const ImageClass &FileDict::get(std::string file)
  {
    return files_.at(file);
  }
  void FileDict::printLogs(std::ostream& os)
  {
    IOFormatGuard guard(os);
    for (auto it = files_.begin(); it != files_.end(); it++) {
      os <<  "| " << std::right << std::setw(20) << std::setfill(' ') << it->first << ": ";
      os << std::right << std::setw(10) << std::setfill(' ') << it->second.toString() << " |";
      os << '\n';
    }
  }
  void FileDict::clear()
  {
    files_.clear();
  }
  bool FileDict::isEmpty()
  {
    return files_.isEmpty();
  }
}
