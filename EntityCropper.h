#pragma once

#include <array>

#include <opencv2/opencv.hpp>

class EntityCropper {
public:
  std::array<cv::Mat, 6> imagesBGR;
  std::array<cv::Mat, 6> imagesBGRA;
  std::array<cv::Mat, 6> masks;
  const std::array<int, 2> rangeCol;
  const std::array<std::array<int, 2>, 6> rangesRow;
  cv::Point seedPoint;

  EntityCropper(const std::array<int, 2> rangeCol,
                const std::array<std::array<int, 2>, 6> rangesRow)
      : rangeCol(rangeCol), rangesRow(rangesRow), seedPoint(0, 0) {}
  void crop(const cv::Mat &screenBGRA);
};