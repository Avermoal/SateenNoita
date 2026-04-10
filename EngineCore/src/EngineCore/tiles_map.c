#include "EngineCore/tiles_map.h"

#include <stdlib.h>
#include <string.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "EngineCore/debug_log.h"
#include "EngineCore/game_window_info.h"
#include "EngineCore/map_generator.h"
#include "EngineCore/stats_system.h"
#include "EngineCore/inventory.h"

#include "Mathematics/ortho_projection.h"

#define GROUND_LAYER 0.0f
#define MOBS_LAYER 0.5f
#define ITEMS_LAYER 1.0f

#define PATH_TO_TEXTURES "res/textures/game_textures"

#define MAP_START 0
#define PLAYER_START_COORDX 9
#define PLAYER_START_COORDY 3
#define PLAYER_BORDER_COORD (int)(MAP_HEIGHT / 2)

#define ANIMATION_AMPLITUDE 0.005f
#define ANIMATION_SPEED 3.0f

#define MOBS_START_HP_MP 10
#define PLAYER_START_HP_MP 100

#define SEED 97152890

static void set_mobs(struct gamemap* gmap, int lbc, bool is_new_line)
{
  /*Create vertices and indices for all tiles*/
  struct vertex vertices[VERTICES_COUNT];
  unsigned int indices[INDICES_COUNT] = {0, 1, 2, 2, 3, 0};
  /*Place mobs on map*/
  if(is_new_line){
    struct tile tiletype[1][MAP_WIDTH];
    mapgen(1, MAP_WIDTH, tiletype, lbc, MOBS_TILE);
    for (int x = 0; x < MAP_WIDTH; ++x){
      /*VOID CHECK*/
      if(tiletype[0][x].id == ID_000000_VOID){
        gmap->mobs[MAP_HEIGHT - 1][x].tile.vao = 0;
        gmap->mobs[MAP_HEIGHT - 1][x].id = ID_000000_VOID;
        gmap->mobs[MAP_HEIGHT - 1][x].isobstacle = false;
        gmap->mobs[MAP_HEIGHT - 1][x].stts = NULL;
        continue;
      }
      /*Calculate tile coordinates in pixels*/
      const float xpos = x * TILE_SIZE;
      const float ypos = (MAP_HEIGHT - 1) * TILE_SIZE;
      /*Get tile type*/
      gmap->mobs[MAP_HEIGHT - 1][x].id = tiletype[0][x].id;
      gmap->mobs[MAP_HEIGHT - 1][x].xcoord = xpos;
      gmap->mobs[MAP_HEIGHT - 1][x].ycoord = ypos;
      gmap->mobs[MAP_HEIGHT - 1][x].isobstacle = tiletype[0][x].isobstacle;
      gmap->mobs[MAP_HEIGHT - 1][x].layer = MOBS_LAYER;
      gmap->mobs[MAP_HEIGHT - 1][x].stts = malloc(sizeof(struct stats));
      gmap->mobs[MAP_HEIGHT - 1][x].stts->HP = MOBS_START_HP_MP;
      gmap->mobs[MAP_HEIGHT - 1][x].stts->MP = MOBS_START_HP_MP;
      gmap->mobs[MAP_HEIGHT - 1][x].stts->isalive = true;

      /*Set texture coordinates*/
      const float xtex = 0.0f;
      const float ytex = 0.0f;
      const int texlayer = tiletype[0][x].id;
      /*Setup vertices for this tile*/
      /*Bottom-left*/
      vertices[0].pos[0] = xpos;
      vertices[0].pos[1] = ypos;
      vertices[0].pos[2] = MOBS_LAYER;
      vertices[0].tex[0] = xtex;
      vertices[0].tex[1] = ytex;
      vertices[0].texlayer = texlayer;
      /*Bottom-right*/
      vertices[1].pos[0] = xpos + TILE_SIZE;
      vertices[1].pos[1] = ypos;
      vertices[1].pos[2] = MOBS_LAYER;
      vertices[1].tex[0] = xtex + TEX_SHIFT;
      vertices[1].tex[1] = ytex;
      vertices[1].texlayer = texlayer;
      /*Top-right*/
      vertices[2].pos[0] = xpos + TILE_SIZE;
      vertices[2].pos[1] = ypos + TILE_SIZE;
      vertices[2].pos[2] = MOBS_LAYER;
      vertices[2].tex[0] = xtex + TEX_SHIFT;
      vertices[2].tex[1] = ytex + TEX_SHIFT;
      vertices[2].texlayer = texlayer;
      /*Top-left*/
      vertices[3].pos[0] = xpos;
      vertices[3].pos[1] = ypos + TILE_SIZE;
      vertices[3].pos[2] = MOBS_LAYER;
      vertices[3].tex[0] = xtex;
      vertices[3].tex[1] = ytex + TEX_SHIFT;
      vertices[3].texlayer = texlayer;
      /*Create OpenGL element for this tile*/
      gmap->mobs[MAP_HEIGHT - 1][x].tile = createelement(vertices, VERTICES_COUNT, 
                                             indices, INDICES_COUNT, 
                                             true, GL_STATIC_DRAW);
    }
  }else{
    struct tile tiletype[MAP_HEIGHT][MAP_WIDTH];
    mapgen(MAP_HEIGHT, MAP_WIDTH, tiletype, lbc, MOBS_TILE);
    /*Initialize all mobs tiles with their elements*/
    for (int y = 0; y < MAP_HEIGHT; ++y){
      for (int x = 0; x < MAP_WIDTH; ++x){
        /*VOID CHECK*/
        if(tiletype[y][x].id == ID_000000_VOID){
          gmap->mobs[y][x].tile.vao = 0;
          gmap->mobs[y][x].stts = NULL;
          continue;
        }
        /*Calculate tile coordinates in pixels*/
        const float xpos = x * TILE_SIZE;
        const float ypos = y * TILE_SIZE;
        /*Get tile type*/
        gmap->mobs[y][x].id = tiletype[y][x].id;
        gmap->mobs[y][x].xcoord = xpos;
        gmap->mobs[y][x].ycoord = ypos;
        gmap->mobs[y][x].isobstacle = tiletype[y][x].isobstacle;
        gmap->mobs[y][x].layer = MOBS_LAYER;
        gmap->mobs[y][x].stts = malloc(sizeof(struct stats));
        gmap->mobs[y][x].stts->HP = MOBS_START_HP_MP;
        gmap->mobs[y][x].stts->MP = MOBS_START_HP_MP;
        gmap->mobs[y][x].stts->isalive = true;
        /*Set texture coordinates*/
        const float xtex = 0.0f;
        const float ytex = 0.0f;
        const int texlayer = tiletype[y][x].id;
        /*Setup vertices for this tile*/
      /*Bottom-left*/
        vertices[0].pos[0] = xpos;
        vertices[0].pos[1] = ypos;
        vertices[0].pos[2] = MOBS_LAYER;
        vertices[0].tex[0] = xtex;
        vertices[0].tex[1] = ytex;
        vertices[0].texlayer = texlayer;
        /*Bottom-right*/
        vertices[1].pos[0] = xpos + TILE_SIZE;
        vertices[1].pos[1] = ypos;
        vertices[1].pos[2] = MOBS_LAYER;
        vertices[1].tex[0] = xtex + TEX_SHIFT;
        vertices[1].tex[1] = ytex;
        vertices[1].texlayer = texlayer;
        /*Top-right*/
        vertices[2].pos[0] = xpos + TILE_SIZE;
        vertices[2].pos[1] = ypos + TILE_SIZE;
        vertices[2].pos[2] = MOBS_LAYER;
        vertices[2].tex[0] = xtex + TEX_SHIFT;
        vertices[2].tex[1] = ytex + TEX_SHIFT;
        vertices[2].texlayer = texlayer;
        /*Top-left*/
        vertices[3].pos[0] = xpos;
        vertices[3].pos[1] = ypos + TILE_SIZE;
        vertices[3].pos[2] = MOBS_LAYER;
        vertices[3].tex[0] = xtex;
        vertices[3].tex[1] = ytex + TEX_SHIFT;
        vertices[3].texlayer = texlayer;
        /*Create OpenGL element for this tile*/
        gmap->mobs[y][x].tile = createelement(vertices, VERTICES_COUNT, 
                                                 indices, INDICES_COUNT, 
                                                   true, GL_STATIC_DRAW);
      }
    }
  }
}

void createtilemap(struct tilemap* map)
{
  /*Memset zero*/
  memset(map, 0, sizeof(struct tilemap));
  /*Load texture array*/
  #ifndef NDEBUG
  if(TILES_TYPE_NUMBER == get_textures_number(PATH_TO_TEXTURES)){
    load_texture_array(&map->texarr, TILE_SIZE, TILE_SIZE, TILES_TYPE_NUMBER, PATH_TO_TEXTURES);
    if(!map->texarr.id){
      LOG_CRITICAL("Failed to load texture array\n");
    }
  }else{
    LOG_CRITICAL("TILES_TYPE_NUMBER != get_textures_number()\n");
  }
  #else
  load_texture_array(&map->texarr, TILE_SIZE, TILE_SIZE, TILES_TYPE_NUMBER, PATH_TO_TEXTURES);
  #endif
  /*Create vertices and indices for all tiles*/
  struct vertex vertices[VERTICES_COUNT];
  unsigned int indices[INDICES_COUNT] = {0, 1, 2, 2, 3, 0};
  /*Map pregenerate*/
  map->gmap.border = PLAYER_BORDER_COORD;
  init_map_generator(SEED);
  struct tile tiletype[MAP_HEIGHT][MAP_WIDTH];
  mapgen(MAP_HEIGHT, MAP_WIDTH, tiletype, MAP_START, GROUND_TILE);
  set_mobs(&map->gmap, MAP_START, false);
  /*Initialize all gound tiles with their elements*/
  for (int y = 0; y < MAP_HEIGHT; ++y) {
    for (int x = 0; x < MAP_WIDTH; ++x) {
      /*Calculate tile coordinates in pixels*/
      const float xpos = x * TILE_SIZE;
      const float ypos = y * TILE_SIZE;
      /*Get tile type*/
      map->gmap.groundmap[y][x].id = tiletype[y][x].id;
      map->gmap.groundmap[y][x].xcoord = xpos;
      map->gmap.groundmap[y][x].ycoord = ypos;
      map->gmap.groundmap[y][x].isobstacle = tiletype[y][x].isobstacle;
      map->gmap.groundmap[y][x].layer = GROUND_LAYER;
      map->gmap.groundmap[y][x].stts = NULL;
      /*Set texture coordinates*/
      const float xtex = 0.0f;
      const float ytex = 0.0f;
      const int texlayer = tiletype[y][x].id;
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
  vertices[0].texlayer = ID_000012_PLAYER;
  /*Bottom-right*/
  vertices[1].pos[0] = xpos + TILE_SIZE;
  vertices[1].pos[1] = ypos;
  vertices[1].pos[2] = MOBS_LAYER;
  vertices[1].tex[0] = xtex + TEX_SHIFT;
  vertices[1].tex[1] = ytex;
  vertices[1].texlayer = ID_000012_PLAYER;
  /*Top-right*/
  vertices[2].pos[0] = xpos + TILE_SIZE;
  vertices[2].pos[1] = ypos + TILE_SIZE;
  vertices[2].pos[2] = MOBS_LAYER;
  vertices[2].tex[0] = xtex + TEX_SHIFT;
  vertices[2].tex[1] = ytex + TEX_SHIFT;
  vertices[2].texlayer = ID_000012_PLAYER;
  /*Top-left*/
  vertices[3].pos[0] = xpos;
  vertices[3].pos[1] = ypos + TILE_SIZE;
  vertices[3].pos[2] = MOBS_LAYER;
  vertices[3].tex[0] = xtex;
  vertices[3].tex[1] = ytex + TEX_SHIFT;
  vertices[3].texlayer = ID_000012_PLAYER;
  /*Set player on place and player properties*/
  map->gmap.pcx = PLAYER_START_COORDX;
  map->gmap.pcy = PLAYER_START_COORDY;
  map->gmap.mobs[PLAYER_START_COORDY][PLAYER_START_COORDX].isobstacle = true;
  map->gmap.mobs[PLAYER_START_COORDY][PLAYER_START_COORDX].xcoord = xpos;
  map->gmap.mobs[PLAYER_START_COORDY][PLAYER_START_COORDX].ycoord = ypos;
  map->gmap.mobs[PLAYER_START_COORDY][PLAYER_START_COORDX].id = ID_000012_PLAYER;
  map->gmap.mobs[PLAYER_START_COORDY][PLAYER_START_COORDX].layer = MOBS_LAYER;
  map->gmap.mobs[PLAYER_START_COORDY][PLAYER_START_COORDX].stts = malloc(sizeof(struct stats));
  map->gmap.mobs[PLAYER_START_COORDY][PLAYER_START_COORDX].stts->HP = PLAYER_START_HP_MP;
  map->gmap.mobs[PLAYER_START_COORDY][PLAYER_START_COORDX].stts->MP = PLAYER_START_HP_MP;
  map->gmap.mobs[PLAYER_START_COORDY][PLAYER_START_COORDX].stts->isalive = true;
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
        free(map->gmap.mobs[y][x].stts);
        map->gmap.mobs[y][x].stts = NULL;
      }
      if(map->gmap.items[y][x].tile.vao != 0){
        destroyelement(&map->gmap.mobs[y][x].tile);
        free(map->gmap.items[y][x].stts);
        map->gmap.items[y][x].stts = NULL;
      }
    }
  }
  /*Destroy texture array*/
  delete_texture_array(&map->texarr);
}

static void update_tile_position(struct tile* t, float x, float y)
{
  if(t->tile.vao == 0 || t->tile.vbo == 0){
    return;
  }
  /*Z-coord defines*/
  float layer = t->layer;
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

bool nearest_swap_tile(struct tile (*map)[MAP_WIDTH], int h1, int w1, int h2, int w2)
{
  if(h1 < 0 || h1 >= MAP_HEIGHT || w1 < 0 || w1 >= MAP_WIDTH ||
     h2 < 0 || h2 >= MAP_HEIGHT || w2 < 0 || w2 >= MAP_WIDTH){
    return false;
  }
  struct tile temp = map[h1][w1];
  map[h1][w1] = map[h2][w2];
  map[h2][w2] = temp;
  /*Update tile position*/
  update_tile_position(&map[h1][w1], w1 * TILE_SIZE, h1 * TILE_SIZE);
  update_tile_position(&map[h2][w2], w2 * TILE_SIZE, h2 * TILE_SIZE);
  return true;
}

static void add_first_map_line(struct tilemap* map)
{
  /*Create vertices and indices for all tiles*/
  struct vertex vertices[VERTICES_COUNT];
  unsigned int indices[INDICES_COUNT] = {0, 1, 2, 2, 3, 0};
  /*New line map gen*/
  struct tile tiletype[1][MAP_WIDTH];
  mapgen(1, MAP_WIDTH, tiletype, map->gmap.genoffset, GROUND_TILE);
  ++(map->gmap.genoffset);
  for (int x = 0; x < MAP_WIDTH; ++x) {
    /*Calculate tile coordinates in pixels*/
    const float xpos = x * TILE_SIZE;
    const float ypos = (MAP_HEIGHT - 1) * TILE_SIZE;
    /*Get tile type*/
    map->gmap.groundmap[MAP_HEIGHT - 1][x].id = tiletype[0][x].id;
    map->gmap.groundmap[MAP_HEIGHT - 1][x].xcoord = xpos;
    map->gmap.groundmap[MAP_HEIGHT - 1][x].ycoord = ypos;
    map->gmap.groundmap[MAP_HEIGHT - 1][x].isobstacle = tiletype[0][x].isobstacle;
    map->gmap.groundmap[MAP_HEIGHT - 1][x].layer = GROUND_LAYER;
    /*Set texture coordinates*/
    const float xtex = 0.0f;
    const float ytex = 0.0f;
    const int texlayer = tiletype[0][x].id;
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
    map->gmap.groundmap[MAP_HEIGHT - 1][x].tile = createelement(vertices, VERTICES_COUNT, 
                                             indices, INDICES_COUNT, 
                                             true, GL_STATIC_DRAW);
  }
  set_mobs(&map->gmap, map->gmap.genoffset, true);
  for(int i = 0; i < MAP_WIDTH; ++i){
    memset(&map->gmap.items[MAP_HEIGHT - 1][i], 0, sizeof(struct tile));
  }
}

static void destroy_last_map_line(struct tilemap* map)
{
  for(int x = 0; x < MAP_WIDTH; ++x){
    destroyelement(&map->gmap.groundmap[0][x].tile);
    if(map->gmap.mobs[0][x].tile.vao != 0){
      destroyelement(&map->gmap.mobs[0][x].tile);
      free(map->gmap.mobs[0][x].stts);
      map->gmap.mobs[0][x].stts = NULL;
    }
    if(map->gmap.items[0][x].tile.vao != 0){
      destroyelement(&map->gmap.items[0][x].tile);
    }
  }
}

void updatetilemap(struct tilemap* map)
{
  if(map->gmap.pcy >= map->gmap.border){
    destroy_last_map_line(map);
    for(int y = 0; y < MAP_HEIGHT - 1; ++y){
      for(int x = 0; x < MAP_WIDTH; ++x){
        /*Swap lines*/
        nearest_swap_tile(map->gmap.groundmap, y, x, y + 1, x);
        nearest_swap_tile(map->gmap.mobs, y, x, y + 1, x);
        nearest_swap_tile(map->gmap.items, y, x, y + 1, x);
      }
    }
    add_first_map_line(map);
    --(map->gmap.pcy);
  }
  short id = 0;
  struct tile* mob = NULL;
  for(int y = 0; y < MAP_HEIGHT; ++y){
    for(int x = 0; x < MAP_WIDTH; ++x){
      id = map->gmap.mobs[y][x].id;
      mob = &map->gmap.mobs[y][x];
      if(id != ID_000000_ERROR && id != ID_000000_VOID && id != ID_000012_PLAYER){
        if(mob->stts && !mob->stts->isalive){
          mob->id = ID_000000_VOID;
          mob->isobstacle = false;
          free(mob->stts);
          mob->stts = NULL;
          update_tile_position(mob, mob->xcoord, mob->ycoord);
          if(id == ID_000013_CHEST){
            map->gmap.items[y][x].tile = spawnitem(x, y, map->gmap.genoffset, &map->gmap.items[y][x].id, ITEMS_LAYER);
            map->gmap.items[y][x].xcoord = x * TILE_SIZE;
            map->gmap.items[y][x].ycoord = y * TILE_SIZE;
            map->gmap.items[y][x].isobstacle = false;
            map->gmap.items[y][x].layer = ITEMS_LAYER;
            map->gmap.items[y][x].stts = NULL;
          }
        }
      }
    }
  }
}

void rendertilemap(struct tilemap* map, GLuint shaderprogram, float screenaspect)
{
  /*Send uniform to shader for animation*/
  GLint timeloc = glGetUniformLocation(shaderprogram, "u_time");
  if(timeloc != -1){
    glUniform1f(timeloc, (float)glfwGetTime());
  }
  GLint amploc = glGetUniformLocation(shaderprogram, "u_amplitude");
  if(amploc != -1){
    glUniform1f(amploc, ANIMATION_AMPLITUDE);
  }
  GLint speedloc = glGetUniformLocation(shaderprogram, "u_speed");
  if(speedloc != -1){
    glUniform1f(speedloc, ANIMATION_SPEED);
  }
  /*Render all tiles*/
  for(int y = 0; y < MAP_HEIGHT; ++y){
    for(int x = 0; x < MAP_WIDTH; ++x){
      if(map->gmap.groundmap[y][x].tile.vao != 0){
        displayelement(map->gmap.groundmap[y][x].tile);
      }
      if(map->gmap.mobs[y][x].tile.vao != 0){
        displayelement(map->gmap.mobs[y][x].tile);
      }
      if(map->gmap.items[y][x].tile.vao != 0){
        displayelement(map->gmap.items[y][x].tile);
      }
    }
  }
}

void move_player_on_place(struct gamemap* gmap, enum MOVE_TO_TILE MT_T)
{
  #define MOVE 1
  if(MT_T == MT_FT){
    if(!gmap->groundmap[gmap->pcy + MOVE][gmap->pcx].isobstacle && !gmap->mobs[gmap->pcy + MOVE][gmap->pcx].isobstacle){
      if(nearest_swap_tile(gmap->mobs, gmap->pcy, gmap->pcx, gmap->pcy + MOVE, gmap->pcx)){
        gmap->pcy += MOVE;
        addMP(&gmap->mobs[gmap->pcy][gmap->pcx], 1); /*PLAYER*/
        addHP(&gmap->mobs[gmap->pcy][gmap->pcx], 5);
      }
    }else{
      short mobid = gmap->mobs[gmap->pcy + MOVE][gmap->pcx].id;
      struct tile* mob = &gmap->mobs[gmap->pcy + MOVE][gmap->pcx];
      if(mobid != ID_000000_ERROR && mobid != ID_000000_VOID && mobid != ID_000012_PLAYER){
        addHP(mob, -5);
      }
    }
  }
  if(MT_T == MT_RT){
    if(!gmap->groundmap[gmap->pcy][gmap->pcx + MOVE].isobstacle && !gmap->mobs[gmap->pcy][gmap->pcx + MOVE].isobstacle){
      if(nearest_swap_tile(gmap->mobs, gmap->pcy, gmap->pcx, gmap->pcy, gmap->pcx + MOVE)){
        gmap->pcx += MOVE;
        addMP(&gmap->mobs[gmap->pcy][gmap->pcx], 1);
        addHP(&gmap->mobs[gmap->pcy][gmap->pcx], 5);
      }
    }else{
      short mobid = gmap->mobs[gmap->pcy][gmap->pcx + MOVE].id;
      struct tile* mob = &gmap->mobs[gmap->pcy][gmap->pcx + MOVE];
      if(mobid != ID_000000_ERROR && mobid != ID_000000_VOID && mobid != ID_000012_PLAYER){
        addHP(mob, -5);
      }
    }
  }
  if(MT_T == MT_DT){
    if(!gmap->groundmap[gmap->pcy - MOVE][gmap->pcx].isobstacle && !gmap->mobs[gmap->pcy - MOVE][gmap->pcx].isobstacle){
      if(nearest_swap_tile(gmap->mobs, gmap->pcy, gmap->pcx, gmap->pcy - MOVE, gmap->pcx)){
        gmap->pcy -= MOVE;
        addMP(&gmap->mobs[gmap->pcy][gmap->pcx], 1);
        addHP(&gmap->mobs[gmap->pcy][gmap->pcx], 5);
      }
    }else{
      short mobid = gmap->mobs[gmap->pcy - MOVE][gmap->pcx].id;
      struct tile* mob = &gmap->mobs[gmap->pcy - MOVE][gmap->pcx];
      if(mobid != ID_000000_ERROR && mobid != ID_000000_VOID && mobid != ID_000012_PLAYER){
        addHP(mob, -5);
      }
    }
  }
  if(MT_T == MT_LT){
    if(!gmap->groundmap[gmap->pcy][gmap->pcx - MOVE].isobstacle && !gmap->mobs[gmap->pcy][gmap->pcx - MOVE].isobstacle){
      if(nearest_swap_tile(gmap->mobs, gmap->pcy, gmap->pcx, gmap->pcy, gmap->pcx - MOVE)){
        gmap->pcx -= MOVE;
        addMP(&gmap->mobs[gmap->pcy][gmap->pcx], 1);
        addHP(&gmap->mobs[gmap->pcy][gmap->pcx], 5);
      }
    }else{
      short mobid = gmap->mobs[gmap->pcy][gmap->pcx - MOVE].id;
      struct tile* mob = &gmap->mobs[gmap->pcy][gmap->pcx - MOVE];
      if(mobid != ID_000000_ERROR && mobid != ID_000000_VOID && mobid != ID_000012_PLAYER){
        addHP(mob, -5);
      }
    }
  }
}
