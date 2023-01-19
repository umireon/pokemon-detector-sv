#include "EntityCropper.h"
#include <iostream>
int main(int argc, const char **argv) {
  const std::vector<std::string> paths(&argv[1], &argv[argc]);
  const std::array<int, 2> rangeCol{795, 827};
  const std::array<std::array<int, 2>, 6> rangesRow{
      154, 186, 271, 303, 388, 420, 503, 535, 619, 651, 735, 767};

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