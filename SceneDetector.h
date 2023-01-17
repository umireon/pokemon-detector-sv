#pragma once

#include "pokemon-detector-sv.h"
#include <opencv2/opencv.hpp>

class SceneDetector {
public:
  SceneDetector(pokemon_detector_sv_config config) : config(config) {}
  bool isSelectingScreen(cv::Mat &screen);
  void calcHueHist(const cv::Mat &input, cv::Mat &hist, int nBins);

private:
  pokemon_detector_sv_config config;
};
