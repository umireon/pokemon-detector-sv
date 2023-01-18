#include <iostream>

#include "PokemonRecognizer.h"

std::string PokemonRecognizer::recognizePokemon(const cv::Mat &imageBGR,
                                                const cv::Mat &mask) {
  std::vector<cv::KeyPoint> targetKeyPoints;
  cv::Mat targetDescriptors;
  auto algorithm = cv::AKAZE::create(cv::AKAZE::DESCRIPTOR_MLDB, 16);
  algorithm->detectAndCompute(imageBGR, mask, targetKeyPoints,
                              targetDescriptors);

  auto matcher = cv::DescriptorMatcher::create("BruteForce");
  std::vector<double> results;
  for (int i = 0; i < POKEMON_DESCRIPTORS.size(); i++) {
    std::vector<cv::DMatch> matches;
    const cv::Mat candidateDescriptor = POKEMON_DESCRIPTORS[i % POKEMON_DESCRIPTORS.size()];
    matcher->match(targetDescriptors, candidateDescriptor, matches);

    double sum = 0.0;
    for (auto match : matches) {
      sum += match.distance;
    }

    results.push_back(sum / (double) matches.size());
  }

  const auto index = std::distance(results.begin(), std::min_element(results.begin(), results.end()));

  return POKEMON_IDS[index];
}