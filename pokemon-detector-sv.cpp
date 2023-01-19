#include <filesystem>

#include <opencv2/opencv.hpp>

#include "pokemon-detector-sv.h"

#include "EntityCropper.h"
#include "PokemonRecognizer.h"
#include "ResultRecognizer.h"
#include "SceneDetector.h"
#include "SelectionRecognizer.h"

static constexpr std::array<int, 2>
convertInt2ToStdArray2(const int cArray[2]) {
  return std::array<int, 2>{cArray[0], cArray[1]};
}

static const std::vector<std::array<int, 2>>
convertInt62ToVector6Array2(const int cArray[6][2]) {
  return std::vector<std::array<int, 2>>{
      convertInt2ToStdArray2(cArray[0]), convertInt2ToStdArray2(cArray[1]),
      convertInt2ToStdArray2(cArray[2]), convertInt2ToStdArray2(cArray[3]),
      convertInt2ToStdArray2(cArray[4]), convertInt2ToStdArray2(cArray[5]),
  };
}

extern "C" struct pokemon_detector_sv_context {
  pokemon_detector_sv_context(const struct pokemon_detector_sv_config config)
      : config(config), sceneDetector(config.classifier_lobby_my_select,
                                      config.classifier_lobby_opponent_select,
                                      config.classifier_black_transition,
                                      config.classifier_select_my_team),
        opponentPokemonsCropper(
            convertInt2ToStdArray2(config.opponent_col_range),
            convertInt62ToVector6Array2(config.opponent_row_range)),
        selectionOrderCropper(
            convertInt2ToStdArray2(config.selection_order_range_col),
            convertInt62ToVector6Array2(config.selection_order_range_row)),
        resultCropper(convertInt2ToStdArray2(config.result_range_col),
                      std::vector<std::array<int, 2>>{
                          convertInt2ToStdArray2(config.result_range_row[0])}),
        resultRecognizer(config.result_n_bins, config.result_lose_max_index,
                         config.result_lose_ratio, config.result_win_max_index,
                         config.result_win_max_index) {}

  const struct pokemon_detector_sv_config config;
  cv::Mat screenBGRA, screenBGR, screenHSV;
  SceneDetector sceneDetector;
  EntityCropper opponentPokemonsCropper;
  PokemonRecognizer pokemonRecognizer;
  std::array<std::string, 6> opponentPokemonIds;
  EntityCropper selectionOrderCropper;
  SelectionRecognizer selectionRecognizer;
  std::array<int, 6> selectionOrderIndex;
  EntityCropper resultCropper;
  ResultRecognizer resultRecognizer;
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
                                void *buf_bgra, int width, int height) {
  context->screenBGRA = cv::Mat(height, width, CV_8UC4, buf_bgra);
}

extern "C" enum pokemon_detector_sv_scene
pokemon_detector_sv_detect_scene(struct pokemon_detector_sv_context *context) {
  cv::Mat screenBGR, screenHSV;
  cv::cvtColor(context->screenBGRA, screenBGR, cv::COLOR_BGRA2BGR);
  cv::cvtColor(screenBGR, screenHSV, cv::COLOR_BGR2HSV);
  return context->sceneDetector.detectScene(screenHSV);
}

extern "C" void pokemon_detector_sv_crop_opponent_pokemons(
    struct pokemon_detector_sv_context *context) {
  context->opponentPokemonsCropper.crop(context->screenBGRA);
  context->opponentPokemonsCropper.generateMask();
}

extern "C" void pokemon_detector_sv_export_opponent_pokemon_image(
    struct pokemon_detector_sv_context *context, int index, const char *basedir,
    const char *filename) {
  std::filesystem::path filepath(basedir);
  filepath /= filename;
  cv::imwrite(filepath.c_str(),
              context->opponentPokemonsCropper.imagesBGRA[index]);
}

extern "C" const char *pokemon_detector_sv_recognize_opponent_pokemon(
    struct pokemon_detector_sv_context *context, int index) {
  context->opponentPokemonIds[index] =
      context->pokemonRecognizer.recognizePokemon(
          context->opponentPokemonsCropper.imagesBGR[index],
          context->opponentPokemonsCropper.masks[index]);
  return context->opponentPokemonIds[index].data();
}

extern "C" void pokemon_detector_sv_crop_my_team_pokemons(
    struct pokemon_detector_sv_context *context) {
  context->selectionOrderCropper.crop(context->screenBGRA);
  context->selectionOrderCropper.generateMask();
}

extern "C" void pokemon_detector_sv_selection_order_crop(
    struct pokemon_detector_sv_context *context) {
  context->selectionOrderCropper.crop(context->screenBGRA);
}

extern "C" int pokemon_detector_sv_selection_order_recognize(
    struct pokemon_detector_sv_context *context, int index) {
  context->selectionOrderIndex[index] =
      context->selectionRecognizer.recognizeSelection(
          context->selectionOrderCropper.imagesBGR[index]);
  return context->selectionOrderIndex[index];
}

extern "C" void pokemon_detector_sv_selection_order_export(
    struct pokemon_detector_sv_context *context, int index, const char *path,
    bool shouldBeBlank) {
  cv::Mat image = context->selectionOrderCropper.imagesBGRA[index];
  if (shouldBeBlank) {
    image *= 0;
  }
  cv::imwrite(path, image);
}

extern "C" void
pokemon_detector_sv_result_crop(struct pokemon_detector_sv_context *context) {
  context->resultCropper.crop(context->screenBGRA);
}

extern "C" enum pokemon_detector_sv_result pokemon_detector_sv_result_recognize(
    struct pokemon_detector_sv_context *context) {
  return context->resultRecognizer.recognizeResult(
      context->resultCropper.imagesBGR[0]);
}
