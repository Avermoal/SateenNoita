#include "EngineCore/tile_renderer.h"

#include <glad/glad.h>

#include "EngineCore/display_manager.h"
#include "EngineCore/map_data.h"

static void fillvert(struct tile* t, struct vertex vertices[VERTICES_COUNT]);

void create_tile_element(struct tile* t)
{
  /*Defines vertices and indices*/
  struct vertex vertices[VERTICES_COUNT];
  unsigned int indices[INDICES_COUNT] = {0, 1, 2, 2, 3, 0};
  /*Fill vertices array*/
  fillvert(t, texlayer, vertices);
  t->elem = createelement(vertices, VERTICES_COUNT, indices, INDICES_COUNT, true, GL_DYNAMIC_DRAW);
}

void rendertile(const tile* t)
{
  if(t || t->elem.vao != 0){
    displayelement(t->elem);
  }
}

void updatetile(const tile* t, float new_x_coord, float new_y_coord)
{
  t->xcoord = new_x_coord;
  t->ycoord = new_y_coord;
  /*Refill vertices array*/
  struct vertex vertices[VERTICES_COUNT];
  fillvert(t, texlayer, vertices);
  /*Update OpenGL buffer*/
  glBindBuffer(GL_ARRAY_BUFFER, t->elem.vbo);
  glBufferSubData(GL_ARRAY_BUFFER, 0, VERTICES_COUNT * sizeof(struct vertex), vertices);
}

static void fillvert(struct tile* t, struct vertex vertices[VERTICES_COUNT])
{
  const float xpos = t->xcoord;
  const float ypos = t->ycoord;
  const float xtex = 0.0f;
  const float ytex = 0.0f;
  const float layer = t->layer;
  const float texlayer = y->id;
  /*SETUP VERTEX FOR THIS TILE*/
  /*Bottom-left*/
  vertices[0].pos[0] = xpos;
  vertices[0].pos[1] = ypos;
  vertices[0].pos[2] = layer;
  vertices[0].tex[0] = xtex;
  vertices[0].tex[1] = ytex;
  vertices[0].texlayer = texlayer;
  /*Bottom-right*/
  vertices[1].pos[0] = xpos + TILE_SIZE;
  vertices[1].pos[1] = ypos;
  vertices[1].pos[2] = layer;
  vertices[1].tex[0] = xtex + TEX_SHIFT;
  vertices[1].tex[1] = ytex;
  vertices[1].texlayer = texlayer;
  /*Top-right*/
  vertices[2].pos[0] = xpos + TILE_SIZE;
  vertices[2].pos[1] = ypos + TILE_SIZE;
  vertices[2].pos[2] = layer;
  vertices[2].tex[0] = xtex + TEX_SHIFT;
  vertices[2].tex[1] = ytex + TEX_SHIFT;
  vertices[2].texlayer = texlayer;
  /*Top-left*/
  vertices[3].pos[0] = xpos;
  vertices[3].pos[1] = ypos + TILE_SIZE;
  vertices[3].pos[2] = layer;
  vertices[3].tex[0] = xtex;
  vertices[3].tex[1] = ytex + TEX_SHIFT;
  vertices[3].texlayer = texlayer;
}
