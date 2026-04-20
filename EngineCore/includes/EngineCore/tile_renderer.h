#ifndef ENGINECORE_TILE_RENDERER_H
#define ENGINECORE_TILE_RENDERER_H

#include "EngineCore/tile_data.h"

void create_tile_element(struct tile* t);

void rendertile(const tile* t);

void updatetile(const tile* t, float new_x_coord, float new_y_coord);

#endif/*ENGINECORE_TILE_RENDERER_H*/
