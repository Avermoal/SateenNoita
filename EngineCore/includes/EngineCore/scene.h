#ifndef ENGINECORE_SCENE_H
#define ENGINECORE_SCENE_H

#include "EngineCore/tiles_map.h"

struct GLFWwindow_;
struct window;

struct scene{
  struct tilemap map;
};

void initscene(struct scene *scn, struct GLFWwindow_ *win);

void destroyscene(struct scene *scn);

void renderscene(struct scene *scn, struct window *win);

void updatescene(struct scene *scn);

#endif/*ENGINECORE_SCENE_H*/
