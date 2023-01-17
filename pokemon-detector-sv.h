#pragma once

#ifdef __cplusplus
extern "C" {
#endif

enum pokemon_detector_sv_scene {
  POKEMON_DETECTOR_SV_SCENE_UNDEFINED,
  POKEMON_DETECTOR_SV_SCENE_SELECT
};

struct pokemon_detector_sv_config {
  const int screen_width;
  const int screen_height;
  const int num_of_max_opponent_pokemons;
  const int opponent_col_range[2];
  const int opponent_row_range[6][2];
  const int selecting_area_ranges[2][2];
  const int selecting_area_hist_bins;
  const int selecting_area_hist_max_index;
  const double selecting_area_hist_ratio;
};

const struct pokemon_detector_sv_config pokemon_detector_sv_default_config = {
    .screen_width = 1920,
    .screen_height = 1080,
    .num_of_max_opponent_pokemons = 6,
    .opponent_col_range = {1239, 1337},
    .opponent_row_range = {{228, 326},
                           {330, 428},
                           {432, 530},
                           {534, 632},
                           {636, 734},
                           {738, 836}},
    .selecting_area_ranges = {{149, 811}, {139, 842}},
    .selecting_area_hist_bins = 30,
    .selecting_area_hist_max_index = 17,
    .selecting_area_hist_ratio = 0.5,
};

struct pokemon_detector_sv_context;

struct pokemon_detector_sv_context *
pokemon_detector_sv_create(const struct pokemon_detector_sv_config config);

void pokemon_detector_sv_destroy(struct pokemon_detector_sv_context *context);

void pokemon_detector_sv_load_screen(
    struct pokemon_detector_sv_context *context, unsigned char *buf_bgra);

enum pokemon_detector_sv_scene
pokemon_detector_sv_detect_scene(struct pokemon_detector_sv_context *context);

void pokemon_detector_sv_crop_opponent_pokemons(
    struct pokemon_detector_sv_context *context);

void pokemon_detector_sv_export_opponent_pokemon_image(
    struct pokemon_detector_sv_context *context, int index, const char *basedir,
    const char *filename);

#ifdef __cplusplus
}
#endif
