#include "SceneDetector.h"

pokemon_detector_sv_scene SceneDetector::detectScene(const cv::Mat &screenHSV) {
  if (isSelectPokemonScreen(screenHSV)) {
    return POKEMON_DETECTOR_SV_SCENE_SELECT_POKEMON;
  } else if (isBlackTransition(screenHSV)) {
    return POKEMON_DETECTOR_SV_SCENE_BLACK_TRANSITION;
  } else if (isSelectMyTeamScreen(screenHSV)) {
    return POKEMON_DETECTOR_SV_SCENE_SELECT_MY_TEAM;
  } else {
    return POKEMON_DETECTOR_SV_SCENE_UNDEFINED;
  }
}

bool SceneDetector::isSelectPokemonScreen(const cv::Mat &screenHSV) {
  return predictByHueHist(screenHSV, classifierLobbyMySelect) &&
         predictByHueHist(screenHSV, classifierLobbyOpponentSelect);
}

bool SceneDetector::isBlackTransition(const cv::Mat &screenHSV) {
  return predictByHueHist(screenHSV, classifierBlackTransition);
}

bool SceneDetector::isSelectMyTeamScreen(const cv::Mat &screenHSV) {
  return predictByHueHist(screenHSV, classifierSelectMyTeam);
}

void SceneDetector::calcHistHue(const cv::Mat &areaHSV, cv::Mat &hist,
                                int channel, int nBins) {
  const int channels[]{channel};
  const int histSize[]{nBins};
  const float hranges[] = {0, channel == 0 ? 180.0f : 256.0f};
  const float *ranges[]{hranges};
  cv::calcHist(&areaHSV, 1, channels, cv::Mat(), hist, 1, histSize, ranges);
}

bool SceneDetector::predictByHueHist(
    const cv::Mat &screenHSV,
    const pokemon_detector_sv_hist_classifier &classifier) {
  const cv::Range rowRange(classifier.ranges_row[0], classifier.ranges_row[1]),
      colRange(classifier.ranges_col[0], classifier.ranges_col[1]);

  const cv::Mat areaHSV = screenHSV(rowRange, colRange);
  cv::Mat hist;
  calcHistHue(areaHSV, hist, classifier.hist_channel, classifier.hist_bins);

  double maxVal;
  cv::Point maxIdx;
  cv::minMaxLoc(hist, nullptr, &maxVal, nullptr, &maxIdx);
  return maxVal > areaHSV.total() * classifier.hist_ratio &&
         maxIdx.y == classifier.hist_max_index;
}
