#include <opencv2/opencv.hpp>

#include "EntityCropper.h"

void EntityCropper::crop(const cv::Mat &screenBGRA) {
  for (int i = 0; i < 6; i++) {
    const cv::Range colRange(rangeCol[0], rangeCol[1]),
        rowRange(rangesRow[i][0], rangesRow[i][1]);
    imagesBGRA[i] = screenBGRA(rowRange, colRange);
    cv::cvtColor(imagesBGRA[i], imagesBGR[i], cv::COLOR_BGRA2BGR);

    cv::Mat mask;
    cv::floodFill(imagesBGR[i], mask, seedPoint, 0, 0, 0, 0,
                  cv::FLOODFILL_MASK_ONLY);
    cv::Range maskCropCol(1, mask.cols - 1), maskCropRow(1, mask.rows - 1);
    masks[i] = mask(maskCropCol, maskCropRow);

    for (int y = 0; y < imagesBGRA[i].rows; y++) {
      for (int x = 0; x < imagesBGRA[i].cols; x++) {
        imagesBGRA[i].at<cv::Vec4b>(y, x)[3] = masks[i].at<uchar>(y, x) * 255;
      }
    }
  }
}