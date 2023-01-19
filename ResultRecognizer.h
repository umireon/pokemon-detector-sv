#pragma once

#include <opencv2/opencv.hpp>

#include "pokemon-detector-sv.h"

class ResultRecognizer {
public:
  int nBins;
  double loseRatio;
  int loseMaxIndex;
  double winRatio;
  int winMaxIndex;

  ResultRecognizer(int nBins, int loseMaxIndex, double loseRatio,
                   int winMaxIndex, double winRatio)
      : nBins(nBins), loseRatio(loseRatio), loseMaxIndex(loseMaxIndex),
        winRatio(winRatio), winMaxIndex(winMaxIndex) {}

  pokemon_detector_sv_result recognizeResult(const cv::Mat &imageBGR);
  void calcHistHue(const cv::Mat &areaHSV, cv::Mat &hist);
};