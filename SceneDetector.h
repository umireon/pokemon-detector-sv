#pragma once

#include "pokemon-detector-sv.h"
#include <opencv2/opencv.hpp>

class SceneDetector {
public:
  const pokemon_detector_sv_hist_classifier classifierLobbyMySelect;
  const pokemon_detector_sv_hist_classifier classifierLobbyOpponentSelect;
  const pokemon_detector_sv_hist_classifier classifierBlackTransition;

  SceneDetector(
      const pokemon_detector_sv_hist_classifier classifierLobbyMySelect,
      const pokemon_detector_sv_hist_classifier classifierLobbyOpponentSelect,
      const pokemon_detector_sv_hist_classifier classifierBlackTransition)
      : classifierLobbyMySelect(classifierLobbyMySelect),
        classifierLobbyOpponentSelect(classifierLobbyOpponentSelect),
        classifierBlackTransition(classifierBlackTransition) {}

  pokemon_detector_sv_scene detectScene(cv::Mat &screenHSV);
  bool isSelectScreen(cv::Mat &screenHSV);
  bool isBlackTransition(cv::Mat &screenHSV);
  void calcHistHue(const cv::Mat &areaHSV, cv::Mat &hist, int channel,
                   int nBins);
  bool predictByHueHist(const cv::Mat &screenHSV,
                        const pokemon_detector_sv_hist_classifier &classifier);
};
