#include "EngineCore/tiles_map.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>

#include "Mathematics/ortho_projection.h"

#define MAP_WIDTH_ASPECT 0.5f
#define MAP_HEIGHT_ASPECT 1
#define VERTICES_COUNT 4
#define INDICES_COUNT 6
#define TEX_SHIFT 1.0f

#define PATH_TO_TEXTURES "res/textures"

void createtilemap(struct tilemap *map, struct GLFWwindow *win)
{
  /*Set map coords*/
  int windowwidth = 0, windowheight = 0;
  glfwGetFramebufferSize(win, &windowwidth, &windowheight);
  map->xcoord = (int)(MAP_WIDTH_ASPECT * MAP_WIDTH_ASPECT * windowwidth);
  map->ycoord = 0;
  /*Set map width and height*/
  map->width = (int)(MAP_WIDTH_ASPECT * windowwidth);
  map->height = MAP_HEIGHT_ASPECT * windowheight;
  /*Load texture array*/
  load_texture_array(&map->texarr, TILE_SIZE, TILES_TYPE_NUMBER, PATH_TO_TEXTURES);
  /*Create vertices and indices for all tiles*/
  struct vertex vertices[VERTICES_COUNT];
  unsigned int indices[INDICES_COUNT] = {0, 1, 2, 2, 3, 0};
  /*Initialize all tiles with their elements*/
  for (int i = 0, y = 0; y < MAP_HEIGHT; y++) {
    for (int x = 0; x < MAP_WIDTH; x++) {
      int index = y * MAP_WIDTH + x;
      /*Calculate tile coordinates in pixels*/
      float xpos = x * TILE_SIZE;
      float ypos = y * TILE_SIZE;
      /*Get tile type*/
      short tiletype = index % TILES_TYPE_NUMBER;
      map->gamemap[index].id = tiletype;
      map->gamemap[index].xcoord = xpos;
      map->gamemap[index].ycoord = ypos;
      /*Set texture coordinates*/
      float xtex = 0.0f;
      float ytex = 0.0f;
      float texlayer = (float)tiletype;
      /*Set up vertices for this tile*/
      /*Bottom-left*/
      vertices[0].pos[0] = xpos;
      vertices[0].pos[1] = ypos;
      vertices[0].pos[2] = 0.0f;
      vertices[0].tex[0] = xtex;
      vertices[0].tex[1] = ytex;
      vertices[0].texlayer = texlayer;
      /*Bottom-right*/
      vertices[1].pos[0] = xpos + TILE_SIZE;
      vertices[1].pos[1] = ypos;
      vertices[1].pos[2] = 0.0f;
      vertices[1].tex[0] = xtex + TEX_SHIFT;
      vertices[1].tex[1] = ytex;
      vertices[1].texlayer = texlayer;
      /*Top-right*/
      vertices[2].pos[0] = xpos + TILE_SIZE;
      vertices[2].pos[1] = ypos + TILE_SIZE;
      vertices[2].pos[2] = 0.0f;
      vertices[2].tex[0] = xtex + TEX_SHIFT;
      vertices[2].tex[1] = ytex + TEX_SHIFT;
      vertices[2].texlayer = texlayer;
      /*Top-left*/
      vertices[3].pos[0] = xpos;
      vertices[3].pos[1] = ypos + TILE_SIZE;
      vertices[3].pos[2] = 0.0f;
      vertices[3].tex[0] = xtex;
      vertices[3].tex[1] = ytex + TEX_SHIFT;
      vertices[3].texlayer = texlayer;
      /*Create OpenGL element for this tile*/
      map->gamemap[index].tile = createelement(vertices, VERTICES_COUNT, 
                                               indices, INDICES_COUNT, 
                                               true, GL_STATIC_DRAW);
    }
  }
}

void destroytilemap(struct tilemap *map)
{
  /*Destroy all tile elements*/
  for(int i = 0; i < CELLS_NUMBER; ++i){
    destroyelement(&map->gamemap[i].tile);
  }
  /*Destroy texture array*/
  delete_texture_array(&map->texarr);
}

void updatetilemap(struct tilemap *map)
{
  /*Update tilemap logic here if needed*/
}

void rendertilemap(struct tilemap *map, GLuint shaderprogram, float screenaspect)
{
  /*Calculate orthographic projection matrix*/
  float projection[16];
  float mapwidth = MAP_WIDTH * TILE_SIZE;
  float mapheight = MAP_HEIGHT * TILE_SIZE;
  float mapaspect = mapwidth / mapheight;
  float left = 0, right = 0, bottom = 0, top = 0;
  if(screenaspect > mapaspect){
    float visible_map_width = mapheight * screenaspect;
    left = -(visible_map_width - mapwidth) / 2.0f;
    right = mapwidth - left;
    bottom = 0.0f;
    top = mapheight;
  } else {
    float visible_map_height = mapwidth / screenaspect;
    left = 0.0f;
    right = mapwidth;
    bottom = -(visible_map_height - mapheight) / 2.0f;
    top = mapheight - bottom;
  }
  create_ortho_projection(projection, left, right, bottom, top);
  /*Send orthographic projection matrix to shader*/
  GLuint projectionloc = glGetUniformLocation(shaderprogram, "projview");
  glUniformMatrix4fv(projectionloc, 1, GL_FALSE, projection); 
  /*Render all tiles*/
  for(int i = 0; i < CELLS_NUMBER; ++i){
    displayelement(map->gamemap[i].tile);
  }
}
