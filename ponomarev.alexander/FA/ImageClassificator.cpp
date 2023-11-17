#include "ImageClassificator.h"
#include <iostream>

ponomarev::ImageClass::ImageClass():
  ImageClass("Undefined")
{}
ponomarev::ImageClass::ImageClass(std::string val):
  val_(val)
{}
ponomarev::ImageClass ponomarev::ImageClass::Cat()
{
  return ImageClass("Cat");
}
ponomarev::ImageClass ponomarev::ImageClass::Dog()
{
  return ImageClass("Dog");
}
ponomarev::ImageClass ponomarev::ImageClass::Undefined()
{
  return ImageClass("Undefined");
}
std::string ponomarev::ImageClass::toString()
{
  return val_;
}
std::istream& ponomarev::operator>>(std::istream& is, ImageClass& cls)
{
  std::istream::sentry sentry(is);
  if (!sentry) {
    return is;
  }

  std::string clazz;
  is >> clazz;
  if (!clazz.compare("Cat")) {
    cls = ImageClass::Cat();
  } else if (!clazz.compare("Dog")) {
    cls = ImageClass::Dog();
  } else if (!clazz.compare("Undefined")) {
    cls = ImageClass::Undefined();
  } else {
    is.setstate(std::ios_base::failbit);
  }
  return is;
}

ponomarev::ImageClassificator::ImageClassificator(std::string modelFilepath)
{
  try {
    model_ = cv::ml::ANN_MLP::load(modelFilepath);
    net_ = cv::dnn::readNetFromCaffe(NET_TXT, NET_BIN);
  } catch (const cv::Exception &e) {
    std::cerr << "Error: directory should contain: " << modelFilepath << ", " << NET_BIN << ", " << NET_TXT << '\n';
    throw;
  }
}
ponomarev::ImageClass ponomarev::ImageClassificator::getImageClass(std::string imageFilePath)
{
  cv::Mat img = getPreprocessed(imageFilePath);
  cv::Mat resVector = classify(img);
  int ansrIndx[] = {0, 0};
  cv::minMaxIdx(resVector, NULL, NULL, NULL, ansrIndx);
  return RESULT_CLASSES_INDX.at(ansrIndx[0] + ansrIndx[1]);
}
std::vector< double > ponomarev::ImageClassificator::getOutputVector(std::string imageFilePath)
{
  cv::Mat img = getPreprocessed(imageFilePath);
  cv::Mat resVector = classify(img);
  return std::vector< double >(resVector);
}
cv::Mat ponomarev::ImageClassificator::getPreprocessed(std::string imgFilePath)
{
  cv::Mat img = cv::imread(imgFilePath);
  if (img.empty()) {
    throw std::runtime_error("Image can not be read.");
  }

  net_.setInput(cv::dnn::blobFromImage(img, 1, cv::Size(227, 227), cv::Scalar::all(127), false));
  img = net_.forward("pool10");
  return img.reshape(1, 1).clone();
}
cv::Mat ponomarev::ImageClassificator::classify(cv::Mat img)
{
  cv::Mat result;
  model_->predict(img, result);
  cv::Mat normed;
  cv::normalize(result, normed, 1.0, 0.0, cv::NORM_L1);
  return normed;
}
