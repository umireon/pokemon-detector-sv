#include "SceneDetector.h"

pokemon_detector_sv_scene SceneDetector::detectScene(cv::Mat &screen) {
  if (isSelectScreen(screen)) {
    return POKEMON_DETECTOR_SV_SCENE_SELECT;
  } else {
    return POKEMON_DETECTOR_SV_SCENE_UNDEFINED;
  }
}

bool SceneDetector::isSelectScreen(cv::Mat &screen) {
  auto screenRanges = config.selecting_area_ranges;

  const cv::Range colRange(screenRanges[0][0], screenRanges[0][1]),
      rowRange(screenRanges[1][0], screenRanges[1][1]);
  cv::Mat selectingArea = screen(rowRange, colRange);
  cv::cvtColor(selectingArea, selectingArea, cv::COLOR_BGR2HSV);

  cv::Mat hist;
  calcHueHist(selectingArea, hist, config.selecting_area_hist_bins);

  double maxVal;
  cv::Point maxIdx;
  cv::minMaxLoc(hist, nullptr, &maxVal, nullptr, &maxIdx);
  return maxVal > selectingArea.total() * config.selecting_area_hist_ratio &&
         maxIdx.y == config.selecting_area_hist_max_index;
}

void SceneDetector::calcHueHist(const cv::Mat &input, cv::Mat &hist,
                                int nBins) {
  const int channels[]{0};
  const int histSize[]{nBins};
  const float hranges[] = {0, 180};
  const float *ranges[]{hranges};
  cv::calcHist(&input, 1, channels, cv::Mat(), hist, 1, histSize, ranges);
}
