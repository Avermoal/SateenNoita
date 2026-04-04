#include "EngineCore/game_interface.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "EngineCore/game_window_info.h"
#include "EngineCore/text_display_system.h"

#include "Mathematics/ortho_projection.h"

#define PARTS_NUMBER 2
#define PATH_TO_TEXTURES "res/textures/interface_textures"
#define INTERFACE_LAYER 0.0f
#define INTERFACE_TEXTURE_START_COORD 0.0f
#define INTERFACE_CONTENT_LAYER 1.0f

#define INTERFACE_TEXTURE_LAYER_LEFT 0
#define INTERFACE_TEXTURE_LAYER_RIGHT 1

#define STATS_LIST_COORDY (INTERFACE_HEIGHT - 16)
#define STATS_LIST_COORDX 0.0f
#define STATS_LIST_HEIGHT (16.0f + 16.0f)
#define STATS_LIST_WIDTH 32.0f
#define TEXT_SCALE 0.5f

static struct element create_element_part_interface(float y, float x, float height, float width, int texlayer);
static void resize_interface_part(struct element* elem, float y, float x, float height, float width, int texlayer);
static struct container create_spell_list();
static struct container create_stats_list(float rely, float relx, float height, float width);
static struct element createinventory();
static struct element create_spell_craft_menu();

void create_game_interface(struct gameinterface* gint)
{
  /*Set coordinates*/
  gint->left.xcoord = -INTERFACE_WIDTH;
  gint->left.ycoord = 0.0f;
  gint->right.xcoord = 2.0f * INTERFACE_WIDTH;
  gint->right.ycoord = 0.0f;
  /*Textures loading*/
  load_texture_array(&gint->texarray, INTERFACE_HEIGHT, INTERFACE_WIDTH, PARTS_NUMBER, PATH_TO_TEXTURES);
  /*Create OpenGL object*/
  gint->left.part = create_element_part_interface(gint->left.ycoord, gint->left.xcoord, INTERFACE_HEIGHT, INTERFACE_WIDTH, INTERFACE_TEXTURE_LAYER_LEFT);
  gint->right.part = create_element_part_interface(gint->right.ycoord, gint->right.xcoord, INTERFACE_HEIGHT, INTERFACE_WIDTH, INTERFACE_TEXTURE_LAYER_RIGHT);
  /*Other interface element*/
  gint->spelllist = create_spell_list();
  gint->statslist = create_stats_list(STATS_LIST_COORDY, STATS_LIST_COORDX, STATS_LIST_HEIGHT, STATS_LIST_WIDTH);
  gint->inventory.part = createinventory();
  gint->spell_craft_menu.part = create_spell_craft_menu();
}
//@TODO Uncomment when interface will be created full
void destroy_game_interface(struct gameinterface* gint)
{
  if(gint->left.part.vao != 0){
    destroyelement(&gint->left.part);
  }
  if(gint->right.part.vao != 0){
    destroyelement(&gint->right.part);
  }
  if(gint->spelllist.txt){
    //destroyelement(&gint->spelllist.part);
  }
  if(gint->statslist.txt){
    destroytext(gint->statslist.txt);
  }
  if(gint->inventory.part.vao != 0){
    //destroyelement(&gint->inventory.part);
  }
  if(gint->spell_craft_menu.part.vao != 0){
    //destroyelement(&gint->spell_craft_menu.part);
  }
  delete_texture_array(&gint->texarray);
}

void update_game_interface(struct gameinterface* gint)
{

}

void render_game_interface(struct gameinterface* gint, float screenaspect)
{
  float mapwidth = INTERFACE_WIDTH * 2;
  float mapheight = INTERFACE_HEIGHT;
  float mapaspect = mapwidth / mapheight;
  float left = 0, right = 0, bottom = 0, top = 0;
  /*Projection*/
  if(screenaspect > mapaspect){
    float visibleheight = mapheight;
    float visiblewidth = mapheight * screenaspect;
    left = (mapwidth - visiblewidth) / 2.0f;
    right = mapwidth - left;
    bottom = (mapheight - visibleheight) / 2.0f;
    top = mapheight - bottom;
  }else{
    float visiblewidth = mapwidth;
    float visibleheight = mapwidth / screenaspect;
    left = (mapwidth - visiblewidth) / 2.0f;
    right = mapwidth - left;
    bottom = (mapheight - visibleheight) / 2.0f;
    top = mapheight - bottom;
  }
  /*Update containers coordinates*/
  gint->statslist.xcoord = left + 5.0f;
  /*RESIZE INTERFACE TO WINDOW SIDES*/
  float left_width = -left;
  float right_width = right - mapwidth;
  if(left_width > 0){
    struct vertex verts[VERTICES_COUNT];
    verts[0] = (struct vertex){{left, 0.0f, 0.0f}, {0.0f, 0.0f}, INTERFACE_TEXTURE_LAYER_LEFT};
    verts[1] = (struct vertex){{left + left_width, 0.0f, 0.0f}, {1.0f, 0.0f}, INTERFACE_TEXTURE_LAYER_LEFT};
    verts[2] = (struct vertex){{left + left_width, INTERFACE_HEIGHT, 0.0f}, {1.0f, 1.0f}, INTERFACE_TEXTURE_LAYER_LEFT};
    verts[3] = (struct vertex){{left, INTERFACE_HEIGHT, 0.0f}, {0.0f, 1.0f}, INTERFACE_TEXTURE_LAYER_LEFT};
    glBindBuffer(GL_ARRAY_BUFFER, gint->left.part.vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), verts);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    displayelement(gint->left.part);
  }
  if(right_width > 0){
    struct vertex verts[VERTICES_COUNT];
    verts[0] = (struct vertex){{mapwidth, 0.0f, 0.0f}, {0.0f, 0.0f}, INTERFACE_TEXTURE_LAYER_RIGHT};
    verts[1] = (struct vertex){{mapwidth + right_width, 0.0f, 0.0f}, {1.0f, 0.0f}, INTERFACE_TEXTURE_LAYER_RIGHT};
    verts[2] = (struct vertex){{mapwidth + right_width, INTERFACE_HEIGHT, 0.0f}, {1.0f, 1.0f}, INTERFACE_TEXTURE_LAYER_RIGHT};
    verts[3] = (struct vertex){{mapwidth, INTERFACE_HEIGHT, 0.0f}, {0.0f, 1.0f}, INTERFACE_TEXTURE_LAYER_RIGHT};
    glBindBuffer(GL_ARRAY_BUFFER, gint->right.part.vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), verts);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    displayelement(gint->right.part);
  }
}

static struct element create_element_part_interface(float y, float x, float height, float width, int texlayer)
{
  /*Vertices and indices defines*/
  struct vertex vertices[VERTICES_COUNT];
  unsigned int indices[INDICES_COUNT] = {0, 1, 2, 2, 3, 0};
  /*Setup vertices for this part of interface*/
  /*Bottom-left*/
  vertices[0].pos[0] = x;
  vertices[0].pos[1] = y;
  vertices[0].pos[2] = INTERFACE_LAYER;
  vertices[0].tex[0] = INTERFACE_TEXTURE_START_COORD;
  vertices[0].tex[1] = INTERFACE_TEXTURE_START_COORD;
  vertices[0].texlayer = texlayer;
  /*Bottom-right*/
  vertices[1].pos[0] = x + width;
  vertices[1].pos[1] = y;
  vertices[1].pos[2] = INTERFACE_LAYER;
  vertices[1].tex[0] = INTERFACE_TEXTURE_START_COORD + TEX_SHIFT;
  vertices[1].tex[1] = INTERFACE_TEXTURE_START_COORD;
  vertices[1].texlayer = texlayer;
  /*Top-right*/
  vertices[2].pos[0] = x + width;
  vertices[2].pos[1] = y + height;
  vertices[2].pos[2] = INTERFACE_LAYER;
  vertices[2].tex[0] = INTERFACE_TEXTURE_START_COORD + TEX_SHIFT;
  vertices[2].tex[1] = INTERFACE_TEXTURE_START_COORD + TEX_SHIFT;
  vertices[2].texlayer = texlayer;
  /*Top-left*/
  vertices[3].pos[0] = x;
  vertices[3].pos[1] = y + height;
  vertices[3].pos[2] = INTERFACE_LAYER;
  vertices[3].tex[0] = INTERFACE_TEXTURE_START_COORD;
  vertices[3].tex[1] = INTERFACE_TEXTURE_START_COORD + TEX_SHIFT;
  vertices[3].texlayer = texlayer;
  return createelement(vertices, VERTICES_COUNT, indices, INDICES_COUNT, true, GL_STATIC_DRAW);
}

static void resize_interface_part(struct element* elem, float y, float x, float height, float width, int texlayer)
{
  struct vertex vertices[VERTICES_COUNT];
  unsigned int indices[INDICES_COUNT] = {0, 1, 2, 2, 1, 0};
  /*Fill vertex*/
  vertices[0] = (struct vertex){{x, y, 0.0f}, {0.0f, 0.0f}, texlayer};
  vertices[1] = (struct vertex){{x + width, y, 0.0f}, {1.0f, 0.0f}, texlayer};
  vertices[2] = (struct vertex){{x + width, y + height, 0.0f}, {1.0f, 1.0f}, texlayer};
  vertices[3] = (struct vertex){{x, y + height, 0.0f}, {0.0f, 1.0f}, texlayer};
  /*If element exist delete it*/
  if(elem->vao){
    destroyelement(elem);
  }
  *elem = createelement(vertices, VERTICES_COUNT, indices, INDICES_COUNT, true, GL_DYNAMIC_DRAW);
}

static struct container create_spell_list()
{
  struct container spelllist;
  return spelllist;
}

#define STATSLIST_TEXT_REL_COORDX 0.0f
#define STATSLIST_TEXT_REL_COORDY 0.0f

static struct container create_stats_list(float rely, float relx, float height, float width)
{
  struct container statslist;
  statslist.ycoord = rely;
  statslist.xcoord = relx;
  statslist.height = height;
  statslist.width = width;
  statslist.txt = createtext("HP: 100", STATSLIST_TEXT_REL_COORDX, STATSLIST_TEXT_REL_COORDY, TEXT_SCALE);

  return statslist;
}

static struct element createinventory()
{
  struct element inventory;
  return inventory;
}

static struct element create_spell_craft_menu()
{
  struct element spell_craft_menu;
  return spell_craft_menu;
}

