#include <iostream>

#include "EntityCropper.h"

int main(int argc, const char **argv) {
  const std::vector<std::string> paths(&argv[1], &argv[argc]);
  const std::array<int, 2> rangeCol{483, 697};
  const std::vector<std::array<int, 2>> rangesRow{{958, 1039}};

  for (auto path : paths) {
    cv::Mat screenBGR = cv::imread(path, cv::IMREAD_COLOR), screenBGRA;
    cv::cvtColor(screenBGR, screenBGRA, cv::COLOR_BGR2BGRA);
    EntityCropper cropper(rangeCol, rangesRow);
    cropper.crop(screenBGRA);
    for (int i = 0; i < cropper.imagesBGRA.size(); i++) {
      cv::imwrite(path + "-" + std::to_string(i) + ".png",
                  cropper.imagesBGRA[i]);
    }
  }
}