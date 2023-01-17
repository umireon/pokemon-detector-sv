#include "SceneDetector.h"

pokemon_detector_sv_scene SceneDetector::detectScene(cv::Mat &screenHSV) {
  if (isSelectScreen(screenHSV)) {
    return POKEMON_DETECTOR_SV_SCENE_SELECT;
  } else if (isBlackTransition(screenHSV)) {
    return POKEMON_DETECTOR_SV_SCENE_BLACK_TRANSITION;
  } else {
    return POKEMON_DETECTOR_SV_SCENE_UNDEFINED;
  }
}

bool SceneDetector::isSelectScreen(cv::Mat &screenHSV) {
  return predictByHueHist(screenHSV, config.classifier_lobby_my_select) &&
         predictByHueHist(screenHSV, config.classifier_lobby_opponent_select);
}

bool SceneDetector::isBlackTransition(cv::Mat &screenHSV) {
  return predictByHueHist(screenHSV, config.classifier_black_transition);
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