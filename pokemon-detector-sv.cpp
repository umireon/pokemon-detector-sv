#include "pokemon-detector-sv.h"
#include <filesystem>
#include <opencv2/opencv.hpp>

#include "EntityCropper.h"
#include "SceneDetector.h"

static constexpr std::array<int, 2>
convertInt2ToStdArray2(const int cArray[2]) {
  return std::array<int, 2>{cArray[0], cArray[1]};
}

static constexpr std::array<std::array<int, 2>, 6>
convertInt62ToStdArray62(const int cArray[6][2]) {
  return std::array<std::array<int, 2>, 6>{
      convertInt2ToStdArray2(cArray[0]), convertInt2ToStdArray2(cArray[1]),
      convertInt2ToStdArray2(cArray[2]), convertInt2ToStdArray2(cArray[3]),
      convertInt2ToStdArray2(cArray[4]), convertInt2ToStdArray2(cArray[5]),
  };
}

extern "C" struct pokemon_detector_sv_context {
  pokemon_detector_sv_context(const struct pokemon_detector_sv_config config)
      : config(config), sceneDetector(this->config),
        opponentPokemonsCropper(
            convertInt2ToStdArray2(config.opponent_col_range),
            convertInt62ToStdArray62(config.opponent_row_range)) {}

  const struct pokemon_detector_sv_config config;
  cv::Mat screenBGRA, screenBGR, screenHSV;
  SceneDetector sceneDetector;
  EntityCropper opponentPokemonsCropper;
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
  cv::cvtColor(context->screenBGRA, context->screenBGR, cv::COLOR_BGRA2BGR);
  cv::cvtColor(context->screenBGR, context->screenHSV, cv::COLOR_BGR2HSV);
}

extern "C" enum pokemon_detector_sv_scene
pokemon_detector_sv_detect_scene(struct pokemon_detector_sv_context *context) {
  return context->sceneDetector.detectScene(context->screenHSV);
}

extern "C" void pokemon_detector_sv_crop_opponent_pokemons(
    struct pokemon_detector_sv_context *context) {
  context->opponentPokemonsCropper.crop(context->screenBGRA);
}

extern "C" void pokemon_detector_sv_export_opponent_pokemon_image(
    struct pokemon_detector_sv_context *context, int index, const char *basedir,
    const char *filename) {
  std::filesystem::path filepath(basedir);
  filepath /= filename;
  cv::imwrite(filepath.c_str(),
              context->opponentPokemonsCropper.imagesBGRA[index]);
}
