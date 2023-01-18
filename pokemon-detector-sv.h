#pragma once

#ifdef __cplusplus
extern "C" {
#endif

enum pokemon_detector_sv_scene {
  POKEMON_DETECTOR_SV_SCENE_UNDEFINED,
  POKEMON_DETECTOR_SV_SCENE_SELECT_POKEMON,
  POKEMON_DETECTOR_SV_SCENE_BLACK_TRANSITION,
  POKEMON_DETECTOR_SV_SCENE_SELECT_MY_TEAM
};

struct pokemon_detector_sv_hist_classifier {
  const int ranges_col[2];
  const int ranges_row[2];
  const int hist_channel;
  const int hist_bins;
  const int hist_max_index;
  const double hist_ratio;
};

struct pokemon_detector_sv_config {
  const int screen_width;
  const int screen_height;
  const int opponent_col_range[2];
  const int opponent_row_range[6][2];
  const int selection_order_range_col[2];
  const int selection_order_range_row[6][2];
  const int my_team_select_range_col[2];
  const int my_team_select_range_row[6][2];
  const struct pokemon_detector_sv_hist_classifier classifier_lobby_my_select;
  const struct pokemon_detector_sv_hist_classifier
      classifier_lobby_opponent_select;
  const struct pokemon_detector_sv_hist_classifier classifier_black_transition;
  const struct pokemon_detector_sv_hist_classifier classifier_select_my_team;
};

const struct pokemon_detector_sv_config pokemon_detector_sv_default_config = {
    .screen_width = 1920,
    .screen_height = 1080,
    .opponent_col_range = {1239, 1337},
    .opponent_row_range = {{228, 326},
                           {330, 428},
                           {432, 530},
                           {534, 632},
                           {636, 734},
                           {738, 836}},
    .selection_order_range_col = {774, 926},
    .selection_order_range_row = {{147, 196},
                                  {263, 312},
                                  {379, 428},
                                  {495, 544},
                                  {611, 660},
                                  {727, 776}},
    .my_team_select_range_col = {0, 1},
    .my_team_select_range_row = {{}, {}, {}, {}, {}, {}},
    .classifier_lobby_my_select = {.ranges_col = {149, 811},
                                   .ranges_row = {139, 842},
                                   .hist_channel = 0,
                                   .hist_bins = 30,
                                   .hist_max_index = 17,
                                   .hist_ratio = 0.5},
    .classifier_lobby_opponent_select = {.ranges_col = {1229, 1649},
                                         .ranges_row = {227, 836},
                                         .hist_channel = 0,
                                         .hist_bins = 30,
                                         .hist_max_index = 0,
                                         .hist_ratio = 0.8},
    .classifier_black_transition = {.ranges_col = {400, 600},
                                    .ranges_row = {400, 600},
                                    .hist_channel = 2,
                                    .hist_bins = 8,
                                    .hist_max_index = 0,
                                    .hist_ratio = 0.8},
    .classifier_select_my_team = {.ranges_col = {749, 1179},
                                  .ranges_row = {237, 859},
                                  .hist_channel = 2,
                                  .hist_bins = 8,
                                  .hist_max_index = 7,
                                  .hist_ratio = 0.5}};

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

const char *pokemon_detector_sv_recognize_opponent_pokemon(
    struct pokemon_detector_sv_context *context, int index);

int pokemon_detector_sv_recognize_selection_order(
    struct pokemon_detector_sv_context *context, int index);

extern "C" void pokemon_detector_sv_export_image_selection_order(
    struct pokemon_detector_sv_context *context, int index, const char *path);

void pokemon_detector_sv_selection_order_crop(
    struct pokemon_detector_sv_context *context);
int pokemon_detector_sv_selection_order_recognize(
    struct pokemon_detector_sv_context *context, int index);
void pokemon_detector_sv_selection_order_export(
    struct pokemon_detector_sv_context *context, int index, const char *path);

#ifdef __cplusplus
}
#endif
