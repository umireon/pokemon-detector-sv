#!/bin/bash
set -euo pipefail
(
    printf '%s\n' '#include <vector>'
    printf '%s\n' '#include <opencv2/opencv.hpp>'
    printf '%s\n' '#include "PokemonRecognizer.h"'

    printf '%s' 'const std::vector<std::string> PokemonRecognizer::POKEMON_IDS = {'
    for name in $(basename -s .png ./train/PokemonRecognizer/*.png)
    do
    printf '%s' \"$name\",
    done
    printf '%s\n' '};'

    printf '%s' 'static const std::vector<std::vector<uchar>> DATA = '
    ./cmake-build-debug/AkAZETrainer ./train/PokemonRecognizer/*.png
    printf '%s\n' ';'

    printf '%s' 'const std::vector<cv::Mat> PokemonRecognizer::POKEMON_DESCRIPTORS = {'
    for i in $(seq 0 $(($(ls ./train/PokemonRecognizer/*.png | wc -l) - 1)))
    do
    printf '%s' "cv::Mat(DATA[$i].size()/2,2,CV_8U,(void*)DATA[$i].data()),";
    done
    printf '%s\n' '};'
) > ./PokemonRecognizerGenerated.cpp
clang-format -i ./PokemonRecognizerGenerated.cpp
