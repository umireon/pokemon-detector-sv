#include <fstream>
#include <iomanip>
#include <iostream>

#include <opencv2/opencv.hpp>

int main(int argc, const char **argv) {
  const std::vector<std::string> paths(&argv[1], &argv[argc]);
  auto algorithm = cv::AKAZE::create(cv::AKAZE::DESCRIPTOR_MLDB, 16);

  std::vector<std::vector<cv::KeyPoint>> keyPointsVector;
  std::vector<cv::Mat> descriptorsVector;
  for (auto path : paths) {
    cv::Mat imageBGRA = cv::imread(path, cv::IMREAD_UNCHANGED);
    cv::Mat imageBGR;
    cv::cvtColor(imageBGRA, imageBGR, cv::COLOR_BGRA2BGR);
    cv::Mat mask(imageBGRA.rows, imageBGRA.cols, CV_8UC1);
    for (int y = 0; y < imageBGRA.rows; y++) {
      for (int x = 0; x < imageBGRA.cols; x++) {
        mask.at<uchar>(y, x) = imageBGRA.at<cv::Vec4b>(y, x)[3];
      }
    }

    std::vector<cv::KeyPoint> keyPoints;
    cv::Mat descriptors;
    algorithm->detectAndCompute(imageBGR, mask, keyPoints, descriptors);
    keyPointsVector.push_back(keyPoints);
    descriptorsVector.push_back(descriptors);
  }

  std::ofstream ofs("./PokemonRecognizerGenerated.cpp");

  ofs << "#include <opencv2/opencv.hpp>" << std::endl;
  ofs << "#include \"PokemonRecognizer.h\"" << std::endl;
  ofs << "const std::vector<std::string> PokemonRecognizer::POKEMON_IDS = {";
  for (auto &filepath : paths) {
    auto beginIndex = filepath.find_last_of("/") + 1;
    auto endIndex = filepath.find_last_of(".");
    auto length = endIndex - beginIndex;
    ofs << "\"" << filepath.substr(beginIndex, length) << "\",";
  }
  ofs << "};" << std::endl;

  ofs << "const std::vector<std::vector<unsigned char>> "
         "PokemonRecognizer::POKEMON_DESCRIPTORS = {";
  for (auto &descriptors : descriptorsVector) {
    ofs << "{";
    for (int i = 0; i < descriptors.total(); i++) {
      ofs << (int)descriptors.at<uchar>(i) << ",";
    }
    ofs << "},";
  }
  ofs << "};" << std::endl;

  ofs << "const std::vector<std::vector<cv::KeyPoint>> "
         "PokemonRecognizer::POKEMON_KEYPOINTS = {";
  for (auto &keyPoints : keyPointsVector) {
    ofs << "{";
    for (auto &keyPoint : keyPoints) {
      ofs << std::scientific << std::setprecision(10) << "cv::KeyPoint("
          << keyPoint.pt.x << "," << keyPoint.pt.y << "," << keyPoint.angle
          << "," << keyPoint.response << "," << keyPoint.octave << ","
          << keyPoint.class_id << "),";
    }
    ofs << "},";
  }
  ofs << "};" << std::endl;
}
