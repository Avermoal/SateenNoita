#include "EngineCore/map_generator.h"

#include "EngineCore/tiles_info.h"

#include "Mathematics/perlin_noise.h"

void mapgen(int maph, int mapw, struct tile (*map)[mapw], int seed, int lbc, int rbc)
{

  for(int y = 0; y < maph; ++y){
    for(int x = 0; x < mapw; ++x){
      map[y][x].id = ID_000001_GROUND;
      map[y][x].isobstacle = false;
    }
  }
}
