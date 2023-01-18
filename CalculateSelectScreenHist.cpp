#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

static void calcHist(const cv::Mat &input, cv::Mat &hist) {
  const int channels[]{2};
  const int histSize[]{8};
  const float hranges[] = {0, 256};
  const float *ranges[]{hranges};
  cv::calcHist(&input, 1, channels, cv::Mat(), hist, 1, histSize, ranges);
}

int main(int argc, const char **argv) {
  const std::vector<std::string> screenPaths(&argv[1], &argv[argc]);
  const cv::Range colRange(749, 1179), rowRange(237, 859);

  for (std::string imagePath : screenPaths) {
    const cv::Mat screen = cv::imread(imagePath);
    const cv::Mat myPokemons = screen(rowRange, colRange);
    cv::Mat myPokemonsHSV;
    cv::cvtColor(myPokemons, myPokemonsHSV, cv::COLOR_BGR2HSV);
    cv::Mat hist;
    calcHist(myPokemonsHSV, hist);
    double maxVal;
    cv::Point maxIdx;
    cv::minMaxLoc(hist, nullptr, &maxVal, nullptr, &maxIdx);
    if (maxVal > myPokemonsHSV.total() / 2 && maxIdx.y == 17) {
      std::cout << imagePath << " true ";
    } else {
      std::cout << imagePath << " false ";
    }
    std::cout << maxVal << " " << maxIdx.y << " " << myPokemonsHSV.total() <<std::endl;
  }
  return 0;
}