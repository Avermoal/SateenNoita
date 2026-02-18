#ifndef ENGINECORE_TIELS_MAP_H
#define ENGINECORE_TIELS_MAP_H

#include "EngineCore/display_manager.h"
#include "EngineCore/texture_manager.h"
#include "EngineCore/tiles_info.h"

#define MAP_WIDTH 13
#define MAP_HEIGHT 17

struct GLFWwindow;

struct tile{
  float xcoord;/*OpenGL use float for displaying elements on screen*/
  float ycoord;
  struct element tile;
  short id;
  bool isobstacle;
};

struct gamemap{
  struct tile groundmap[MAP_HEIGHT][MAP_WIDTH];
  struct tile effect[MAP_HEIGHT][MAP_WIDTH];
  struct tile mobs[MAP_HEIGHT][MAP_WIDTH];
  int pcx, pcy; /*Player coordinate x and y*/
};

struct tilemap{
  struct texturearray texarr;
  int xcoord;/*Use pixels coordinate on the window*/
  int ycoord;
  int width;
  int height;
  struct gamemap gmap;
};

void createtilemap(struct tilemap* map, struct GLFWwindow* win);

void destroytilemap(struct tilemap* map);/*Need to destroy elements in tiles*/

void updatetilemap(struct tilemap* map);

void rendertilemap(struct tilemap* map, GLuint shaderprogram, float screenaspect);

void move_mob_on_place(struct gamemap* gmap, enum MOVE_TO_TILE MT_T);

#endif/*ENGINECORE_TIELS_MAP_H*/
