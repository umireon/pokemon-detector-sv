#include <vector>
#include <opencv2/opencv.hpp>
#include "SelectionRecognizer.h"
const int SelectionRecognizer::DESCRIPTOR_SIZE = 16;
const int SelectionRecognizer::NORMALIZED_HEIGHT = 128;
const int SelectionRecognizer::BINARY_THRESHOLD = 200;
const std::vector<int> SelectionRecognizer::SELECTION_INDEX = {
	1,
	2,
	3,
	1,
};
static const std::vector<std::vector<uchar>> DATA = {
	{
		190,
		115,
		146,
		96,
		58,
		95,
		138,
		114,
		162,
		105,
		158,
		18,
	},
	{
		154, 114, 146, 66, 130, 115, 146, 2,  0,   110, 58, 91,
		28,  22,  32,  79, 60,  119, 154, 18, 32,  95,  54, 75,
		174, 10,  186, 10, 138, 10,  8,   86, 166, 11,  32, 79,
		138, 74,  8,   6,  34,  75,  8,   22, 8,   30,  8,  62,
		139, 74,  138, 90, 26,  90,  46,  2,
	},
	{
		128, 107, 146, 66, 8,   111, 160, 107, 146, 66,  10,
		2,   8,   98,  11, 10,  20,  37,  144, 103, 18,  66,
		146, 54,  146, 66, 186, 91,  138, 82,  24,  87,  11,
		74,  88,  126, 54, 215, 168, 42,  8,   70,  146, 119,
	},
	{
		190, 115, 146, 96,  58, 95,  16,  68,  136, 114,
		56,  94,  8,   22,  8,  94,  8,   94,  8,   110,
		186, 74,  162, 104, 36, 129, 182, 129, 154, 38,
	},
};
const std::vector<cv::Mat> SelectionRecognizer::SELECTION_DESCRIPTORS = {
	cv::Mat(DATA[0].size() / 2, 2, CV_8U, (void *)DATA[0].data()),
	cv::Mat(DATA[1].size() / 2, 2, CV_8U, (void *)DATA[1].data()),
	cv::Mat(DATA[2].size() / 2, 2, CV_8U, (void *)DATA[2].data()),
	cv::Mat(DATA[3].size() / 2, 2, CV_8U, (void *)DATA[3].data()),
};
