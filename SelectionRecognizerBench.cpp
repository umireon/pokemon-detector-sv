#include <array>
#include <iostream>
#include <vector>

#include <opencv2/opencv.hpp>

#include "SelectionRecognizer.h"

int main(int argc, const char **argv) {
  const std::vector<std::string> paths(&argv[1], &argv[argc]);
  auto path = paths[0];
  cv::Mat imageBGRA = cv::imread(path, cv::IMREAD_UNCHANGED);
  cv::Mat imageBGR;
  cv::cvtColor(imageBGRA, imageBGR, cv::COLOR_BGRA2BGR);

  SelectionRecognizer recognizer;
  std::cout << recognizer.recognizeSelection(imageBGR);
}