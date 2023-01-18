#include "SelectionRecognizer.h"
#include <iostream>
static constexpr int NORMALIZED_HEIGHT = 98;

std::string SelectionRecognizer::recognizeSelection(const cv::Mat &imageBGR) {
  cv::Mat scaledBGR;
  cv::resize(imageBGR, scaledBGR,
             cv::Size(imageBGR.cols * NORMALIZED_HEIGHT / imageBGR.rows,
                      NORMALIZED_HEIGHT));
  std::cout << scaledBGR.size;

  std::vector<cv::KeyPoint> targetKeyPoints;
  cv::Mat targetDescriptors;
  auto algorithm = cv::AKAZE::create(cv::AKAZE::DESCRIPTOR_MLDB, 64);
  algorithm->detectAndCompute(scaledBGR, cv::Mat(), targetKeyPoints,
                              targetDescriptors);

  auto matcher = cv::DescriptorMatcher::create("BruteForce");
  std::vector<double> results;
  for (int i = 0; i < SELECTION_DESCRIPTORS.size(); i++) {
    std::vector<cv::DMatch> matches;
    const cv::Mat candidateDescriptor = SELECTION_DESCRIPTORS[i];
    matcher->match(targetDescriptors, candidateDescriptor, matches);

    double sum = 0.0;
    for (auto match : matches) {
      sum += match.distance;
    }

    if (matches.size() == 0) {
      results.push_back(1.0 / 0.0);
    } else {
      results.push_back(sum / (double)matches.size());
    }
  }

  const auto index = std::distance(
      results.begin(), std::min_element(results.begin(), results.end()));

  if (results[index] > 100) {
    return "unselected";
  } else {
    return SELECTION_IDS[index];
  }
}