#ifndef ENGINECORE_MAP_GENERATOR_H
#define ENGINECORE_MAP_GENERATOR_H

#include "EngineCore/tiles_map.h"

/*lbc, rbc --- left bottom corner coordinate and right bottom coordinate*/
void mapgen(int maph, int mapw, struct tile (*map)[mapw], int seed, int lbc, int rbc);

#endif/*ENGINECORE_MAP_GENERATOR_H*/
