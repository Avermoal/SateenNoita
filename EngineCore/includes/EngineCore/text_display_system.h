#ifndef ENGINECORE_TEXT_DISPLAY_SYSTEM_H
#define ENGINECORE_TEXT_DISPLAY_SYSTEM_H

#include <glad/glad.h>

#include "EngineCore/display_manager.h"
#include "EngineCore/texture_manager.h"

#define TEXT_CHAR_WIDTH 16
#define TEXT_CHAR_HEIGHT 16

struct container;

struct textvert{
  float pos[3];
  float tex[2];
};

struct text{
  char* content;
  struct element elem;
  float relx, rely; /*Relative coordinates in panel*/
  float scale; /*Relative scale (simbol height/panel height)*/
  int charsnum;
  struct texturearray texarray;
};

void init_text_system(void);

void destroy_text_system(void);

struct text* createtext(const char* content, float relx, float rely, float scale);

void destroytext(struct text* txt);

void update_text_content(struct container* cnt, const char* newcontent);

void rendertext(struct container* cnt);

struct texturearray get_text_texture_array();

#endif/*ENGINECORE_TEXT_DISPLAY_SYSTEM_H*/
