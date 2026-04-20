#include "EngineCore/map_data.h"

#include <string.h>
#include <stdlib.h>

#include "EngineCore/map_generator.h"
#include "EngineCore/stats_system.h"

#define PLAYER_MAP_BORDER (MAP_HEIGHT/2)

void game_map_init(struct gamemap* gmap)
{
  memset(gamp, 0, sizeof(struct gamemap));
  /*Stats memory allocation for mobs array*/
  for(int y = 0; y < MAP_HEIGHT; ++y){
    for(int x = 0; x < MAP_WIDTH; ++x){
      gmap->mobs[y][x].stts = calloc(1, sizeof(struct stats));
    }
  }
  /*Map generate*/
  gmap->border = PLAYER_MAP_BORDER;
  const int lbc = 0; /*Start left bottom corner coord*/
  map_ground_gen(MAP_HEIGHT, MAP_WIDTH, gmap->ground, lbc);
  map_mobs_gen(MAP_HEIGHT, MAP_WIDTH, gmap->mobs, lbc);
  map_items_gen(MAP_HEIGHT, MAP_WIDTH, gmap->items, lbc);
}

void game_map_destroy();

void game_map_swap_lines();

void game_map_clear_line();

