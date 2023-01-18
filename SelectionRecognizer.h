#pragma once

#include <string>
#include <vector>

#include <opencv2/opencv.hpp>

class SelectionRecognizer {
public:
  int recognizeSelection(const cv::Mat &imageBGR);

private:
  static const std::vector<int> SELECTION_INDEX;
  static const std::vector<cv::Mat> SELECTION_DESCRIPTORS;
};