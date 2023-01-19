#include "SelectionRecognizer.h"
#include <opencv2/opencv.hpp>
#include <vector>
const int SelectionRecognizer::DESCRIPTOR_SIZE = 64;
const int SelectionRecognizer::NORMALIZED_HEIGHT = 256;
const int SelectionRecognizer::BINARY_THRESHOLD = 200;
const std::vector<int> SelectionRecognizer::SELECTION_INDEX = {
    1, 2, 3, -1, -1, -1,
};
static const std::vector<std::vector<uchar>> DATA = {
    {
        0,   46,  224, 176, 1,   227, 230, 128, 154, 18,  35,  146, 0,   96,
        230, 164, 28,  22,  96,  144, 64,  113, 230, 192, 154, 18,  35,  146,
        0,   96,  230, 164, 40,  79,  242, 180, 143, 250, 198, 137, 184, 127,
        243, 182, 143, 251, 199, 141, 146, 38,  98,  144, 12,  97,  166, 172,
        16,  38,  98,  176, 1,   225, 166, 192, 32,  75,  243, 182, 7,   224,
        194, 165, 32,  79,  178, 52,  6,   250, 194, 9,   28,  22,  96,  144,
        128, 107, 230, 160, 28,  119, 98,  144, 174, 122, 231, 168, 154, 91,
        163, 146, 232, 127, 230, 172, 184, 127, 243, 182, 231, 254, 195, 231,
        146, 75,  18,  182, 37,  242, 130, 164, 8,   18,  36,  49,  1,   233,
        70,  128, 184, 127, 242, 166, 135, 246, 195, 167, 16,  79,  242, 180,
        36,  242, 130, 160, 160, 111, 147, 52,  134, 251, 199, 141, 146, 64,
        83,  150, 37,  240, 130, 36,  58,  95,  242, 166, 135, 244, 193, 135,
        62,  18,  36,  177, 161, 191, 86,  224, 186, 123, 225, 177, 163, 255,
        119, 224,
    },
    {
        56,  95,  242, 182, 135, 226, 194, 169, 32,  111, 242, 180, 7,   224,
        194, 129, 40,  111, 242, 180, 207, 242, 194, 137, 40,  95,  242, 180,
        143, 250, 194, 137, 32,  127, 242, 180, 143, 250, 194, 137, 154, 18,
        35,  146, 0,   96,  230, 164, 130, 34,  51,  146, 0,   96,  226, 164,
        154, 18,  35,  146, 0,   96,  230, 164, 40,  79,  242, 180, 143, 250,
        194, 137, 28,  22,  96,  144, 0,   99,  230, 160, 28,  54,  96,  144,
        0,   99,  230, 160, 24,  22,  96,  144, 0,   97,  230, 160, 154, 18,
        35,  146, 0,   96,  230, 172, 154, 18,  35,  146, 0,   96,  230, 172,
        32,  127, 242, 180, 135, 250, 199, 137, 48,  127, 242, 182, 143, 246,
        199, 141, 56,  127, 242, 164, 135, 246, 199, 137, 0,   78,  160, 176,
        33,  243, 230, 128, 2,   75,  19,  182, 32,  242, 194, 37,  56,  95,
        242, 182, 143, 226, 194, 233, 32,  127, 242, 164, 135, 246, 199, 137,
        26,  22,  32,  144, 0,   99,  230, 160, 40,  111, 242, 180, 143, 250,
        194, 137, 146, 18,  35,  146, 0,   96,  230, 172, 154, 18,  51,  146,
        0,   96,  226, 164, 32,  111, 226, 180, 143, 250, 198, 137, 56,  127,
        242, 180, 207, 234, 194, 137, 178, 91,  243, 182, 15,  226, 194, 173,
        32,  79,  242, 180, 207, 234, 194, 137, 32,  111, 242, 180, 207, 242,
        194, 137, 56,  127, 226, 176, 207, 226, 198, 137, 130, 34,  51,  150,
        0,   96,  194, 165, 56,  127, 242, 180, 143, 250, 194, 137, 138, 18,
        35,  146, 0,   96,  230, 140, 8,   18,  35,  146, 0,   96,  194, 132,
        40,  111, 242, 180, 15,  250, 194, 137, 146, 54,  50,  150, 4,   96,
        162, 164, 32,  75,  146, 4,   6,   251, 194, 9,   178, 91,  243, 182,
        15,  224, 194, 237, 32,  127, 242, 182, 135, 246, 199, 175, 32,  111,
        242, 182, 143, 227, 198, 137, 32,  111, 242, 182, 239, 226, 194, 169,
        32,  111, 242, 182, 143, 234, 194, 137, 40,  79,  242, 180, 199, 234,
        194, 137, 32,  111, 242, 182, 239, 227, 198, 137, 24,  22,  34,  144,
        160, 115, 230, 160, 40,  127, 242, 180, 143, 251, 198, 137, 32,  111,
        242, 180, 135, 226, 194, 137, 18,  22,  34,  144, 224, 99,  230, 160,
        24,  22,  34,  144, 160, 107, 230, 160, 146, 18,  34,  144, 96,  99,
        166, 160, 32,  127, 242, 182, 167, 247, 199, 141, 24,  22,  34,  144,
        192, 115, 230, 160, 56,  111, 226, 176, 143, 226, 198, 137, 0,   110,
        162, 176, 161, 243, 230, 128, 154, 18,  34,  144, 0,   97,  230, 160,
        32,  75,  147, 148, 38,  242, 198, 45,  32,  111, 242, 180, 135, 226,
        194, 137, 32,  75,  19,  20,  6,   251, 194, 5,   40,  79,  226, 176,
        175, 242, 199, 169, 48,  127, 242, 182, 167, 246, 199, 175, 26,  18,
        34,  144, 160, 99,  230, 160, 24,  22,  32,  144, 128, 99,  230, 160,
        32,  111, 242, 180, 135, 226, 194, 137, 24,  22,  96,  144, 128, 107,
        230, 160, 28,  22,  96,  144, 174, 105, 230, 168, 154, 18,  35,  146,
        160, 111, 230, 172, 32,  127, 242, 182, 231, 226, 194, 137, 32,  111,
        226, 144, 47,  234, 198, 168, 186, 91,  49,  150, 46,  108, 194, 237,
        32,  111, 242, 182, 47,  242, 194, 169, 56,  95,  242, 148, 47,  242,
        194, 168, 166, 107, 177, 182, 47,  236, 194, 237, 62,  95,  242, 182,
        47,  242, 194, 233, 186, 91,  242, 182, 47,  242, 194, 237, 32,  111,
        242, 148, 167, 242, 194, 137, 32,  127, 242, 148, 239, 242, 194, 201,
        186, 75,  177, 150, 46,  96,  194, 173, 32,  111, 242, 182, 47,  242,
        194, 233, 28,  23,  98,  144, 46,  114, 230, 168, 32,  107, 243, 182,
        47,  242, 194, 237, 154, 18,  33,  146, 160, 115, 226, 164, 28,  22,
        96,  144, 46,  112, 230, 160, 8,   22,  96,  144, 128, 107, 230, 128,
        48,  95,  242, 182, 111, 226, 194, 233, 32,  111, 242, 182, 103, 224,
        194, 233, 8,   22,  96,  144, 128, 107, 198, 128, 8,   22,  32,  128,
        128, 123, 70,  128, 32,  79,  242, 180, 143, 250, 194, 137, 32,  111,
        242, 182, 7,   226, 194, 137, 8,   22,  36,  144, 128, 227, 78,  128,
        20,  37,  82,  144, 72,  67,  166, 201, 20,  38,  114, 144, 68,  99,
        166, 224, 154, 114, 35,  208, 160, 127, 231, 160, 146, 66,  35,  208,
        160, 115, 166, 172, 154, 66,  35,  144, 160, 115, 166, 172, 0,   111,
        3,   176, 161, 243, 198, 160, 130, 107, 3,   148, 32,  251, 199, 21,
        138, 18,  35,  144, 160, 127, 230, 172, 138, 50,  35,  144, 160, 127,
        230, 160, 154, 54,  34,  144, 224, 127, 230, 160, 28,  22,  98,  144,
        238, 108, 230, 168, 32,  79,  242, 150, 103, 226, 194, 233, 154, 18,
        177, 214, 110, 108, 195, 173, 34,  75,  242, 150, 103, 226, 194, 233,
        186, 75,  241, 214, 46,  224, 194, 233, 190, 11,  241, 214, 111, 224,
        194, 233, 138, 10,  161, 210, 46,  96,  195, 172, 8,   87,  114, 148,
        46,  98,  194, 168, 8,   22,  96,  144, 40,  112, 194, 160, 8,   22,
        96,  144, 128, 123, 198, 128, 139, 74,  163, 248, 161, 250, 199, 188,
        138, 90,  163, 216, 161, 126, 199, 188,
    },
    {
        48,  95,  242, 182, 7,   226, 194, 169, 32,  111, 242, 180, 7,   226,
        194, 137, 56,  95,  242, 180, 143, 250, 194, 137, 32,  111, 242, 180,
        207, 234, 194, 137, 32,  95,  242, 182, 143, 227, 194, 169, 162, 107,
        243, 178, 15,  226, 226, 172, 40,  111, 242, 180, 143, 250, 194, 137,
        160, 127, 243, 182, 143, 251, 199, 237, 4,   54,  96,  144, 0,   97,
        230, 160, 24,  22,  32,  144, 0,   99,  230, 160, 32,  127, 242, 180,
        143, 255, 199, 137, 154, 18,  35,  146, 0,   96,  230, 164, 154, 18,
        35,  146, 0,   96,  230, 164, 4,   38,  96,  144, 0,   97,  230, 192,
        28,  22,  96,  144, 0,   97,  230, 160, 48,  95,  242, 182, 143, 226,
        194, 137, 160, 127, 243, 182, 135, 246, 195, 143, 154, 2,   35,  146,
        0,   96,  230, 164, 40,  111, 226, 176, 143, 226, 198, 137, 24,  22,
        34,  144, 64,  97,  230, 160, 32,  111, 242, 180, 135, 250, 194, 137,
        62,  95,  242, 150, 134, 118, 197, 173, 56,  95,  226, 176, 143, 226,
        230, 168, 48,  127, 242, 182, 135, 246, 197, 173, 24,  22,  34,  144,
        0,   97,  230, 160, 48,  95,  242, 182, 143, 226, 194, 137, 32,  127,
        242, 180, 143, 226, 194, 137, 32,  111, 243, 182, 15,  226, 194, 141,
        154, 18,  35,  146, 0,   99,  230, 172, 40,  95,  242, 180, 143, 250,
        194, 137, 154, 18,  35,  146, 0,   96,  230, 164, 48,  127, 242, 182,
        135, 246, 199, 141, 8,   70,  98,  132, 142, 122, 194, 136, 150, 39,
        114, 146, 76,  96,  166, 237, 40,  111, 242, 180, 135, 250, 194, 136,
        32,  79,  178, 52,  6,   250, 194, 137, 154, 18,  51,  146, 0,   96,
        226, 164, 146, 18,  35,  146, 0,   96,  230, 165, 28,  22,  96,  144,
        0,   99,  198, 129, 154, 18,  35,  146, 0,   96,  230, 172, 58,  95,
        242, 182, 143, 226, 194, 169, 146, 18,  35,  146, 0,   96,  230, 172,
        48,  95,  242, 180, 143, 226, 226, 136, 40,  111, 226, 176, 143, 226,
        198, 137, 26,  22,  34,  144, 0,   97,  230, 160, 32,  111, 241, 182,
        7,   224, 194, 133, 32,  111, 242, 180, 135, 250, 194, 137, 32,  111,
        242, 182, 7,   226, 194, 137, 56,  95,  114, 180, 134, 246, 197, 141,
        48,  127, 242, 180, 135, 246, 199, 143, 138, 18,  35,  146, 0,   96,
        230, 172, 32,  79,  242, 52,  134, 250, 194, 137, 154, 18,  35,  146,
        0,   96,  230, 164, 24,  22,  96,  144, 64,  105, 230, 160, 28,  22,
        96,  144, 0,   99,  230, 160, 32,  75,  242, 182, 167, 242, 194, 169,
        138, 2,   51,  150, 0,   96,  194, 165, 28,  22,  96,  144, 0,   97,
        230, 160, 28,  22,  96,  144, 0,   99,  230, 128, 28,  22,  96,  144,
        0,   97,  230, 160, 146, 18,  35,  146, 0,   96,  230, 164, 154, 18,
        35,  146, 0,   96,  230, 164, 32,  79,  242, 180, 143, 250, 194, 169,
        32,  95,  242, 180, 143, 250, 194, 137, 32,  111, 242, 182, 7,   226,
        194, 169, 32,  111, 242, 182, 15,  226, 194, 233, 40,  111, 242, 180,
        135, 250, 194, 137, 160, 127, 243, 182, 239, 255, 199, 173, 32,  111,
        242, 180, 135, 226, 194, 137, 16,  54,  34,  144, 160, 123, 230, 160,
        24,  22,  34,  144, 160, 115, 230, 160, 28,  22,  32,  144, 160, 107,
        230, 160, 18,  22,  34,  144, 96,  98,  166, 160, 34,  74,  129, 148,
        38,  178, 198, 45,  32,  111, 242, 182, 135, 226, 194, 137, 0,   110,
        130, 176, 129, 251, 198, 129, 146, 22,  34,  144, 64,  107, 166, 164,
        56,  79,  226, 176, 174, 242, 198, 169, 32,  107, 19,  52,  0,   251,
        194, 5,   40,  111, 242, 180, 135, 226, 194, 137, 138, 18,  35,  144,
        160, 123, 230, 164, 138, 50,  35,  144, 160, 123, 230, 164, 10,  18,
        34,  144, 96,  98,  230, 160, 32,  111, 242, 150, 231, 226, 194, 137,
        24,  22,  32,  144, 128, 123, 230, 160, 138, 66,  35,  210, 160, 115,
        230, 172, 8,   94,  98,  134, 166, 114, 194, 169, 28,  22,  96,  144,
        128, 123, 230, 160, 8,   90,  162, 192, 6,   242, 66,  184, 32,  75,
        146, 4,   134, 251, 199, 9,   124, 95,  180, 241, 50,  242, 106, 224,
        16,  102, 210, 180, 201, 123, 130, 136, 34,  75,  146, 148, 38,  242,
        194, 41,  16,  103, 114, 182, 229, 253, 130, 132, 0,   111, 242, 182,
        135, 235, 194, 136, 28,  22,  224, 144, 33,  114, 230, 160, 32,  111,
        242, 180, 135, 250, 194, 137, 4,   38,  96,  144, 128, 99,  230, 160,
        20,  22,  34,  144, 128, 123, 230, 160, 32,  111, 242, 180, 135, 226,
        194, 137, 18,  22,  34,  144, 32,  114, 166, 160, 146, 18,  34,  144,
        96,  97,  166, 164, 40,  111, 226, 176, 143, 251, 199, 137, 32,  111,
        146, 52,  134, 251, 199, 137, 154, 18,  34,  144, 160, 99,  230, 160,
        138, 18,  35,  144, 160, 99,  230, 164, 28,  87,  98,  144, 142, 114,
        230, 168, 28,  22,  32,  144, 224, 99,  230, 160, 24,  22,  34,  144,
        192, 107, 230, 160, 24,  22,  32,  144, 128, 107, 230, 160, 32,  111,
        242, 180, 135, 226, 194, 137, 154, 66,  35,  146, 160, 115, 230, 172,
        28,  22,  96,  144, 96,  98,  230, 160, 16,  22,  32,  144, 160, 115,
        230, 160, 130, 123, 163, 176, 161, 255, 231, 160, 146, 66,  34,  208,
        160, 115, 166, 172, 8,   111, 3,   176, 129, 255, 198, 129, 128, 107,
        3,   148, 160, 219, 199, 5,   146, 66,  35,  208, 160, 115, 166, 172,
        138, 50,  35,  144, 160, 125, 167, 172, 10,  2,   35,  208, 32,  98,
        198, 160, 136, 126, 35,  144, 128, 127, 231, 128, 10,  66,  98,  192,
        38,  114, 194, 168, 11,  74,  160, 216, 54,  226, 66,  248, 190, 123,
        243, 246, 231, 244, 195, 231, 126, 27,  228, 241, 115, 226, 110, 224,
        186, 91,  242, 166, 167, 245, 195, 166, 2,   74,  224, 144, 33,  226,
        230, 160, 2,   67,  34,  144, 160, 114, 166, 160, 146, 114, 35,  144,
        224, 125, 166, 160, 146, 54,  34,  208, 224, 99,  166, 236, 146, 66,
        34,  208, 160, 115, 166, 172, 32,  111, 179, 180, 134, 255, 199, 141,
        138, 82,  35,  144, 160, 127, 230, 172, 26,  87,  114, 148, 174, 114,
        231, 168, 216, 126, 162, 24,  137, 124, 135, 152, 182, 211, 114, 222,
        231, 100, 225, 231, 168, 42,  129, 22,  87,  201, 195, 197, 8,   70,
        130, 144, 33,  234, 134, 144, 146, 119, 35,  16,  165, 125, 167, 172,
    },
    {
        160, 111, 243, 182, 1,   224, 194, 133, 0,   110, 224, 176, 1,   227,
        230, 128, 28,  87,  98,  144, 142, 98,  230, 136, 186, 91,  242, 150,
        6,   98,  194, 169, 190, 119, 243, 182, 231, 252, 195, 231, 166, 123,
        179, 182, 137, 255, 199, 237, 154, 90,  49,  146, 134, 99,  226, 164,
        48,  95,  242, 182, 143, 242, 198, 137, 40,  79,  162, 48,  12,  251,
        198, 137, 40,  79,  242, 180, 143, 250, 194, 137, 32,  75,  146, 20,
        6,   251, 194, 13,  176, 127, 243, 182, 135, 247, 199, 141, 56,  95,
        242, 148, 7,   226, 194, 233, 162, 8,   137, 146, 46,  138, 212, 45,
        148, 119, 115, 180, 231, 124, 227, 205, 28,  119, 114, 180, 136, 127,
        199, 136, 0,   111, 226, 176, 129, 227, 230, 128, 160, 107, 179, 182,
        129, 251, 198, 133, 32,  78,  144, 4,   6,   251, 194, 129, 48,  95,
        242, 180, 135, 242, 194, 137, 154, 95,  114, 182, 198, 118, 227, 166,
        0,   79,  226, 180, 143, 248, 198, 137, 28,  119, 114, 180, 142, 118,
        231, 140, 32,  111, 243, 180, 1,   227, 194, 133, 0,   110, 224, 176,
        73,  227, 230, 136, 154, 91,  163, 178, 143, 99,  230, 236, 28,  87,
        114, 148, 142, 123, 194, 136, 48,  127, 242, 150, 134, 227, 199, 137,
        160, 127, 243, 182, 143, 255, 199, 141, 32,  127, 242, 180, 135, 242,
        194, 137, 32,  107, 131, 4,   134, 251, 199, 133, 40,  111, 242, 180,
        135, 255, 199, 137, 190, 91,  243, 244, 231, 246, 195, 239,
    },
    {
        32,  127, 242, 180, 143, 226, 194, 137, 154, 91,  178, 150, 14,  98,
        226, 168, 20,  87,  114, 148, 134, 114, 194, 201, 52,  87,  98,  144,
        206, 242, 198, 201, 32,  79,  242, 180, 143, 250, 194, 137, 32,  111,
        226, 176, 143, 250, 198, 137, 32,  111, 241, 182, 7,   224, 194, 229,
        56,  95,  226, 176, 143, 226, 230, 136, 48,  95,  242, 182, 15,  226,
        194, 137, 32,  111, 241, 182, 1,   224, 194, 133, 32,  111, 226, 176,
        9,   226, 230, 136, 56,  95,  226, 176, 143, 226, 230, 136, 32,  111,
        242, 180, 7,   226, 194, 201, 40,  79,  242, 180, 207, 242, 194, 137,
        32,  111, 242, 182, 7,   226, 194, 137, 40,  111, 242, 148, 207, 242,
        194, 137, 32,  107, 243, 182, 15,  226, 194, 173, 48,  127, 242, 182,
        15,  226, 194, 169, 32,  111, 242, 182, 15,  224, 194, 137, 32,  111,
        242, 182, 15,  224, 194, 137, 56,  79,  242, 180, 143, 250, 194, 137,
        32,  79,  226, 176, 143, 226, 198, 136, 48,  95,  242, 180, 143, 226,
        194, 137, 32,  111, 242, 182, 15,  226, 194, 201,
    },
    {
        32,  111, 145, 4,   14,  251, 194, 13,  24,  127, 114, 150, 134, 118,
        199, 173, 32,  79,  242, 180, 135, 250, 194, 137, 32,  79,  242, 180,
        135, 250, 194, 137, 40,  79,  242, 180, 135, 250, 194, 137, 166, 107,
        179, 182, 137, 226, 194, 141, 48,  127, 226, 176, 141, 226, 230, 200,
        56,  127, 242, 164, 135, 246, 195, 135, 30,  87,  226, 176, 137, 226,
        230, 168, 56,  95,  242, 180, 135, 242, 194, 161, 32,  79,  242, 182,
        135, 250, 194, 137, 184, 127, 242, 180, 231, 254, 195, 135, 56,  79,
        242, 180, 135, 250, 194, 169, 40,  127, 243, 182, 135, 252, 195, 135,
    },
};
const std::vector<cv::Mat> SelectionRecognizer::SELECTION_DESCRIPTORS = {
    cv::Mat(DATA[0].size() / 8, 8, CV_8U, (void *)DATA[0].data()),
    cv::Mat(DATA[1].size() / 8, 8, CV_8U, (void *)DATA[1].data()),
    cv::Mat(DATA[2].size() / 8, 8, CV_8U, (void *)DATA[2].data()),
    cv::Mat(DATA[3].size() / 8, 8, CV_8U, (void *)DATA[3].data()),
    cv::Mat(DATA[4].size() / 8, 8, CV_8U, (void *)DATA[4].data()),
    cv::Mat(DATA[5].size() / 8, 8, CV_8U, (void *)DATA[5].data()),
};
