#ifndef ENGINECORE_MAP_DATA_H
#define ENGINECORE_MAP_DATA_H

#include "EngineCore/tile_data.h"

#define MAP_WIDTH 19
#define MAP_HEIGHT 23

struct gamemap{
  struct tile ground[MAP_HEIGHT][MAP_WIDTH];
  struct tile mobs[MAP_HEIGHT][MAP_HEIGHT];
  struct tile items[MAP_HEIGHT][MAP_WIDTH];
  int pcx, pcy; /*Player coordinates*/
  int border;
  int genoffset;
};

void game_map_init(struct gamemap* gmap);

void game_map_destroy();

void game_map_swap_lines();

void game_map_clear_line();

#endif/*ENGINECORE_MAP_DATA_H*/
