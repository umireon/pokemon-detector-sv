#include <iostream>

#include <opencv2/core/utils/logger.hpp>
#include <opencv2/opencv.hpp>

int main(int argc, const char **argv) {
  cv::utils::logging::setLogLevel(cv::utils::logging::LogLevel::LOG_LEVEL_SILENT);

  const auto height = std::stoi(argv[1]);
  const int threshold = std::stoi(argv[2]);
  const std::vector<std::string> paths(&argv[3], &argv[argc]);

  std::vector<cv::Mat> templates;
  for (auto path : paths) {
    const cv::Mat image = cv::imread(path, cv::IMREAD_UNCHANGED);
    cv::Mat scaledImage;
    if (image.rows == height) {
	    scaledImage = image;
    } else {
	    cv::resize(image, scaledImage, cv::Size(image.rows * height / image.cols, height));
    }
    cv::Mat targetImage;
    if (threshold == -1) {
      cv::cvtColor(scaledImage, targetImage, cv::COLOR_BGRA2BGR);
    } else {
      cv::cvtColor(scaledImage, targetImage, cv::COLOR_BGRA2GRAY);
      cv::threshold(targetImage, targetImage, threshold, 255, cv::THRESH_BINARY);
    }

    templates.push_back(targetImage);
  }

  std::cout << "{";
  for (auto &t : templates) {
    std::cout << "{";
    for (int i = 0; i < t.total(); i++) {
      std::cout << (int)t.at<uchar>(i) << ",";
    }
    std::cout << "},";
  }
  std::cout << "}";

  return 0;
}
