#include <filesystem>
#include <fstream>

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
        opponentPokemonCropper(
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
  struct pokemon_detector_sv_matchstate matchstate;

  cv::Mat screenBGRA, screenBGR, screenHSV;

  SceneDetector sceneDetector;

  EntityCropper opponentPokemonCropper;
  PokemonRecognizer pokemonRecognizer;

  EntityCropper selectionOrderCropper;
  SelectionRecognizer selectionOrderRecognizer;

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

extern "C" void pokemon_detector_sv_opponent_pokemon_crop(
    struct pokemon_detector_sv_context *context) {
  context->opponentPokemonCropper.crop(context->screenBGRA);
  context->opponentPokemonCropper.generateMask();
}

extern "C" void pokemon_detector_sv_opponent_pokemon_export_image(
    struct pokemon_detector_sv_context *context, int index, const char *basedir,
    const char *filename) {
  std::filesystem::path filepath(basedir);
  filepath /= filename;
  cv::imwrite(filepath.c_str(),
              context->opponentPokemonCropper.imagesBGRA[index]);
}

extern "C" const char *pokemon_detector_sv_opponent_pokemon_recognize(
    struct pokemon_detector_sv_context *context, int index) {
  auto id = context->pokemonRecognizer.recognizePokemon(
      context->opponentPokemonCropper.imagesBGR[index],
      context->opponentPokemonCropper.masks[index]);
  id.copy(context->matchstate.opponent_pokemon_ids[index],
          sizeof(context->matchstate.opponent_pokemon_ids[0]) - 1);
  return context->matchstate.opponent_pokemon_ids[index];
}

extern "C" void pokemon_detector_sv_my_selection_order_crop(
    struct pokemon_detector_sv_context *context) {
  context->selectionOrderCropper.crop(context->screenBGRA);
}

extern "C" int pokemon_detector_sv_my_selection_order_recognize(
    struct pokemon_detector_sv_context *context, int index) {
  auto order = context->selectionOrderRecognizer.recognizeSelection(
      context->selectionOrderCropper.imagesBGR[index]);
  context->matchstate.my_selection_order[index] = order;
  return order;
}

extern "C" void pokemon_detector_sv_my_selection_order_export_image(
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
  auto result = context->resultRecognizer.recognizeResult(
      context->resultCropper.imagesBGR[0]);
  context->matchstate.result = result;
  return result;
}

extern "C" void pokemon_detector_sv_matchstate_clear(
    struct pokemon_detector_sv_context *context) {
  auto &matchstate = context->matchstate;
  matchstate.result = POKEMON_DETECTOR_SV_RESULT_UNKNOWN;
  std::memset(matchstate.my_selection_order, 0,
              sizeof(matchstate.my_selection_order));
  std::memset(matchstate.opponent_pokemon_ids, 0,
              sizeof(matchstate.opponent_pokemon_ids));
}

extern "C" void pokemon_detector_sv_matchstate_append(
    struct pokemon_detector_sv_context *context, const char *filepath) {
  std::ofstream ofs(filepath, std::ios_base::app);
  auto &matchstate = context->matchstate;
  if (matchstate.result == POKEMON_DETECTOR_SV_RESULT_UNKNOWN) {
    ofs << "UNKNOWN";
  } else if (matchstate.result == POKEMON_DETECTOR_SV_RESULT_LOSE) {
    ofs << "LOSE";
  } else if (matchstate.result == POKEMON_DETECTOR_SV_RESULT_WIN) {
    ofs << "WIN";
  }
  ofs << "\t";
  for (int i = 0; i < 6; i++) {
    ofs << matchstate.my_selection_order[i] << "\t";
  }
  for (int i = 0; i < 6; i++) {
    ofs << matchstate.opponent_pokemon_ids[i] << "\t";
  }
  ofs << "\n";
}