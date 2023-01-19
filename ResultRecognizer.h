#pragma once

#include <opencv2/opencv.hpp>

class ResultRecognizer {
public:
  enum ResultType { UNKNOWN, WIN, LOSE };

  int nBins;
  double winRatio;
  int winMaxIndex;
  double loseRatio;
  int loseMaxIndex;

  ResultRecognizer(int nBins, double winRatio, double loseRatio,
                   int winMaxIndex, int loseMaxIndex)
      : nBins(nBins), winRatio(winRatio), loseRatio(loseRatio),
        winMaxIndex(winMaxIndex), loseMaxIndex(loseMaxIndex) {}

  ResultType recognizeResult(const cv::Mat &imageBGR);
  void calcHistHue(const cv::Mat &areaHSV, cv::Mat &hist);
};