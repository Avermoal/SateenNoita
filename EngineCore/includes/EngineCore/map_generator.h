#ifndef ENGINECORE_MAP_GENERATOR_H
#define ENGINECORE_MAP_GENERATOR_H

#include "EngineCore/tile_data.h"

void init_map_generator(int seed);

/*lbc --- left bottom corner coordinate*/
void map_ground_gen(int maph, int mapw, struct tile (*map)[mapw], int lbc);

void map_mobs_gen(int maph, int mapw, struct tile (*map)[mapw], int lbc);

void map_items_gen(int maph, int mapw, struct tile (*map)[mapw], int lbc);

#endif/*ENGINECORE_MAP_GENERATOR_H*/
