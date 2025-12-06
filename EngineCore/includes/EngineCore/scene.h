#ifndef ENGINECORE_SCENE_H
#define ENGINECORE_SCENE_H

#include "EngineCore/tiles_map.h"

struct window;

struct scene{
  struct tilemap map;
};

void initscene(struct scene *scn, struct window *win);

void destroyscene(struct scene *scn);

void renderscene(struct window *win);

void updatescene(struct scene *scn);

#endif/*ENGINECORE_SCENE_H*/
