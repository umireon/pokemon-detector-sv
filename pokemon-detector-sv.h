#pragma once

#ifdef __cplusplus
extern "C" {
#endif

enum pokemon_detector_sv_scene {
  POKEMON_DETECTOR_SV_SCENE_UNDEFINED,
  POKEMON_DETECTOR_SV_SCENE_SELECT_POKEMON,
  POKEMON_DETECTOR_SV_SCENE_BLACK_TRANSITION
};

struct pokemon_detector_sv_hist_classifier {
  const int range_col[2];
  const int range_row[2];
  const int hist_channel;
  const int hist_bins;
  const int hist_max_index;
  const double hist_ratio;
};

struct pokemon_detector_sv_config {
  const int opponent_col_range[2];
  const int opponent_row_range[6][2];
  const int selection_order_range_col[2];
  const int selection_order_range_row[6][2];
  const struct pokemon_detector_sv_hist_classifier classifier_lobby_my_select;
  const struct pokemon_detector_sv_hist_classifier
      classifier_lobby_opponent_select;
  const struct pokemon_detector_sv_hist_classifier classifier_black_transition;
  const struct pokemon_detector_sv_hist_classifier classifier_select_my_team;
};

const struct pokemon_detector_sv_config pokemon_detector_sv_default_config = {
    .opponent_col_range = {1239, 1337},
    .opponent_row_range = {{228, 326},
                           {330, 428},
                           {432, 530},
                           {534, 632},
                           {636, 734},
                           {738, 836}},
    .selection_order_range_col = {795, 827},
    .selection_order_range_row = {{154, 186},
                                  {271, 303},
                                  {388, 420},
                                  {503, 535},
                                  {619, 651},
                                  {735, 767}},
    .classifier_lobby_my_select = {.range_col = {149, 811},
                                   .range_row = {139, 842},
                                   .hist_channel = 0,
                                   .hist_bins = 30,
                                   .hist_max_index = 17,
                                   .hist_ratio = 0.5},
    .classifier_lobby_opponent_select = {.range_col = {1229, 1649},
                                         .range_row = {227, 836},
                                         .hist_channel = 0,
                                         .hist_bins = 30,
                                         .hist_max_index = 0,
                                         .hist_ratio = 0.8},
    .classifier_black_transition = {.range_col = {400, 600},
                                    .range_row = {400, 600},
                                    .hist_channel = 2,
                                    .hist_bins = 8,
                                    .hist_max_index = 0,
                                    .hist_ratio = 0.8}};

struct pokemon_detector_sv_context;

struct pokemon_detector_sv_context *
pokemon_detector_sv_create(const struct pokemon_detector_sv_config config);

void pokemon_detector_sv_destroy(struct pokemon_detector_sv_context *context);

void pokemon_detector_sv_load_screen(
    struct pokemon_detector_sv_context *context, void *buf_bgra,
    int width, int height);

enum pokemon_detector_sv_scene
pokemon_detector_sv_detect_scene(struct pokemon_detector_sv_context *context);

void pokemon_detector_sv_crop_opponent_pokemons(
    struct pokemon_detector_sv_context *context);

void pokemon_detector_sv_export_opponent_pokemon_image(
    struct pokemon_detector_sv_context *context, int index, const char *basedir,
    const char *filename);

const char *pokemon_detector_sv_recognize_opponent_pokemon(
    struct pokemon_detector_sv_context *context, int index);

int pokemon_detector_sv_recognize_selection_order(
    struct pokemon_detector_sv_context *context, int index);

void pokemon_detector_sv_selection_order_crop(
    struct pokemon_detector_sv_context *context);
int pokemon_detector_sv_selection_order_recognize(
    struct pokemon_detector_sv_context *context, int index);
void pokemon_detector_sv_selection_order_export(
    struct pokemon_detector_sv_context *context, int index, const char *path,
    bool shouldBeBlank);

#ifdef __cplusplus
}
#endif
