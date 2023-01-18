#include <array>
#include <iostream>
#include <vector>

#include <opencv2/opencv.hpp>

#include "PokemonRecognizer.h"

int main(int argc, const char **argv) {
  const std::vector<std::string> paths(&argv[1], &argv[argc]);
  auto path = paths[0];
  cv::Mat imageBGRA = cv::imread(path, cv::IMREAD_UNCHANGED);
  cv::Mat imageBGR;
  cv::cvtColor(imageBGRA, imageBGR, cv::COLOR_BGRA2BGR);
  cv::Mat mask(imageBGRA.rows, imageBGRA.cols, CV_8UC1);
  for (int y = 0; y < imageBGRA.rows; y++) {
    for (int x = 0; x < imageBGRA.cols; x++) {
      mask.at<uchar>(y, x) = imageBGRA.at<cv::Vec4b>(y, x)[3];
    }
  }

  PokemonRecognizer recognizer;
  std::cout << recognizer.recognizePokemon(imageBGR, mask);
}