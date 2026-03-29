#ifndef ENGINECORE_SCENE_H
#define ENGINECORE_SCENE_H

#include "EngineCore/tiles_map.h"
#include "EngineCore/game_interface.h"

struct window;

struct scene{
  struct tilemap map;
  struct gameinterface gint;
};

void initscene(struct scene* scn, struct window* win);

void destroyscene(struct scene** scn);

void renderscene(struct window* win);

void updatescene(struct scene* scn);

#endif/*ENGINECORE_SCENE_H*/
