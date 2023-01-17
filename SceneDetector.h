#pragma once

#include "pokemon-detector-sv.h"
#include <opencv2/opencv.hpp>

class SceneDetector {
public:
  SceneDetector(pokemon_detector_sv_config &config) : config(config) {}
  pokemon_detector_sv_scene detectScene(cv::Mat &screenHSV);
  bool isSelectScreen(cv::Mat &screenHSV);
  void calcHistHue(const cv::Mat &areaHSV, cv::Mat &hist, int nBins);
  bool predictByHueHist(const cv::Mat &screenHSV,
                        const pokemon_detector_sv_hue_classifier &classifier);

private:
  const pokemon_detector_sv_config &config;
};
