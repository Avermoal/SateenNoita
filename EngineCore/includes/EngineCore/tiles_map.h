#ifndef ENGINECORE_TIELS_MAP_H
#define ENGINECORE_TIELS_MAP_H

#include <glad/glad.h>

#include "EngineCore/display_manager.h"

#define MAP_WIDTH 9
#define MAP_HEIGHT 13
#define CELLS_NUMBER (MAP_WIDTH * MAP_HEIGHT) /*117*/
#define TILE_SIZE 16

enum TILE_TYPE{
  GROUND = 0,
  GRASS = 1,
  WALL,
};

struct tile{
  float xcoord;/*OpenGL use float for displaying elements on screen*/
  float ycoord;
  struct element tile;
  short id;
};

struct tilemap{
  int xcoord;/*Use pixels coordinate on the window*/
  int ycoord;
  int width;
  int height;
  struct tile gamemap[CELLS_NUMBER];
};

void createtilemap(struct tilemap *map, GLFWwindow *win);

void destroytilemap(struct tilemap *map);/*Need to destroy elements in tiles*/

void updatetilemap(struct tilemap *map);

void rendertilemap(struct tilemap *map, GLuint shaderprogram, float screenaspect);

#endif/*ENGINECORE_TIELS_MAP_H*/
