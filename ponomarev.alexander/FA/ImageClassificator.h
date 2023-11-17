#ifndef IMAGE_CLASSIFICATOR_H
#define IMAGE_CLASSIFICATOR_H

#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <utility>
#include <vector>
#include <iosfwd>
#include <BinarySearchTree.h>

namespace ponomarev {
  class ImageClass {
    public:
      ImageClass();
      static ImageClass Cat();
      static ImageClass Dog();
      static ImageClass Undefined();

      std::string toString();

      friend std::istream &operator>>(std::istream &is, ImageClass &cls);
    private:
      std::string val_;

      explicit ImageClass(std::string val);
  };
  std::istream &operator>>(std::istream &is, ImageClass &cls);
  class ImageClassificator {
    public:
      ImageClassificator(std::string modelFilePath);

      ImageClass getImageClass(std::string imageFilePath);
      std::vector< double > getOutputVector(std::string imageFilePath);
    private:
      const std::string NET_BIN = "src/squeezenet_v1.1.caffemodel";
      const std::string NET_TXT = "src/squeezenet_v1.1.prototxt";
      AVLTree< int, ImageClass > RESULT_CLASSES_INDX = {
          {0, ImageClass::Cat()},
          {1, ImageClass::Dog()}
        };

      cv::Ptr< cv::ml::ANN_MLP > model_;
      cv::dnn::Net net_;

      cv::Mat getPreprocessed(std::string imgFilePath);
      cv::Mat classify(cv::Mat img);
  };
}

#endif
