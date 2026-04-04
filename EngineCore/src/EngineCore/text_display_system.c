#include "EngineCore/text_display_system.h"

#include <stdlib.h>
#include <string.h>

#include "EngineCore/debug_log.h"
#include "EngineCore/game_window_info.h"
#include "EngineCore/game_interface.h"

#define FONT_PATH "res/textures/fonts"
#define TEXT_LAYER_SHIFT 32/*32 --- ASCII code of SPACE*/

#define TEXT_LAYER 1.0f /*INTERFACE_LAYER = 0.0f*/

static struct texturearray font_tex_array = {0};
static bool text_sys_init = false;

void init_text_system(void)
{
  if(text_sys_init){
    return;
  }
  texnumber numlayers = get_textures_number(FONT_PATH);
  if(numlayers == 0){
    LOG_CRITICAL("No font's textures found in %s\n", FONT_PATH);
    return;
  }
  load_texture_array(&font_tex_array, TEXT_CHAR_HEIGHT, TEXT_CHAR_WIDTH, numlayers, FONT_PATH);
  if(!font_tex_array.id){
    LOG_CRITICAL("Font texture array not loaded\n");
    return;
  }
  text_sys_init = true;
}

void destroy_text_system(void)
{
  if(font_tex_array.id != 0){
    delete_texture_array(&font_tex_array);
    font_tex_array.id = 0;
  }
  text_sys_init = false;
}

struct text* createtext(const char* content, float relx, float rely, float scale)
{
  if(!font_tex_array.id){
    LOG_CRITICAL("Text system not init\n");
    return NULL;
  }
  struct text* txt = calloc(1, sizeof(struct text));
  if(!txt){
    return NULL;
  }
  txt->content = strdup(content ? content : "");
  txt->relx = relx;
  txt->rely = rely;
  txt->scale = scale;
  txt->charsnum = strlen(txt->content);
  txt->elem.vao = 0; /*Temporary. Created at first render*/
  /*Texture array is general?*/
  txt->texarray = font_tex_array;
  return txt;
}

void destroytext(struct text* txt)
{
  if(!txt){
    return;
  }
  if(txt->elem.vao != 0){
    destroyelement(&txt->elem);
  }
  free(txt->content);
  txt->content = NULL;
  free(txt);
  txt = NULL;
}

void update_text_content(struct container* cnt, const char* newcontent)
{
  if(!text_sys_init){
    LOG_CRITICAL("Text system not init");
    return;
  }
  if(!cnt->txt){
    return;
  }
  char* newcopy = strdup(newcontent ? newcontent : "");
  free(cnt->txt->content);
  cnt->txt->content = newcopy;
  cnt->txt->charsnum = strlen(cnt->txt->content);
  if(cnt->txt->elem.vao != 0){
    destroyelement(&cnt->txt->elem);
    cnt->txt->elem.vao = 0; /*Created at next render*/
  }
}

  /*update containers coordinates*/
void rendertext(struct container* cnt)
{
  if(!cnt->txt || !cnt->txt->content || cnt->txt->charsnum == 0){
        return;
  }
  if(!text_sys_init){
    return;
  }

  /*Use relative coordinates*/
  float startx = cnt->xcoord + cnt->txt->relx;
  float starty = cnt->ycoord + cnt->txt->rely;

  /*Simbol height*/
  float charheight = TEXT_CHAR_HEIGHT * cnt->txt->scale;
  if (charheight <= 0.0f) {
    charheight = TEXT_CHAR_HEIGHT; // защита от нулевого масштаба
  }
  /*Simbol width*/
  float charwidth = charheight * ((float)TEXT_CHAR_WIDTH / TEXT_CHAR_HEIGHT);

  int quadsnum = cnt->txt->charsnum;
  int verticescount = quadsnum * VERTICES_COUNT;
  int indicescount = quadsnum * INDICES_COUNT;

  struct vertex* vertices = (struct vertex*)calloc(verticescount, sizeof(struct vertex));
  unsigned int* indices = (unsigned int*)calloc(indicescount, sizeof(unsigned int));
  if (!vertices || !indices) {
    LOG_CRITICAL("Text memory allocation failed\n");
    free(vertices);
    free(indices);
    return;
  }

  float cursorx = startx;
  char c;
  int layer, base, ibase;
  float x0, x1, y0, y1;
  for (int i = 0; i < cnt->txt->charsnum; ++i) {
    c = cnt->txt->content[i];
    layer = c - TEXT_LAYER_SHIFT;
    if (layer < 0 || layer >= font_tex_array.layers) {
      cursorx += charwidth;
      continue;
    }
    x0 = cursorx;
    x1 = cursorx + charwidth;
    y0 = starty;
    y1 = starty + charheight;
    base = i * 4;
    /*Bottom-left*/
    vertices[base+0].pos[0] = x0; vertices[base+0].pos[1] = y0; vertices[base+0].pos[2] = TEXT_LAYER;
    vertices[base+0].tex[0] = 0.0f; vertices[base+0].tex[1] = 0.0f;
    vertices[base+0].texlayer = layer;
    /*Bottom-right*/
    vertices[base+1].pos[0] = x1; vertices[base+1].pos[1] = y0; vertices[base+1].pos[2] = TEXT_LAYER;
    vertices[base+1].tex[0] = 1.0f; vertices[base+1].tex[1] = 0.0f;
    vertices[base+1].texlayer = layer;
    /*Top-right*/
    vertices[base+2].pos[0] = x1; vertices[base+2].pos[1] = y1; vertices[base+2].pos[2] = TEXT_LAYER;
    vertices[base+2].tex[0] = 1.0f; vertices[base+2].tex[1] = 1.0f;
    vertices[base+2].texlayer = layer;
    /*Top-left*/
    vertices[base+3].pos[0] = x0; vertices[base+3].pos[1] = y1; vertices[base+3].pos[2] = TEXT_LAYER;
    vertices[base+3].tex[0] = 0.0f; vertices[base+3].tex[1] = 1.0f;
    vertices[base+3].texlayer = layer;

    ibase = i * 6;
    indices[ibase+0] = base+0;
    indices[ibase+1] = base+1;
    indices[ibase+2] = base+2;
    indices[ibase+3] = base+2;
    indices[ibase+4] = base+3;
    indices[ibase+5] = base+0;

    cursorx += charwidth;
  }

  if(cnt->txt->elem.vao != 0){
    destroyelement(&cnt->txt->elem);
  }
  cnt->txt->elem = createelement(vertices, verticescount, indices, indicescount, true, GL_DYNAMIC_DRAW);
  free(vertices);
  free(indices);
  bind_texture_array(font_tex_array);
  displayelement(cnt->txt->elem);
  unbind_texture_array();
}

struct texturearray get_text_texture_array()
{
  return font_tex_array;
}
