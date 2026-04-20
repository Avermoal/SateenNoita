#include "EngineCore/map_generator.h"

#include "EngineCore/tiles_info.h"

#include "Mathematics/noises.h"

#define OAK_BOUND 0.09f
#define TREE_BOUND 0.0f
#define FIR_BOUND -0.09f
#define GRASS_BOUND_TOP -0.32f
#define GRASS_BOUND_BOTTOM -0.7f
#define MOBS_PLACE_BOUND_TOP 0.14f
#define MOBS_PLACE_BOUND_BOTTOM 0.12f
#define CHEST_BOUND_TOP -0.4f
#define CHEST_BOUND_BOTTOM -0.42f

void init_map_generator(int seed)
{
  init_perlin_noise(seed);
}

void map_ground_gen(int maph, int mapw, struct tile (*map)[mapw], int lbc)
{
  float heightmap[maph][mapw];
  perlin_noise_map_gen(maph, mapw, heightmap, lbc);
  for(int y = 0; y < maph; ++y){
    for(int x = 0; x < mapw; ++x){
      map[y][x].id = ID_000001_GROUND;
      map[y][x].isobstacle = false;
      map[y][x].xcoord = x * TILE_SIZE;
      map[y][x].ycoord = y * TILE_SIZE;
      map[y][x].layer = GROUND_LAYER;
      map[y][x].stts = NULL;
    }
  }
  for(int y = 0; y < maph; ++y){
    for(int x = 0; x < mapw; ++x){
      if(heightmap[y][x] < OAK_BOUND && heightmap[y][x] > TREE_BOUND){
        map[y][x].id = ID_000010_TREE_OAK;
        map[y][x].isobstacle = true;
      }
      if(heightmap[y][x] > FIR_BOUND && heightmap[y][x] < TREE_BOUND){
        map[y][x].id = ID_000011_TREE_FIR;
        map[y][x].isobstacle = true;
      }
      if(heightmap[y][x] > GRASS_BOUND_BOTTOM && heightmap[y][x] < GRASS_BOUND_TOP){
        map[y][x].id = ID_000002_GRASS;
      }
    }
  }
}

#define GOLEM_HP_MP 10
#define CHEST_HP_MP 1

void map_mobs_gen(int maph, int mapw, struct tile (*map)[mapw], int lbc)
{
  float heightmap[maph][mapw];
  perlin_noise_map_gen(maph, mapw, heightmap, lbc);
  for(int y = 0; y < maph; ++y){
    for(int x = 0; x < mapw; ++x){
      map[y][x].id = ID_000000_VOID;
      map[y][x].isobstacle = false;
      map[y][x].xcoord = x * TILE_SIZE;
      map[y][x].ycoord = y * TILE_SIZE;
      map[y][x].layer = MOBS_LAYER;
    }
  }
  /*Set mobs*/
  if(lbc > MAP_HEIGHT){
    for(int y = 0; y < maph; ++y){
      for(int x = 0; x < mapw; ++x){
        if(heightmap[y][x] < MOBS_PLACE_BOUND_TOP && heightmap[y][x] > MOBS_PLACE_BOUND_BOTTOM){
          map[y][x].id = ID_000014_GOLEM;
          map[y][x].isobstacle = true;
          map[y][x].stts->HP = GOLEM_HP_MP;
          map[y][x].stts->MP = GOLEM_HP_MP;
        }
      }
    }
  }
  /*Set chests*/
  for(int y = 0; y < maph; ++y){
    for(int x = 0; x < mapw; ++x){
      if(heightmap[y][x] < CHEST_BOUND_TOP && heightmap[y][x] > CHEST_BOUND_BOTTOM){
        map[y][x].id = ID_000013_CHEST;
        map[y][x].isobstacle = true;
        map[y][x].stts->HP = CHEST_HP_MP;
        map[y][x].stts->MP = CHEST_HP_MP;
      }
    }
  }
}

void map_items_gen(int maph, int mapw, struct tile (*map)[mapw], int lbc)
{
  float heightmap[maph][mapw];
  perlin_noise_map_gen(maph, mapw, heightmap, lbc);
  for(int y = 0; y < maph; ++y){
    for(int x = 0; x < mapw; ++x){
      map[y][x].id = ID_000000_VOID;
      map[y][x].isobstacle = false;
      map[y][x].xcoord = x * TILE_SIZE;
      map[y][x].ycoord = y * TILE_SIZE;
      map[y][x].layer = ITEMS_LAYER;
      map[y][x].stts = NULL
    }
  }
}

