#ifndef ENGINECORE_TILE_DATA_H
#define ENGINECORE_TILE_DATA_H

struct stats;

struct tile{
  float xcoord;/*OpenGL use float for displaying elements on screen*/
  float ycoord;
  struct element tile;
  short id;
  bool isobstacle;
  float layer; /*OpenGL's map layers*/
  struct stats* stts;
};

#endif/*ENGINECORE_TILE_DATA_H*/
