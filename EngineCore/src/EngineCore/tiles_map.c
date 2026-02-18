#include "EngineCore/tiles_map.h"

#include <stdlib.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "EngineCore/debug_log.h"
#include "EngineCore/map_generator.h"

#include "Mathematics/ortho_projection.h"

#define MAP_WIDTH_ASPECT 0.5f
#define MAP_HEIGHT_ASPECT 1
#define VERTICES_COUNT 4
#define INDICES_COUNT 6
#define TEX_SHIFT 1.0f
#define GROUND_LAYER 0.0f
#define MOBS_LAYER 0.5f
#define EFFECT_LAYER 1.0f

#define PATH_TO_TEXTURES "res/textures"

#define MAP_START 0
#define PLAYER_START_COORDX 6
#define PLAYER_START_COORDY 2

#define SEED 1234567

void createtilemap(struct tilemap* map, struct GLFWwindow* win)
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
  #ifndef NDEBUG
  if(TILES_TYPE_NUMBER == get_textures_number(PATH_TO_TEXTURES)){
    load_texture_array(&map->texarr, TILE_SIZE, TILES_TYPE_NUMBER, PATH_TO_TEXTURES);
    if(!map->texarr.id){
      LOG_CRITICAL("Failed to load texture array\n");
    }
  }else{
    LOG_CRITICAL("TILES_TYPE_NUMBER != get_textures_number()\n");
  }
  #else
  load_texture_array(&map->texarr, TILE_SIZE, TILES_TYPE_NUMBER, PATH_TO_TEXTURES);
  #endif
  /*Create vertices and indices for all tiles*/
  struct vertex vertices[VERTICES_COUNT];
  unsigned int indices[INDICES_COUNT] = {0, 1, 2, 2, 3, 0};
  /*Map pregenerate*/
  int tiletype[MAP_HEIGHT][MAP_WIDTH];
  mapgen(MAP_HEIGHT, MAP_WIDTH, tiletype, SEED, MAP_START, MAP_START);
  /*Initialize all tiles with their elements*/
  for (int i = 0, y = 0; y < MAP_HEIGHT; y++) {
    for (int x = 0; x < MAP_WIDTH; x++) {
      /*Calculate tile coordinates in pixels*/
      const float xpos = x * TILE_SIZE;
      const float ypos = y * TILE_SIZE;
      /*Get tile type*/
      map->gmap.groundmap[y][x].id = tiletype[y][x];
      map->gmap.groundmap[y][x].xcoord = xpos;
      map->gmap.groundmap[y][x].ycoord = ypos;
      /*Set texture coordinates*/
      const float xtex = 0.0f;
      const float ytex = 0.0f;
      const int texlayer = tiletype[y][x];
      /*Setup vertices for this tile*/
      /*Bottom-left*/
      vertices[0].pos[0] = xpos;
      vertices[0].pos[1] = ypos;
      vertices[0].pos[2] = GROUND_LAYER;
      vertices[0].tex[0] = xtex;
      vertices[0].tex[1] = ytex;
      vertices[0].texlayer = texlayer;
      /*Bottom-right*/
      vertices[1].pos[0] = xpos + TILE_SIZE;
      vertices[1].pos[1] = ypos;
      vertices[1].pos[2] = GROUND_LAYER;
      vertices[1].tex[0] = xtex + TEX_SHIFT;
      vertices[1].tex[1] = ytex;
      vertices[1].texlayer = texlayer;
      /*Top-right*/
      vertices[2].pos[0] = xpos + TILE_SIZE;
      vertices[2].pos[1] = ypos + TILE_SIZE;
      vertices[2].pos[2] = GROUND_LAYER;
      vertices[2].tex[0] = xtex + TEX_SHIFT;
      vertices[2].tex[1] = ytex + TEX_SHIFT;
      vertices[2].texlayer = texlayer;
      /*Top-left*/
      vertices[3].pos[0] = xpos;
      vertices[3].pos[1] = ypos + TILE_SIZE;
      vertices[3].pos[2] = GROUND_LAYER;
      vertices[3].tex[0] = xtex;
      vertices[3].tex[1] = ytex + TEX_SHIFT;
      vertices[3].texlayer = texlayer;
      /*Create OpenGL element for this tile*/
      map->gmap.groundmap[y][x].isobstacle = false;
      map->gmap.groundmap[y][x].tile = createelement(vertices, VERTICES_COUNT, 
                                               indices, INDICES_COUNT, 
                                               true, GL_STATIC_DRAW);
    }
  }
  /*Setup vertices for player tile*/
  const float xpos = PLAYER_START_COORDX * TILE_SIZE;
  const float ypos = PLAYER_START_COORDY * TILE_SIZE;
  const float xtex = 0.0f;
  const float ytex = 0.0f;
  /*Bottom-left*/
  vertices[0].pos[0] = xpos;
  vertices[0].pos[1] = ypos;
  vertices[0].pos[2] = MOBS_LAYER;
  vertices[0].tex[0] = xtex;
  vertices[0].tex[1] = ytex;
  vertices[0].texlayer = ID_000004_PLAYER;
  /*Bottom-right*/
  vertices[1].pos[0] = xpos + TILE_SIZE;
  vertices[1].pos[1] = ypos;
  vertices[1].pos[2] = MOBS_LAYER;
  vertices[1].tex[0] = xtex + TEX_SHIFT;
  vertices[1].tex[1] = ytex;
  vertices[1].texlayer = ID_000004_PLAYER;
  /*Top-right*/
  vertices[2].pos[0] = xpos + TILE_SIZE;
  vertices[2].pos[1] = ypos + TILE_SIZE;
  vertices[2].pos[2] = MOBS_LAYER;
  vertices[2].tex[0] = xtex + TEX_SHIFT;
  vertices[2].tex[1] = ytex + TEX_SHIFT;
  vertices[2].texlayer = ID_000004_PLAYER;
  /*Top-left*/
  vertices[3].pos[0] = xpos;
  vertices[3].pos[1] = ypos + TILE_SIZE;
  vertices[3].pos[2] = MOBS_LAYER;
  vertices[3].tex[0] = xtex;
  vertices[3].tex[1] = ytex + TEX_SHIFT;
  vertices[3].texlayer = ID_000004_PLAYER;
  /*Set player on place*/
  map->gmap.pcx = PLAYER_START_COORDX;
  map->gmap.pcy = PLAYER_START_COORDY;
  map->gmap.mobs[PLAYER_START_COORDY][PLAYER_START_COORDX].isobstacle = true;
  map->gmap.mobs[PLAYER_START_COORDY][PLAYER_START_COORDX].xcoord = xpos;
  map->gmap.mobs[PLAYER_START_COORDY][PLAYER_START_COORDX].ycoord = ypos;
  map->gmap.mobs[PLAYER_START_COORDY][PLAYER_START_COORDX].tile = createelement(vertices, VERTICES_COUNT, 
                                                                          indices, INDICES_COUNT, 
                                                                          true, GL_STATIC_DRAW);
}

void destroytilemap(struct tilemap* map)
{
  /*Destroy all tile elements*/
  for(int y = 0; y < MAP_HEIGHT; ++y){
    for(int x = 0; x < MAP_WIDTH; ++x){
      destroyelement(&map->gmap.groundmap[y][x].tile);
      if(map->gmap.mobs[y][x].tile.vao != 0){
        destroyelement(&map->gmap.mobs[y][x].tile);
      }
      if(map->gmap.effect[y][x].tile.vao != 0){
        destroyelement(&map->gmap.effect[y][x].tile);
      }
    }
  }
  /*Destroy texture array*/
  delete_texture_array(&map->texarr);
}

void updatetilemap(struct tilemap* map)
{

}

void rendertilemap(struct tilemap* map, GLuint shaderprogram, float screenaspect)
{
  /*Calculate orthographic projection matrix*/
  float projection[16];
  float mapwidth = MAP_WIDTH * TILE_SIZE;
  float mapheight = MAP_HEIGHT * TILE_SIZE;
  float mapaspect = mapwidth / mapheight;
  float left = 0, right = 0, bottom = 0, top = 0;
  float visiblewidth = 0, visibleheight = 0;
  if(screenaspect > mapaspect){
    visibleheight = mapheight;
    visiblewidth = mapheight * screenaspect;
  }else{
    visiblewidth = mapwidth;
    visibleheight = mapwidth / screenaspect;
  }

  left = (mapwidth - visiblewidth) / 2.0f;
  right = mapwidth - left;
  bottom = (mapheight - visibleheight) / 2.0f;
  top = mapheight - bottom;
  create_ortho_projection(projection, left, right, bottom, top);
  /*Send orthographic projection matrix to shader*/
  GLuint projectionloc = glGetUniformLocation(shaderprogram, "projview");
  glUniformMatrix4fv(projectionloc, 1, GL_FALSE, projection); 
  /*Render all tiles*/
  for(int y = 0; y < MAP_HEIGHT; ++y){
    for(int x = 0; x < MAP_WIDTH; ++x){
      displayelement(map->gmap.groundmap[y][x].tile);
      if(map->gmap.mobs[y][x].tile.vao != 0){
        displayelement(map->gmap.mobs[y][x].tile);
      }
      if(map->gmap.effect[y][x].tile.vao != 0){
        displayelement(map->gmap.effect[y][x].tile);
      }
    }
  }
}

static void update_tile_position(struct tile* t, float x, float y) {
  if (t->tile.vao == 0 || t->tile.vbo == 0){
    return;
  }
  /*Z-coord defines*/
  float layer = (t->id == ID_000004_PLAYER) ? MOBS_LAYER : GROUND_LAYER;
  /*Texture params*/
  const float xtex = 0.0f, ytex = 0.0f;
  struct vertex verts[VERTICES_COUNT];
  /*Bottom-left*/
  verts[0].pos[0] = x;
  verts[0].pos[1] = y;
  verts[0].pos[2] = layer;
  verts[0].tex[0] = xtex;
  verts[0].tex[1] = ytex;
  verts[0].texlayer = t->id;
  /*Bottom-right*/
  verts[1].pos[0] = x + TILE_SIZE;
  verts[1].pos[1] = y;
  verts[1].pos[2] = layer;
  verts[1].tex[0] = xtex + TEX_SHIFT;
  verts[1].tex[1] = ytex;
  verts[1].texlayer = t->id;
  /*Top-right*/
  verts[2].pos[0] = x + TILE_SIZE;
  verts[2].pos[1] = y + TILE_SIZE;
  verts[2].pos[2] = layer;
  verts[2].tex[0] = xtex + TEX_SHIFT;
  verts[2].tex[1] = ytex + TEX_SHIFT;
  verts[2].texlayer = t->id;
  /*Top-left*/
  verts[3].pos[0] = x;
  verts[3].pos[1] = y + TILE_SIZE;
  verts[3].pos[2] = layer;
  verts[3].tex[0] = xtex;
  verts[3].tex[1] = ytex + TEX_SHIFT;
  verts[3].texlayer = t->id;
  /*VBO update*/
  glBindBuffer(GL_ARRAY_BUFFER, t->tile.vbo);
  glBufferSubData(GL_ARRAY_BUFFER, 0, VERTICES_COUNT * sizeof(struct vertex), verts);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  t->xcoord = x;
  t->ycoord = y;
}

static void nearest_swap_tile(struct tile (*map)[MAP_WIDTH], int h1, int w1, int h2, int w2)
{
  if(h1 < 0 || h1 >= MAP_HEIGHT || w1 < 0 || w1 >= MAP_WIDTH ||
     h2 < 0 || h2 >= MAP_HEIGHT || w2 < 0 || w2 >= MAP_WIDTH){
    return;
  }
  struct tile temp = map[h1][w1];
  map[h1][w1] = map[h2][w2];
  map[h2][w2] = temp;
  /*Update tile position*/
  update_tile_position(&map[h1][w1], w1 * TILE_SIZE, h1 * TILE_SIZE);
  update_tile_position(&map[h2][w2], w2 * TILE_SIZE, h2 * TILE_SIZE);
}

void move_mob_on_place(struct gamemap* gmap, enum MOVE_TO_TILE MT_T)
{
  #define MOVE 1
  if(MT_T == MT_FT){
    if(!gmap->groundmap[gmap->pcy + MOVE][gmap->pcx].isobstacle && !gmap->mobs[gmap->pcy + MOVE][gmap->pcx].isobstacle){
      nearest_swap_tile(gmap->mobs, gmap->pcy, gmap->pcx, gmap->pcy + MOVE, gmap->pcx);
      gmap->pcy += MOVE;
    }
  }
  if(MT_T == MT_FRT){
    if(!gmap->groundmap[gmap->pcy + MOVE][gmap->pcx + MOVE].isobstacle && !gmap->mobs[gmap->pcy + MOVE][gmap->pcx + MOVE].isobstacle){
      nearest_swap_tile(gmap->mobs, gmap->pcy, gmap->pcx, gmap->pcy + MOVE, gmap->pcx);
      gmap->pcy += MOVE;
      gmap->pcx += MOVE;
    }
  }
  if(MT_T == MT_RT){
    if(!gmap->groundmap[gmap->pcy][gmap->pcx + MOVE].isobstacle && !gmap->mobs[gmap->pcy][gmap->pcx + MOVE].isobstacle){
      nearest_swap_tile(gmap->mobs, gmap->pcy, gmap->pcx, gmap->pcy + MOVE, gmap->pcx);
      gmap->pcx += MOVE;
    }
  }
  if(MT_T == MT_DRT){
    if(!gmap->groundmap[gmap->pcy - MOVE][gmap->pcx + MOVE].isobstacle && !gmap->mobs[gmap->pcy - MOVE][gmap->pcx + MOVE].isobstacle){
      nearest_swap_tile(gmap->mobs, gmap->pcy, gmap->pcx, gmap->pcy + MOVE, gmap->pcx);
      gmap->pcy -= MOVE;
      gmap->pcx += MOVE;
    }
  }
  if(MT_T == MT_DT){
    if(!gmap->groundmap[gmap->pcy - MOVE][gmap->pcx].isobstacle && !gmap->mobs[gmap->pcy - MOVE][gmap->pcx].isobstacle){
      nearest_swap_tile(gmap->mobs, gmap->pcy, gmap->pcx, gmap->pcy + MOVE, gmap->pcx);
      gmap->pcy -= MOVE;
    }
  }
  if(MT_T == MT_DLT){
    if(!gmap->groundmap[gmap->pcy - MOVE][gmap->pcx - MOVE].isobstacle && !gmap->mobs[gmap->pcy - MOVE][gmap->pcx - MOVE].isobstacle){
      nearest_swap_tile(gmap->mobs, gmap->pcy, gmap->pcx, gmap->pcy + MOVE, gmap->pcx);
      gmap->pcy -= MOVE;
      gmap->pcx -= MOVE;
    }
  }
  if(MT_T == MT_LT){
    if(!gmap->groundmap[gmap->pcy][gmap->pcx - MOVE].isobstacle && !gmap->mobs[gmap->pcy][gmap->pcx - MOVE].isobstacle){
      nearest_swap_tile(gmap->mobs, gmap->pcy, gmap->pcx, gmap->pcy + MOVE, gmap->pcx);
      gmap->pcx -= MOVE;
    }
  }
  if(MT_T == MT_FLT){
    if(!gmap->groundmap[gmap->pcy + MOVE][gmap->pcx - MOVE].isobstacle && !gmap->mobs[gmap->pcy + MOVE][gmap->pcx - MOVE].isobstacle){
      nearest_swap_tile(gmap->mobs, gmap->pcy, gmap->pcx, gmap->pcy + MOVE, gmap->pcx);
      gmap->pcy += MOVE;
      gmap->pcx -= MOVE;
    }
  }
}
