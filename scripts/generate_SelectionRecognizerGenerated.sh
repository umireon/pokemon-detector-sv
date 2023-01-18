#!/bin/bash
set -euo pipefail
descriptor_size=64
normalized_height=98
rows=$(($descriptor_size / 8))
(
    printf '%s\n' '#include <vector>'
    printf '%s\n' '#include <opencv2/opencv.hpp>'
    printf '%s\n' '#include "SelectionRecognizer.h"'

    printf '%s' 'const std::vector<std::string> SelectionRecognizer::SELECTION_IDS = {'
    for name in $(basename -s .png ./train/SelectionRecognizer/*.png)
    do
    printf '%s' \"$name\",
    done
    printf '%s\n' '};'

    printf '%s' 'static const std::vector<std::vector<uchar>> DATA = '
    ./cmake-build-debug/AkAZETrainer $descriptor_size $normalized_height ./train/SelectionRecognizer/*.png
    printf '%s\n' ';'

    printf '%s' 'const std::vector<cv::Mat> SelectionRecognizer::SELECTION_DESCRIPTORS = {'
    for i in $(seq 0 $(($(ls ./train/SelectionRecognizer/*.png | wc -l) - 1)))
    do
    printf '%s' "cv::Mat(DATA[$i].size()/$rows,$rows,CV_8U,(void*)DATA[$i].data()),";
    done
    printf '%s\n' '};'
) > ./SelectionRecognizerGenerated.cpp
clang-format -i ./SelectionRecognizerGenerated.cpp
