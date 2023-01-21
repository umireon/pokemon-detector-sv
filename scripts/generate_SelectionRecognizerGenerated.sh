#!/bin/bash
set -euo pipefail
normalized_height=32
binary_threshold=200
rows=32
(
    printf '%s\n' '#include <vector>'
    printf '%s\n' '#include <opencv2/opencv.hpp>'
    printf '%s\n' '#include "SelectionRecognizer.h"'
    printf '%s\n' "const int SelectionRecognizer::BINARY_THRESHOLD = $binary_threshold;"
    printf '%s\n' "const int SelectionRecognizer::NORMALIZED_HEIGHT = $normalized_height;"

    printf '%s' 'const std::vector<int> SelectionRecognizer::SELECTION_INDEX = {'
    for name in $(basename -s .png ./train/SelectionRecognizer/*.png)
    do
      printf '%s' ${name: -1},
    done
    printf '%s\n' '};'

    printf '%s' 'static const std::vector<std::vector<uchar>> DATA = '
    ./cmake-build-debug/TemplateMatchTrainer $normalized_height $binary_threshold ./train/SelectionRecognizer/*.png
    printf '%s\n' ';'

    printf '%s' 'const std::vector<cv::Mat> SelectionRecognizer::SELECTION_TEMPLATES = {'
    for i in $(seq 0 $(($(ls ./train/SelectionRecognizer/*.png | wc -l) - 1)))
    do
    printf '%s' "cv::Mat(DATA[$i].size()/$rows,$rows,CV_8U,(void*)DATA[$i].data()),";
    done
    printf '%s\n' '};'
) > ./SelectionRecognizerGenerated.cpp
clang-format -i ./SelectionRecognizerGenerated.cpp
