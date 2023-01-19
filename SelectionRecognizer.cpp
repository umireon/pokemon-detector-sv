#include "SelectionRecognizer.h"
#include <iostream>

static constexpr int BLUE_THRESHOLD = 200;

int SelectionRecognizer::recognizeSelection(const cv::Mat &imageBGR) {
  if (imageBGR.at<cv::Vec3b>(0, 0)[0] < BLUE_THRESHOLD) return -1;

  cv::Mat scaledBGR;
  cv::resize(imageBGR, scaledBGR,
             cv::Size(imageBGR.rows * NORMALIZED_HEIGHT / imageBGR.cols,
                      NORMALIZED_HEIGHT));
  cv::cvtColor(scaledBGR, scaledBGR, cv::COLOR_BGR2GRAY);
  cv::threshold(scaledBGR, scaledBGR, BINARY_THRESHOLD, 255, cv::THRESH_BINARY);

  std::vector<cv::KeyPoint> targetKeyPoints;
  cv::Mat targetDescriptors;
  auto algorithm =
      cv::AKAZE::create(cv::AKAZE::DESCRIPTOR_MLDB, DESCRIPTOR_SIZE);
  cv::Mat mask = cv::Mat::ones(scaledBGR.rows, scaledBGR.cols, CV_8U);
  algorithm->detectAndCompute(scaledBGR, mask, targetKeyPoints,
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

    std::cout << sum / (double)matches.size() << std::endl;
  }

  const auto index = std::distance(
      results.begin(), std::min_element(results.begin(), results.end()));

  return SELECTION_INDEX[index];
}