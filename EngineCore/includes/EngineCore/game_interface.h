#ifndef ENGINECORE_GAME_INTERFACE_H
#define ENGINECORE_GAME_INTERFACE_H

#include "EngineCore/texture_manager.h"
#include "EngineCore/display_manager.h"
#include "EngineCore/tiles_info.h"
#include "EngineCore/inventory.h"

#define INTERFACE_WIDTH (MAP_WIDTH * TILE_SIZE)/2
#define INTERFACE_HEIGHT (MAP_HEIGHT * TILE_SIZE)

struct GLFWwindow;
struct text;

struct interfacepart{
  float xcoord, ycoord; /*OpenGL use float numbers for display figure at window*/
  struct element part;
};

struct container{
  float height, width;
  float xcoord, ycoord; /*Relative coord in the interface panel*/
  struct text* txt;
};

struct gameinterface{
  struct texturearray texarray;
  struct interfacepart left, right;
  struct container spelllist, statslist;
  struct inventory inv;
  struct interfacepart spell_craft_menu;
};

void create_game_interface(struct gameinterface* gint);

void destroy_game_interface(struct gameinterface* gint);

void update_game_interface(struct gameinterface* gint);

void render_game_interface(struct gameinterface* gint, float screenaspect);

#endif/*ENGINECORE_GAME_INTERFACE_H*/
