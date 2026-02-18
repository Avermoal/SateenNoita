#include "EngineCore/map_generator.h"

#include "EngineCore/tiles_info.h"

#include "Mathematics/perlin_noise.h"

void mapgen(int maph, int mapw, int (*map)[mapw], int seed, int lbc, int rbc)
{

  for(int y = 0; y < maph; ++y){
    for(int x = 0; x < mapw; ++x){
      map[y][x] = ID_000001_GROUND;
    }
  }
}
