#pragma once

#ifdef __cplusplus
extern "C" {
#endif

struct pokemon_detector_sv_config {
  const int screen_width;
  const int screen_height;
  const int num_of_max_opponent_pokemons;
  const int opponent_col_range[2];
  const int opponent_row_range[6][2];
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
                           {738, 836}}};

struct pokemon_detector_sv_context;

struct pokemon_detector_sv_context *
pokemon_detector_sv_create(const struct pokemon_detector_sv_config config);

void pokemon_detector_sv_destroy(struct pokemon_detector_sv_context *context);

void pokemon_detector_sv_load_screen(
    struct pokemon_detector_sv_context *context, unsigned char *buf_bgra);

void pokemon_detector_sv_crop_opponent_pokemons(
    struct pokemon_detector_sv_context *context);

void pokemon_detector_sv_export_opponent_pokemon_image(
    struct pokemon_detector_sv_context *context, int index, const char *path);

#ifdef __cplusplus
}
#endif
