#include <iostream>

#include <opencv2/opencv.hpp>

int main(int argc, const char **argv) {
  const std::vector<std::string> paths(&argv[1], &argv[argc]);
  std::vector<cv::Mat> images;
  for (auto path : paths) {
    std::cout << path;
    images.push_back(cv::imread(path));
  }

  auto algorithm = cv::AKAZE::create(cv::AKAZE::DESCRIPTOR_MLDB, 16);
  std::vector<std::vector<cv::KeyPoint>> keyPoints(argc - 1);
  std::vector<cv::Mat> descriptors(argc - 1);
  for (int i = 0; i < argc - 1; i++) {
    algorithm->detectAndCompute(images[i], cv::Mat(), keyPoints[i], descriptors[i]);
  }

  std::cout << descriptors[0] << std::endl;
  for (auto keyPoint : keyPoints[0]) {
    std::cout << keyPoint.angle << std::endl;
    std::cout << keyPoint.class_id << std::endl;
    std::cout << keyPoint.octave << std::endl;
    std::cout << keyPoint.pt << std::endl;
    std::cout << keyPoint.response << std::endl;
    std::cout << keyPoint.size << std::endl;
    std::cout << "--------" << std::endl;
  }

  cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create("BruteForce");
  std::vector<cv::DMatch> match, match12, match21;
  matcher->match(descriptors[0], descriptors[1], match12);
  matcher->match(descriptors[1], descriptors[0], match21);

  for (auto matcha : match12) {
    std::cout << matcha.distance << std::endl;
  }

  cv::Mat dest;
  cv::drawMatches(images[0], keyPoints[0], images[1], keyPoints[1], match12, dest);

  cv::imwrite("/Users/umireon/Movies/1.png", dest);
}