#include <iostream>

#include <opencv2/core/utils/logger.hpp>
#include <opencv2/opencv.hpp>

static cv::Mat extractMaskFromBGRA(const cv::Mat &bgra) {
  cv::Mat mask(bgra.rows, bgra.cols, CV_8U);
  for (int y = 0; y < bgra.rows; y++) {
    for (int x = 0; x < bgra.cols; x++) {
      mask.at<uchar>(y, x) = bgra.at<cv::Vec4b>(y, x)[3];
    }
  }
  return mask;
}

int main(int argc, const char **argv) {
  cv::utils::logging::setLogLevel(cv::utils::logging::LogLevel::LOG_LEVEL_SILENT);

  const auto descriptorSize = std::stoi(argv[1]);
  const auto height = std::stoi(argv[2]);
  const int threshold = std::stoi(argv[3]);
  const std::vector<std::string> paths(&argv[4], &argv[argc]);
  auto algorithm =
      cv::AKAZE::create(cv::AKAZE::DESCRIPTOR_MLDB, descriptorSize);

  std::vector<std::vector<cv::KeyPoint>> keyPointsVector;
  std::vector<cv::Mat> descriptorsVector;
  for (auto path : paths) {
    const cv::Mat image = cv::imread(path, cv::IMREAD_UNCHANGED);
    cv::Mat scaledImage;
    if (image.rows == height) {
      scaledImage = image;
    } else {
      cv::resize(image, scaledImage, cv::Size(image.rows * height / image.cols, height));
    }
    cv::Mat targetImage, mask = extractMaskFromBGRA(scaledImage);
    if (threshold == -1) {
      cv::cvtColor(scaledImage, targetImage, cv::COLOR_BGRA2BGR);
    } else {
      cv::cvtColor(scaledImage, targetImage, cv::COLOR_BGRA2GRAY);
      cv::threshold(targetImage, targetImage, threshold, 255, cv::THRESH_BINARY);
    }

    std::vector<cv::KeyPoint> keyPoints;
    cv::Mat descriptors;
    algorithm->detectAndCompute(targetImage, mask, keyPoints, descriptors);
    descriptorsVector.push_back(descriptors);
  }

  std::cout << "{";
  for (auto &descriptors : descriptorsVector) {
    std::cout << "{";
    for (int i = 0; i < descriptors.total(); i++) {
      std::cout << (int)descriptors.at<uchar>(i) << ",";
    }
    std::cout << "},";
  }
  std::cout << "}";

  return 0;
}
