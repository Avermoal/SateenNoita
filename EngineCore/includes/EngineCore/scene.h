#ifndef ENGINECORE_SCENE_H
#define ENGINECORE_SCENE_H

#include "EngineCore/display_manager.h"

#define CELLS_NUMBER 117/*9x13 cells space*/

struct scene{
  const float fov;
  struct element gamefield[CELLS_NUMBER];/*9x13 cells space*/
};

void initscene(struct scene *scn);

void destroyscene(struct scene *scn);

void renderscene(struct scene *scn);



#endif/*ENGINECORE_SCENE_H*/
