#include "ResultRecognizer.h"

pokemon_detector_sv_result
ResultRecognizer::recognizeResult(const cv::Mat &areaBGR) {
  const cv::Mat areaHSV = areaBGR;
  cv::Mat hist;
  calcHistHue(areaHSV, hist);

  double maxVal;
  cv::Point maxIdx;
  cv::minMaxLoc(hist, nullptr, &maxVal, nullptr, &maxIdx);
  if (maxVal > areaHSV.total() * loseRatio && maxIdx.y == loseMaxIndex) {
    return POKEMON_DETECTOR_SV_RESULT_LOSE;
  } else if (maxVal > areaHSV.total() * winRatio && maxIdx.y == winMaxIndex) {
    return POKEMON_DETECTOR_SV_RESULT_WIN;
  } else {
    return POKEMON_DETECTOR_SV_RESULT_UNKNOWN;
  }
}

void ResultRecognizer::calcHistHue(const cv::Mat &areaHSV, cv::Mat &hist) {
  const int channels[]{0};
  const int histSize[]{nBins};
  const float hranges[] = {0, 180};
  const float *ranges[]{hranges};
  cv::calcHist(&areaHSV, 1, channels, cv::Mat(), hist, 1, histSize, ranges);
}
