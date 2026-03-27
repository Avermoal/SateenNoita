#ifndef ENGINECORE_MAP_GENERATOR_H
#define ENGINECORE_MAP_GENERATOR_H

#include "EngineCore/tiles_map.h"
#include "EngineCore/tiles_info.h"

void init_map_generator(int seed);

/*lbc --- left bottom corner coordinate*/
void mapgen(int maph, int mapw, struct tile (*map)[mapw], int lbc, enum META_TILE_TYPE tt);

#endif/*ENGINECORE_MAP_GENERATOR_H*/
