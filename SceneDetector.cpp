#include "SceneDetector.h"

pokemon_detector_sv_scene SceneDetector::detectScene(cv::Mat &screen) {
  if (isSelectScreen(screen)) {
    return POKEMON_DETECTOR_SV_SCENE_SELECT;
  } else {
    return POKEMON_DETECTOR_SV_SCENE_UNDEFINED;
  }
}

bool SceneDetector::isSelectScreen(cv::Mat &screen) {
  auto ranges1 = config.selecting_area1_ranges;
  const cv::Range colRange1(ranges1[0][0], ranges1[0][1]),
      rowRange1(ranges1[1][0], ranges1[1][1]);
  cv::Mat area1 = screen(rowRange1, colRange1);
  cv::cvtColor(area1, area1, cv::COLOR_BGR2HSV);
  bool predict1 = predictByHueHist(area1, config.selecting_area1_hist_bins,
                                   config.selecting_area1_hist_ratio,
                                   config.selecting_area1_hist_max_index);

  auto ranges2 = config.selecting_area2_ranges;
  const cv::Range colRange2(ranges2[0][0], ranges2[0][1]),
      rowRange2(ranges2[1][0], ranges2[1][1]);
  cv::Mat area2 = screen(rowRange1, colRange1);
  cv::cvtColor(area2, area2, cv::COLOR_BGR2HSV);
  bool predict2 = predictByHueHist(area2, config.selecting_area1_hist_bins,
                                   config.selecting_area1_hist_ratio,
                                   config.selecting_area1_hist_max_index);

  return predict1 && predict2;
}

void SceneDetector::calcHueHist(const cv::Mat &input, cv::Mat &hist,
                                int nBins) {
  const int channels[]{0};
  const int histSize[]{nBins};
  const float hranges[] = {0, 180};
  const float *ranges[]{hranges};
  cv::calcHist(&input, 1, channels, cv::Mat(), hist, 1, histSize, ranges);
}

bool SceneDetector::predictByHueHist(const cv::Mat &area, int nBins,
                                     double ratio, int maxIndex) {
  cv::Mat hist;
  calcHueHist(area, hist, nBins);

  double maxVal;
  cv::Point maxIdx;
  cv::minMaxLoc(hist, nullptr, &maxVal, nullptr, &maxIdx);
  return maxVal > area.total() * ratio && maxIdx.y == maxIndex;
}
