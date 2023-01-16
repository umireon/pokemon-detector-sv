#pragma once

extern "C" {
struct pokemon_detector_sv_config {
  const int screen_width = 1920;
  const int screen_height = 1080;
  const int num_of_max_opponent_pokemons = 6;
  const int opponent_col_range[2]{1239, 1337};
  const int opponent_row_range[6][2]{{228, 326}, {330, 428}, {432, 530},
                                     {534, 632}, {636, 734}, {738, 836}};
};

struct pokemon_detector_sv_context;

struct pokemon_detector_sv_context *
pokemon_detector_sv_create(const struct pokemon_detector_sv_config config);

void pokemon_detector_sv_destroy(struct pokemon_detector_sv_context *context);

void pokemon_detector_sv_load_screen(
    struct pokemon_detector_sv_context *context, unsigned char *buf_bgra);
}

void pokemon_detector_sv_crop_opponent_pokemons(
    struct pokemon_detector_sv_context *context);

void pokemon_detector_sv_export_opponent_pokemon_image(
    struct pokemon_detector_sv_context *context, int index, const char *path);