#ifndef ENGINECORE_TIELS_MAP_H
#define ENGINECORE_TIELS_MAP_H

#include "EngineCore/display_manager.h"
#include "EngineCore/texture_manager.h"
#include "EngineCore/tiles_info.h"

#define MAP_WIDTH 19
#define MAP_HEIGHT 23

struct GLFWwindow;

struct tile{
  float xcoord;/*OpenGL use float for displaying elements on screen*/
  float ycoord;
  struct element tile;
  short id;
  bool isobstacle;
  float layer;
};

struct gamemap{
  struct tile groundmap[MAP_HEIGHT][MAP_WIDTH];
  struct tile effect[MAP_HEIGHT][MAP_WIDTH];
  struct tile mobs[MAP_HEIGHT][MAP_WIDTH];
  int pcx, pcy; /*Player coordinate x and y*/
  int border; /*Border*/
  int genoffset;
};

struct tilemap{
  struct texturearray texarr;
  struct gamemap gmap;
};

void createtilemap(struct tilemap* map);

void destroytilemap(struct tilemap* map);/*Need to destroy elements in tiles*/

void updatetilemap(struct tilemap* map);

void rendertilemap(struct tilemap* map, GLuint shaderprogram, float screenaspect);

bool nearest_swap_tile(struct tile (*map)[MAP_WIDTH], int h1, int w1, int h2, int w2);

void move_player_on_place(struct gamemap* gmap, enum MOVE_TO_TILE MT_T);
void move_mob_on_place(struct gamemap* gmap);

#endif/*ENGINECORE_TIELS_MAP_H*/
