#ifndef ENGINECORE_GAME_INTERFACE_H
#define ENGINECORE_GAME_INTERFACE_H

#include "EngineCore/texture_manager.h"
#include "EngineCore/display_manager.h"

#define INTERFACE_WIDTH 152
#define INTERFACE_HEIGHT 368

struct GLFWwindow;

struct interfacepart{
  float xcoord, ycoord; /*OpenGL use float numbers for display figure at window*/
  struct element part;
};

struct gameinterface{
  struct texturearray texarray;
  struct interfacepart left, right;
  struct interfacepart spelllist, statslist;
  struct interfacepart inventory, spell_craft_menu;
};

void create_game_interface(struct gameinterface* gint);

void destroy_game_interface(struct gameinterface* gint);

void update_game_interface(struct gameinterface* gint);

void render_game_interface(struct gameinterface* gint, GLuint shaderprogram, float screenaspect);

#endif/*ENGINECORE_GAME_INTERFACE_H*/
