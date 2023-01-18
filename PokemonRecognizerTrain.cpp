#include <fstream>
#include <iomanip>

#include <opencv2/opencv.hpp>

static cv::Mat extractMaskFromBGRA(const cv::Mat &bgra) {
  cv::Mat mask(bgra.rows, bgra.cols, CV_8U);
  for (int y = 0; y < bgra.rows; y++) {
    for (int x = 0; x < bgra.cols; x++) {
      mask.at<uchar>(y, x) = bgra.at<cv::Vec4b>(y, x)[3];
    }
  }
  return mask;
}

static std::string basenameWithoutExt(const std::string &path) {
  auto beginIndex = path.find_last_of("/") + 1;
  auto endIndex = path.find_last_of(".");
  auto length = endIndex - beginIndex;
  return path.substr(beginIndex, length);
}

int main(int argc, const char **argv) {
  const std::vector<std::string> paths(&argv[1], &argv[argc]);
  const auto numOfData = paths.size();
  auto algorithm = cv::AKAZE::create(cv::AKAZE::DESCRIPTOR_MLDB, 16);

  std::vector<std::vector<cv::KeyPoint>> keyPointsVector;
  std::vector<cv::Mat> descriptorsVector;
  for (auto path : paths) {
    const cv::Mat imageBGRA = cv::imread(path, cv::IMREAD_UNCHANGED);
    cv::Mat imageBGR, mask = extractMaskFromBGRA(imageBGRA);
    cv::cvtColor(imageBGRA, imageBGR, cv::COLOR_BGRA2BGR);

    std::vector<cv::KeyPoint> keyPoints;
    cv::Mat descriptors;
    algorithm->detectAndCompute(imageBGR, mask, keyPoints, descriptors);
    descriptorsVector.push_back(descriptors);
  }

  std::ofstream ofs("./PokemonRecognizerGenerated.cpp");

  ofs << "#include <array>" << std::endl;
  ofs << "#include <opencv2/opencv.hpp>" << std::endl;
  ofs << "#include \"PokemonRecognizer.h\"" << std::endl;

  ofs << "const std::vector<std::string> PokemonRecognizer::POKEMON_IDS "
         "= {";
  for (auto &filepath : paths) {
    ofs << "\"" << basenameWithoutExt(filepath) << "\",";
  }
  ofs << "};" << std::endl;

  ofs << "static const std::vector<std::vector<uchar>> DATA = {";
  for (auto &descriptors : descriptorsVector) {
    ofs << "{";
    for (int i = 0; i < descriptors.total(); i++) {
      ofs << (int)descriptors.at<uchar>(i) << ",";
    }
    ofs << "},";
  }
  ofs << "};" << std::endl;

  ofs << "const std::vector<cv::Mat> PokemonRecognizer::POKEMON_DESCRIPTORS = "
         "{";
  for (int i = 0; i < descriptorsVector.size(); i++) {
    std::string data = "DATA[" + std::to_string(i) + "]";
    ofs << "cv::Mat(" << data << ".size()/2,2,CV_8U,(void*)" << data << ".data()),";
  }
  ofs << "};" << std::endl;
}
