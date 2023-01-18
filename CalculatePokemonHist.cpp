#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

static void calcHist(const cv::Mat &input, cv::Mat &mask, cv::Mat &hist) {
  const int channels[]{0};
  const int histSize[]{32};
  const float hranges[] = {0, 180};
  const float *ranges[]{hranges};
  cv::calcHist(&input, 1, channels, mask, hist, 1, histSize, ranges);
}

int main(int argc, const char **argv) {
  const std::vector<std::string> screenPaths(&argv[1], &argv[argc]);

  for (std::string imagePath : screenPaths) {
    const cv::Mat pokemonBGRA = cv::imread(imagePath,cv::IMREAD_UNCHANGED);
    cv::Mat pokemonBGR, pokemonHSV, mask(pokemonBGRA.cols, pokemonBGRA.rows, CV_8UC1);
    cv::cvtColor(pokemonBGRA, pokemonBGR, cv::COLOR_BGRA2BGR);
    cv::cvtColor(pokemonBGR, pokemonHSV, cv::COLOR_BGR2HSV);
    for (int x = 0; x < pokemonBGRA.cols; x++) {
      for (int y = 0; y < pokemonBGRA.rows; y++) {
        mask.at<uchar>(y, x) = pokemonBGRA.at<cv::Vec4b>(y, x)[3];
      }
    }

    cv::Mat hist;
    calcHist(pokemonHSV, mask, hist);
    std::cout << hist;
  }
  return 0;
}