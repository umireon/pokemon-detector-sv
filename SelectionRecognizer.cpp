#include "SelectionRecognizer.h"

static constexpr int NORMALIZED_HEIGHT = 98;
static constexpr int DESCRIPTOR_SIZE = 64;
static constexpr int UNRECOGNIZED_THRESHOLD = 100;

int SelectionRecognizer::recognizeSelection(const cv::Mat &imageBGR) {
  cv::Mat scaledBGR;
  cv::resize(imageBGR, scaledBGR,
             cv::Size(imageBGR.cols * NORMALIZED_HEIGHT / imageBGR.rows,
                      NORMALIZED_HEIGHT));

  std::vector<cv::KeyPoint> targetKeyPoints;
  cv::Mat targetDescriptors;
  auto algorithm =
      cv::AKAZE::create(cv::AKAZE::DESCRIPTOR_MLDB, DESCRIPTOR_SIZE);
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

  if (results[index] > UNRECOGNIZED_THRESHOLD) {
    return -1;
  } else {
    return SELECTION_INDEX[index];
  }
}