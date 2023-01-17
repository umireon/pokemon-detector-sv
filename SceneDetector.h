#pragma once

#include "pokemon-detector-sv.h"
#include <opencv2/opencv.hpp>

class SceneDetector {
public:
  enum State { STATE_UNKNOWN, STATE_SELECT, STATE_MATCH_START, STATE_MATCH };

  const pokemon_detector_sv_config &config;
  State currentState;

  SceneDetector(const pokemon_detector_sv_config &config) : config(config) {}
  pokemon_detector_sv_scene detectScene(cv::Mat &screenHSV);
  bool isSelectScreen(cv::Mat &screenHSV);
  bool isBlackTransition(cv::Mat &screenHSV);
  void calcHistHue(const cv::Mat &areaHSV, cv::Mat &hist, int channel,
                   int nBins);
  bool predictByHueHist(const cv::Mat &screenHSV,
                        const pokemon_detector_sv_hist_classifier &classifier);
};
