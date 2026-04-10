#ifndef ENGINECORE_INVENTORY_H
#define ENGINECORE_INVENTORY_H

#include "EngineCore/display_manager.h"
#include "EngineCore/texture_manager.h"

#define MAX_ITEM_COUNT 21

struct item{
  float xcoord, ycoord;
  struct element elem;
  short id;
};

struct inventory{
  struct texturearray texarray;
  struct texturearray items_tex_array;
  float height, width;
  float xcoord, ycoord;
  short itcount;
  struct element elem;
  struct item it[MAX_ITEM_COUNT];
};

struct inventory createinventory(float relx, float rely, float width, float height);

void destroyinventory(struct inventory* inv);

void updateinventory(struct inventory* inv);

void renderinventory(struct inventory* inv);

struct element spawnitem(float x, float y, float genoffset, short* id, float ITEMS_LAYER);

void renderitems(struct inventory* inv);

#endif/*ENGINECORE_INVENTORY_H*/
