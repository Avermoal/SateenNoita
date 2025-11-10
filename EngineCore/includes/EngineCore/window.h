#ifndef ENGINECORE_WINDOW_H
#define ENGINECORE_WINDOW_H

struct GLFWwindow;

struct windowdata{
  int width;
  int height;
  const char *title;
  int window_should_not_close;
};

struct window{
  struct GLFWwindow *pwin;
  struct windowdata windata;
};

void initwindow(struct window *pwindow, const char *title, int width, int height);

void termwindow(struct window *pwindow);

void onupdate(struct window *pwindow);

#endif/*ENGINECORE_WINDOW_H*/
