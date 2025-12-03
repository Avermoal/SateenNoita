#include "EngineCore/tiles_map.h"

#include "Mathemathics/ortho_projection.h"

#define MAP_WIDTH_ASPECT 0.5f
#define MAP_HEIGHT_ASPECT 1
#define VERTICES_COUNT 4
#define INDICES_COUNT 6
#define TEX_SHIFT 0.5f

void createtilemap(struct tilemap *map, GLFWwindow *win)
{
  /*Set map coords*/
  int windowwidth = 0, windowheight = 0;
  glfwGetFrameBufferSize(win, &windowwidth, &windowheight);
  map->xcoord = (int)(MAP_WIDTH_ASPECT * MAP_WIDTH_ASPECT * windowwidth);
  map->ycoord = 0;
  /*Set map width and height*/
  map->width = (int)(MAP_WIDTH_ASPECT * windowwidth);
  map->height = MAP_HEIGHT_ASPECT * windowheight;
  /*EXPERIMENTS!*/
  for(int i = 0; i < CELLS_NUMBER; ++i){
    map->gamemap[i] = i % 3;
  }
}

void destroytilemap(struct tilemap *map)
{
  for(int i = 0; i < CELLS_NUMBER; ++i){
    destroyelement(&map->gamemap[i].tile);
  }
}

void updatetilemap(struct tilemap *map)
{

}

void rendertilemap(struct tilemap *map, GLuint shaderprogram, float screenaspect)
{
  /*Create ortho. projection matrix*/
  float projection[16];
  float mapwidth = MAP_WIDTH * TILE_SIZE;
  float mapheight = MAP_HEIGHT * TILE_SIZE;
  float mapaspect = mapwidth / mapheight;
  float left = 0, right = 0, bottom = 0, top = 0;
  if(screenaspect > mapaspect){
    float visible_map_width = mapheight * screenaspect;
    left = -(visible_map_width - mapwidth)/2.0f;
    right = mapwidth - left;
    bottom = 0.0f;
    top = mapheight;
  }else{
    float visible_map_height = mapwidth * screenaspect;
    left = 0.0f;
    right = mapwidth;
    bottom = -(visible_map_height - mapheight)/2.0f;
    top = mapwidth - bottom;
  }
  create_ortho_projection(projection, left, right, bottom, top);
  /*Share ortho. projection matrix into shader program*/
  GLuint = projectionloc = glGetUniformLocation(shaderprogram, "projection");
  glUniformMatrix4fv(projectionloc, 1, GL_FALSE, projection);
  /*Game map create render element*/
  struct vertex vertices[VERTICES_COUNT];
  unsigned int indices[INDICES_COUNT];
  for(int i = 0, j = 0; i < CELLS_NUMBER; ++i){
    short tiletype = map->gamemap[i].id;
    if(tiletype == 0){
      continue;
    }
    /*Get tile coordinates in pixels*/
    float xpos = (i % MAP_WIDTH) * TILE_SIZE;
    float ypos = (j % MAP_HEIGHT) * TILE_SIZE;
    if(i != 0 && i % MAP_WIDTH == 0){
      ++j;
    }
    map->gamemap[i].xcoord = xpos;
    map->gamemap[i].ycoord = ypos;
    /*Set texture coords*/
    /*EXPERIMENTS!*/
    float xtex = 0.0f, ytex = 0.0f;
    switch(tiletype){
      case GRASS: xtex = 0.0f; ytex = 0.0f; break;/*Left top corner*/
      case WALL: xtex = 0.5f; ytex = 0.0f; break;/*Right top corner*/
    }
    /*Set tile vertices and indices*/
    vertices[0].pos[0] = xpos;  vertices[1].pos[0] = xpos + TILE_SIZE;  vertices[2].pos[0] = xpos + TILE_SIZE;  vertices[3].pos[0] = xpos;
    vertices[0].pos[1] = ypos;  vertices[1].pos[1] = ypos;              vertices[2].pos[1] = ypos + TILE_SIZE;  vertices[3].pos[1] = ypos + TILE_SIZE;
    vertices[0].pos[2] = 0.0f;  vertices[1].pos[2] = 0.0f;              vertices[2].pos[2] = 0.0f;              vertices[3].pos[2] = 0.0f;
    vertices[0].tex[0] = xtex;  vertices[1].tex[0] = xtex + TEX_SHIFT;  vertices[2].tex[0] = xtex + TEX_SHIFT;  vertices[3].tex[0] = xtex;
    vertices[0].tex[1] = ytex;  vertices[1].tex[1] = ytex;              vertices[2].tex[1] = ytex + TEX_SHIFT;  vertices[3].tex[1] = ytex + TEX_SHIFT;
    indices[0] = 0; indices[1] = 1; indices[2] = 2;
    indices[3] = 2; indices[4] = 3; indices[5] = 0;
    /*Create map element*/
    map->gamemap[i].tile = createelement(vertices, VERTICES_COUNT, indices, INDICES_COUNT, GL_STATIC_DRAW);
  }
}
