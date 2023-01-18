#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

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

  const std::vector<std::string> paths(&argv[1], &argv[argc]);
  const auto numOfData = paths.size();
  auto algorithm = cv::AKAZE::create(cv::AKAZE::DESCRIPTOR_MLDB, 16);

  std::vector<std::vector<cv::KeyPoint>> keyPointsVector;
  std::vector<cv::Mat> descriptorsVector;
  for (auto path : paths) {
    const cv::Mat imageBGRA = cv::imread(path, cv::IMREAD_UNCHANGED);
    cv::Mat imageBGR, mask = extractMaskFromBGRA(imageBGRA);
    cv::cvtColor(imageBGRA, imageBGR, cv::COLOR_BGRA2BGR);

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
