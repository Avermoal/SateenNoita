#include "EngineCore/map_generator.h"

#include "Mathematics/noises.h"

#define OAK_BOUND 0.09f
#define TREE_BOUND 0.0f
#define FIR_BOUND -0.09f
#define GRASS_BOUND_TOP -0.32f
#define GRASS_BOUND_BOTTOM -0.7f
#define MOBS_PLACE_BOUND_TOP 0.14f
#define MOBS_PLACE_BOUND_BOTTOM 0.12f

//@TODO Написать функцию устанавливающую препятсвия(любые)
static void setobstacles(int maph, int mapw, struct tile (*map)[mapw])
{

}

void init_map_generator(int seed)
{
  init_perlin_noise(seed);
}

void mapgen(int maph, int mapw, struct tile (*map)[mapw], int lbc, enum META_TILE_TYPE tt)
{
  /*Perlin noise map generation*/
  float hieghtmap[maph][mapw];
  perlin_noise_map_gen(maph, mapw, hieghtmap, lbc);
  switch(tt){
    case GROUND_TILE:
      /*Map generation*/
      for(int y = 0; y < maph; ++y){
        for(int x = 0; x < mapw; ++x){
          map[y][x].id = ID_000001_GROUND;
          map[y][x].isobstacle = false;
        }
      }
      for(int y = 0; y < maph; ++y){
        for(int x = 0; x < mapw; ++x){
          if(hieghtmap[y][x] < OAK_BOUND && hieghtmap[y][x] > TREE_BOUND){
            map[y][x].id = ID_000010_TREE_OAK;
            map[y][x].isobstacle = true;
          }
          if(hieghtmap[y][x] > FIR_BOUND && hieghtmap[y][x] < TREE_BOUND){
            map[y][x].id = ID_000011_TREE_FIR;
            map[y][x].isobstacle = true;
          }
          if(hieghtmap[y][x] > GRASS_BOUND_BOTTOM && hieghtmap[y][x] < GRASS_BOUND_TOP){
            map[y][x].id = ID_000002_GRASS;
            map[y][x].isobstacle = false;
          }
        }
      }
      break;
    case MOBS_TILE:
      for(int y = 0; y < maph; ++y){
        for(int x = 0; x < mapw; ++x){
          map[y][x].id = ID_000000_VOID;
          map[y][x].isobstacle = false;
        }
      }
      /*Set mobs*/
      if(lbc > MAP_HEIGHT){
        for(int y = 0; y < maph; ++y){
          for(int x = 0; x < mapw; ++x){
            if(hieghtmap[y][x] < MOBS_PLACE_BOUND_TOP && hieghtmap[y][x] > MOBS_PLACE_BOUND_BOTTOM){
              map[y][x].id = ID_000014_GOLEM;
              map[y][x].isobstacle = true;
            }
          }
        }
      }
      break;
    default: return;
  }
}
