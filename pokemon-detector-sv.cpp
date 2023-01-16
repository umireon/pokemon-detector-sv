#include "pokemon-detector-sv.h"
#include <opencv2/opencv.hpp>
#include <filesystem>

extern "C" struct pokemon_detector_sv_context {
  pokemon_detector_sv_context(const struct pokemon_detector_sv_config config)
      : config(config),
        opponentPokemonImages(config.num_of_max_opponent_pokemons, cv::Mat()),
        opponentPokemonMasks(config.num_of_max_opponent_pokemons, cv::Mat()) {}

  struct pokemon_detector_sv_config config;
  cv::Mat screenBGRA;
  std::vector<cv::Mat> opponentPokemonImages;
  std::vector<cv::Mat> opponentPokemonMasks;
};

extern "C" struct pokemon_detector_sv_context *
pokemon_detector_sv_create(const struct pokemon_detector_sv_config config) {
  return new pokemon_detector_sv_context(config);
}

extern "C" void
pokemon_detector_sv_destroy(struct pokemon_detector_sv_context *context) {
  delete context;
}

extern "C" void
pokemon_detector_sv_load_screen(struct pokemon_detector_sv_context *context,
                                unsigned char *buf_bgra) {
  auto &config = context->config;
  context->screenBGRA =
      cv::Mat(config.screen_height, config.screen_width, CV_8UC4, buf_bgra);
}

extern "C" void pokemon_detector_sv_crop_opponent_pokemons(
    struct pokemon_detector_sv_context *context) {
  const auto &config = context->config;
  const auto &image = context->screenBGRA;
  for (int i = 0; i < config.num_of_max_opponent_pokemons; i++) {
    const cv::Range colRange(config.opponent_col_range[0],
                             config.opponent_col_range[1]),
        rowRange(config.opponent_row_range[i][0],
                 config.opponent_row_range[i][1]);
    auto &pokemonImageBGRA = context->opponentPokemonImages[i];
    pokemonImageBGRA = image(rowRange, colRange);

    cv::Mat pokemonImageBGR, mask;
    cv::cvtColor(pokemonImageBGRA, pokemonImageBGR, cv::COLOR_BGRA2BGR);
    cv::Point seedPoint(0, 0);
    cv::floodFill(pokemonImageBGR, mask, seedPoint, 0, 0, 0, 0,
                  cv::FLOODFILL_MASK_ONLY);

    auto &pokemonMask = context->opponentPokemonMasks[i];
    cv::Range cropCol(1, mask.cols - 1), cropRow(1, mask.rows - 1);
    pokemonMask = 1 - mask(cropCol, cropRow);
    for (int x = 0; x < pokemonImageBGRA.cols; x++) {
      for (int y = 0; y < pokemonImageBGRA.rows; y++) {
        pokemonImageBGRA.at<cv::Vec4b>(y, x)[3] =
            pokemonMask.at<unsigned char>(y, x) * 255;
      }
    }
  }
}

extern "C" void pokemon_detector_sv_export_opponent_pokemon_image(
    struct pokemon_detector_sv_context *context, int index, const char *basedir,
    const char *filename) {
  std::filesystem::path filepath(basedir);
  filepath /= filename;
  cv::imwrite(filepath.c_str(), context->opponentPokemonImages[index]);
}