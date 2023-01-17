#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

int main(int argc, const char **argv) {
    const std::vector<std::string> screenPaths(&argv[1], &argv[argc]);

    const cv::Range colRange(1239, 1337);
    const cv::Scalar white(255, 255, 255);

    for (std::string imagePath : screenPaths) {
        const cv::Mat screen = cv::imread(imagePath);
        const cv::Mat myPokemons = screen(cv::Range(139, 842), cv::Range(149, 811));
        cv::Mat myPokemonsHSV;
        cv::cvtColor(myPokemons, myPokemonsHSV, cv::COLOR_BGR2HSV);
        const int channels[]{ 0 };
        const cv::Mat mask;
        const int histSize[]{ 30 };
        const float hranges[] = { 0, 180 };
        const float *ranges[]{ hranges };
        cv::Mat hist;
        cv::calcHist(&myPokemonsHSV, 1, channels, mask, hist, 1, histSize, ranges);
        cv::normalize(hist, hist, 0);
        std::cout << hist;
//        for (int i = 0; i < 6; i++) {
//            const cv::Range rowRange(228 + 102 * i, 326 + 102 * i);
//            cv::Mat pokemonBGR = screen(rowRange, colRange);
//            cv::Mat mask;
//            cv::floodFill(pokemonBGR, mask, cv::Point(0, 0), white);
//            cv::Mat maskCropped = 1 - mask(cv::Range(1, 99), cv::Range(1, 99));
//
//            cv::Mat pokemonBGRA;
//            cv::cvtColor(pokemonBGR, pokemonBGRA, cv::COLOR_BGR2BGRA);
//
//            const int nBins = 32;
//            const int nSampling = 256 / nBins;
//            std::vector<int> histB(nBins), histG(nBins), histR(nBins);
//            for (int x = 0; x < pokemonBGRA.cols; x++) {
//                for (int y = 0; y < pokemonBGRA.rows; y++) {
//                    bool isForeground = maskCropped.at<unsigned char>(y, x);
//                    auto &pixel = pokemonBGRA.at<cv::Vec4b>(y, x);
//                    pixel[3] = maskCropped.at<unsigned char>(y, x) * 255;
//                    if (isForeground) {
//                        histB[pixel[0] / nSampling]++;
//                        histG[pixel[0] / nSampling]++;
//                        histB[pixel[0] / nSampling]++;
//                    }
//                }
//            }
//
//            for (auto b : histB) {
//                std::cout << b << ",";
//            }
//            std::cout << std::endl;
//
//            cv::imwrite("dest/" + imagePath + "." + std::to_string(i) + ".png", pokemonBGRA);
//        }
    }
    return 0;
}