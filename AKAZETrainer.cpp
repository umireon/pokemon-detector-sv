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
  const std::vector<std::string> paths(&argv[3], &argv[argc]);
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
      cv::resize(image, scaledImage, cv::Size(image.cols * height / image.rows, height));
    }
    cv::Mat imageBGR, mask = extractMaskFromBGRA(scaledImage);
    cv::cvtColor(scaledImage, imageBGR, cv::COLOR_BGRA2BGR);

    std::vector<cv::KeyPoint> keyPoints;
    cv::Mat descriptors;
    algorithm->detectAndCompute(imageBGR, mask, keyPoints, descriptors);
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
