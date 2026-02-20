#ifndef ENGINECORE_WINDOW_H
#define ENGINECORE_WINDOW_H

#include "EngineCore/shader_program.h"

struct GLFWwindow;
struct scene;

struct windowdata{
  int width;
  int height;
  int fb_width;
  int fb_height;
  const char* title;
  double curtime;
  double last_move_time;
  bool window_should_not_close;
};

struct window{
  struct GLFWwindow* pwin;
  struct windowdata windata;
  struct shaderprogram shp;
  struct scene* scn;
};

void initwindow(struct window* pwindow, const char* title, int width, int height);

void termwindow(struct window* pwindow);

void onupdate(struct window* pwindow);

#endif/*ENGINECORE_WINDOW_H*/
