#pragma once

#include <string>
#include <vector>

#include <opencv2/opencv.hpp>

class SelectionRecognizer {
public:
  std::string recognizeSelection(const cv::Mat &imageBGR);

private:
  static const std::vector<std::string> SELECTION_IDS;
  static const std::vector<cv::Mat> SELECTION_DESCRIPTORS;
};