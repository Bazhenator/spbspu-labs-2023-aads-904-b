#include "Commands.h"
#include <iostream>
#include <iomanip>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/utils/logger.hpp>
#include <limits>
#include "IOFormatGuard.h"

namespace ponomarev {
  Commands::Commands(std::istream& is, std::ostream& os, std::string logFile) :
    descriptions_({
      {"help", "lists all commands and describes them."},
      {"feed", ":[image_file], classfies image as dog or a cat."},
      {"show_image", ":[image_file], shows image in new window (can be saves by typing 's')"},
      {"view_session", "lists all processed images with classified class (or not)"},
      {"clear_session", "clears all history of processed images"},
      {"show_last", "shows last processed image"},
      {"assess_last", ":[Dog/Cat], human assessment of neural network (it will help us in future)"},
      {"feed_percentage", ":[image_file], classfies image as dog or a cat, but outputs percentage of prediction"},
      {"quit", "to quit the programm"},
      {"slide_show", "shows all images with class"}
      }),
    lastImage_(""),
    model_(MODEL_FILE),
    log_(logFile),
    is_(is),
    os_(os)
  {
    cv::utils::logging::setLogLevel(cv::utils::logging::LogLevel::LOG_LEVEL_SILENT);
  }
  void Commands::doHelp()
  {
    IOFormatGuard guard(os_);
    for (auto it = descriptions_.begin(); it != descriptions_.end(); it++) {
      os_ << std::right << std::setw(15) << it->first << " - " << it->second << '\n';
    }
  }
  void Commands::doFeed()
  {
    std::string image;
    std::getline(is_ >> std::ws, image);
    ImageClass clazz;
    try {
      clazz = model_.getImageClass(image);
    } catch (const std::runtime_error &e) {
      os_ << "Error: " << e.what() << '\n';
      return;
    }
    os_ << "Image classified as: " << clazz.toString() << '\n';
    log_.insert(image, clazz);
    lastImage_ = image;
  }
  void Commands::doShowImage()
  {
    std::string imageFilePath;
    std::getline(is_ >> std::ws, imageFilePath);
    cv::Mat img = cv::imread(imageFilePath);
    if (img.empty()) {
      os_ << "Error: Image can not be read.\n";
      return;
    }
    os_ << "Press any key to close the window or 's' to save image in logs.\n";

    const std::string winName = "show_image";
    cv::imshow(winName, img);
    int pressedKey = cv::waitKey(0);
    if (pressedKey == 's') {
      log_.insert(imageFilePath, ImageClass::Undefined());
    }
    cv::destroyWindow(winName);
  }
  void Commands::doViewSession()
  {
    if (log_.isEmpty()) {
      os_ << "Session is empty.\n";
      return;
    }
    log_.printLogs(os_);
  }
  void Commands::doFeedPercentage()
  {
    std::string image;
    std::getline(is_ >> std::ws, image);
    std::vector< double > result;
    try {
      result = model_.getOutputVector(image);
    } catch (const std::runtime_error &e) {
      os_ << "Error: " << e.what() << '\n';
      return;
    }
    IOFormatGuard guard(os_);
    ImageClass imgClass = model_.getImageClass(image);
    os_ << "Image classified as: " << imgClass.toString() << " with percentage: "
      << std::fixed << std::setprecision(2) << *std::max_element(result.begin(), result.end()) << '\n';
    log_.insert(image, imgClass);
    lastImage_ = image;
  }
  void Commands::doClearSession()
  {
    log_.clear();
  }
  void Commands::doShowLast()
  {
    if (lastImage_.empty()) {
      os_ << "There are no processed image yet.\n";
      return;
    }

    showClassifiedImage(lastImage_, log_.get(lastImage_));
  }
  void Commands::doAssessLast()
  {
    if (lastImage_.empty()) {
      os_ << "There are no processed image yet.\n";
      return;
    }

    std::string parameter;
    std::getline(is_ >> std::ws, parameter);
    ImageClass lastImageClass = log_.get(lastImage_);
    if (!parameter.compare(lastImageClass.toString())) {
      os_ << "Image was classified right!\nThanks for your support.\n";
    } else {
      os_ << "Image was classified wrong(\nThanks for your support.\n";
    }
  }
  void Commands::doSlideShow()
  {
    log_.traverse(showClassifiedImage);
  }
  void showClassifiedImage(std::string path, ImageClass clazz)
  {
    cv::Mat img = cv::imread(path);
    if (img.empty()) {
      std::cerr << "Error: Image can not be read.\n";
      return;
    }

    cv::Point place(1, 50);
    cv::putText(img, clazz.toString(), place,
      cv::FONT_HERSHEY_PLAIN, 3.0,
      cv::Scalar(180, 105, 255), 5
    );
    cv::imshow("Image", img);
    cv::waitKey(3000);
    cv::destroyWindow("Image");
  }
}
