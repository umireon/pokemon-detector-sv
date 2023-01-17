#pragma once

#include "pokemon-detector-sv.h"
#include <opencv2/opencv.hpp>

class SceneDetector {
public:
  SceneDetector(pokemon_detector_sv_config &config) : config(config) {}
  pokemon_detector_sv_scene detectScene(cv::Mat &screen);
  bool isSelectScreen(cv::Mat &screen);
  void calcHueHist(const cv::Mat &input, cv::Mat &hist, int nBins);
  bool predictByHueHist(const cv::Mat &area, int nBins, double ratio,
                        int maxIndex);

private:
  pokemon_detector_sv_config &config;
};
