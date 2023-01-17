#include "SceneDetector.h"

void SceneDetector::calcHueHist(const cv::Mat &input, cv::Mat &hist,
                                int nBins) {
  const int channels[]{0};
  const int histSize[]{nBins};
  const float hranges[] = {0, 180};
  const float *ranges[]{hranges};
  cv::calcHist(&input, 1, channels, cv::Mat(), hist, 1, histSize, ranges);
}

bool SceneDetector::isSelectingScreen(cv::Mat &screen) {
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
