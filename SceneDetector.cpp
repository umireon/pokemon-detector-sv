#include "SceneDetector.h"

pokemon_detector_sv_scene SceneDetector::detectScene(cv::Mat &screen) {
  if (isSelectScreen(screen)) {
    return POKEMON_DETECTOR_SV_SCENE_SELECT;
  } else {
    return POKEMON_DETECTOR_SV_SCENE_UNDEFINED;
  }
}

bool SceneDetector::isSelectScreen(cv::Mat &screenHSV) {
  return predictByHueHist(screenHSV, config.classifier_lobby_my_select) &&
         predictByHueHist(screenHSV, config.classifier_lobby_opponent_select);
}

void SceneDetector::calcHistHue(const cv::Mat &areaHSV, cv::Mat &hist,
                                int nBins) {
  const int channels[]{0};
  const int histSize[]{nBins};
  const float hranges[] = {0, 180};
  const float *ranges[]{hranges};
  cv::calcHist(&areaHSV, 1, channels, cv::Mat(), hist, 1, histSize, ranges);
}

bool SceneDetector::predictByHueHist(
    const cv::Mat &screenHSV,
    const pokemon_detector_sv_hue_classifier &classifier) {
  const cv::Range rowRange(classifier.ranges_row[0], classifier.ranges_row[1]),
      colRange(classifier.ranges_col[0], classifier.ranges_col[1]);

  const cv::Mat areaHSV = screenHSV(rowRange, colRange);
  cv::Mat hist;
  calcHistHue(areaHSV, hist, classifier.hist_bins);

  double maxVal;
  cv::Point maxIdx;
  cv::minMaxLoc(hist, nullptr, &maxVal, nullptr, &maxIdx);
  return maxVal > areaHSV.total() * classifier.hist_ratio &&
         maxIdx.y == classifier.hist_max_index;
}
