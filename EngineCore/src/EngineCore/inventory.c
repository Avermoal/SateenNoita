#include "EngineCore/inventory.h"

#include <string.h>

#include <glad/glad.h>

#include "EngineCore/texture_manager.h"
#include "EngineCore/game_window_info.h"
#include "EngineCore/tiles_info.h"

#define INVENTORY_PATH_TO_TEXTURES "res/textures/inventory"
#define INVENTORY_SIZE 150
#define ONCE_INVENTORY 1

#define ITEMS_PATH_TO_TEXTURES "res/textures/items"
#define ITEMS_TEXTURE_START_COORD 0.0f

struct inventory createinventory(float relx, float rely, float width, float height)
{
  struct inventory inv;
  inv.xcoord = relx;
  inv.ycoord = rely;
  inv.width = width;
  inv.height = height;
  inv.itcount = 0;
  memset(inv.it, 0, sizeof(struct item)*MAX_ITEM_COUNT);
  /*Set tex array*/
  load_texture_array(&inv.texarray, INVENTORY_SIZE, INVENTORY_SIZE, ONCE_INVENTORY, INVENTORY_PATH_TO_TEXTURES);
  load_texture_array(&inv.items_tex_array, TILE_SIZE, TILE_SIZE, ITEMS_COUNT, ITEMS_PATH_TO_TEXTURES);
  /*Vertices and indices defines*/
  struct vertex vertices[VERTICES_COUNT];
  unsigned int indices[INDICES_COUNT] = {0, 1, 2, 2, 3, 0};
  /*Setup vertices for this part of interface*/
  /*Bottom-left*/
  vertices[0].pos[0] = relx;
  vertices[0].pos[1] = rely;
  vertices[0].pos[2] = INVENTORY_LAYER;
  vertices[0].tex[0] = INVENTORY_TEXTURE_START_COORD;
  vertices[0].tex[1] = INVENTORY_TEXTURE_START_COORD;
  vertices[0].texlayer = INVENTORY_TEX_LAYER;
  /*Bottom-right*/
  vertices[1].pos[0] = relx + width;
  vertices[1].pos[1] = rely;
  vertices[1].pos[2] = INVENTORY_LAYER;
  vertices[1].tex[0] = INVENTORY_TEXTURE_START_COORD + TEX_SHIFT;
  vertices[1].tex[1] = INVENTORY_TEXTURE_START_COORD;
  vertices[1].texlayer = INVENTORY_TEX_LAYER;
  /*Top-right*/
  vertices[2].pos[0] = relx + width;
  vertices[2].pos[1] = rely + height;
  vertices[2].pos[2] = INVENTORY_LAYER;
  vertices[2].tex[0] = INVENTORY_TEXTURE_START_COORD + TEX_SHIFT;
  vertices[2].tex[1] = INVENTORY_TEXTURE_START_COORD + TEX_SHIFT;
  vertices[2].texlayer = INVENTORY_TEX_LAYER;
  /*Top-left*/
  vertices[3].pos[0] = relx;
  vertices[3].pos[1] = rely + height;
  vertices[3].pos[2] = INVENTORY_LAYER;
  vertices[3].tex[0] = INVENTORY_TEXTURE_START_COORD;
  vertices[3].tex[1] = INVENTORY_TEXTURE_START_COORD + TEX_SHIFT;
  vertices[3].texlayer = INVENTORY_TEX_LAYER;
  inv.elem = createelement(vertices, VERTICES_COUNT, indices, INDICES_COUNT, true, GL_STATIC_DRAW);
  return inv;
}

void destroyinventory(struct inventory* inv)
{
  if(inv->elem.vao != 0){
    destroyelement(&inv->elem);
  }
  for(int i = 0; i < MAX_ITEM_COUNT; ++i){
    if(inv->it[i].elem.vao != 0){
      destroyelement(&inv->it[i].elem);
    }
  }
}

void updateinventory(struct inventory* inv)
{
  
}

void renderinventory(struct inventory* inv)
{
  displayelement(inv->elem);
  renderitems(inv);
}

struct element spawnitem(float x, float y, float genoffset, short* id, float ITEMS_LAYER)
{
  /*Set item id*/
  *id = ((int)(x + y + genoffset))%ITEMS_COUNT + ID_000014_GOLEM + 1;
  /*Vertices and indices defines*/
  struct vertex vertices[VERTICES_COUNT];
  unsigned int indices[INDICES_COUNT] = {0, 1, 2, 2, 3, 0};
  /*Setup vertices for this part of interface*/
  /*Bottom-left*/
  float xpos = x * TILE_SIZE;
  float ypos = y * TILE_SIZE;
  vertices[0].pos[0] = xpos;
  vertices[0].pos[1] = ypos;
  vertices[0].pos[2] = ITEMS_LAYER;
  vertices[0].tex[0] = ITEMS_TEXTURE_START_COORD;
  vertices[0].tex[1] = ITEMS_TEXTURE_START_COORD;
  vertices[0].texlayer = *id;
  /*Bottom-right*/
  vertices[1].pos[0] = xpos + TILE_SIZE;
  vertices[1].pos[1] = ypos;
  vertices[1].pos[2] = ITEMS_LAYER;
  vertices[1].tex[0] = ITEMS_TEXTURE_START_COORD + TEX_SHIFT;
  vertices[1].tex[1] = ITEMS_TEXTURE_START_COORD;
  vertices[1].texlayer = *id;
  /*Top-right*/
  vertices[2].pos[0] = xpos + TILE_SIZE;
  vertices[2].pos[1] = ypos + TILE_SIZE;
  vertices[2].pos[2] = ITEMS_LAYER;
  vertices[2].tex[0] = ITEMS_TEXTURE_START_COORD + TEX_SHIFT;
  vertices[2].tex[1] = ITEMS_TEXTURE_START_COORD + TEX_SHIFT;
  vertices[2].texlayer = *id;
  /*Top-left*/
  vertices[3].pos[0] = xpos;
  vertices[3].pos[1] = ypos + TILE_SIZE;
  vertices[3].pos[2] = ITEMS_LAYER;
  vertices[3].tex[0] = ITEMS_TEXTURE_START_COORD;
  vertices[3].tex[1] = ITEMS_TEXTURE_START_COORD + TEX_SHIFT;
  vertices[3].texlayer = *id;

  return createelement(vertices, VERTICES_COUNT, indices, INDICES_COUNT, true, GL_DYNAMIC_DRAW);
}

void renderitems(struct inventory* inv)
{
  for(int i = 0; i < MAX_ITEM_COUNT; ++i){
    if(inv->it[i].elem.vao != 0){
      displayelement(inv->it[i].elem);
    }
  }
}
