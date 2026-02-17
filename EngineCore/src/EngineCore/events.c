#include "EngineCore/events.h"

#include <stdlib.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "EngineCore/debug_log.h"
#include "EngineCore/window.h"
#include "EngineCore/events_data.h"

#define MOUSE_BUTTONS 1024
#define MAX_KEYS_NUMBERS 1032

/*Callbacks*/
void window_close_callback(struct GLFWwindow *pwindow);

void window_size_callback(struct GLFWwindow *pwindow, int width, int height);

void cursor_position_callback(struct GLFWwindow *pwindow, double xpos, double ypos);

void mouse_button_callback(struct GLFWwindow *pwindow, int button, int action, int mode);

void key_callback(struct GLFWwindow *pwindow, int key, int scancode, int action, int mode);

/*Main events function*/
void init_events_system(struct GLFWwindow *pwindow)
{
  /*Set events data*/
  evdata.current = 0;
  evdata.x = 0;
  evdata.y = 0;
  evdata.keys = (bool*)calloc(MAX_KEYS_NUMBERS, sizeof(bool));
  evdata.frames = (unsigned short*)calloc(MAX_KEYS_NUMBERS, sizeof(unsigned short));
  if(!evdata.keys || !evdata.frames){
    LOG_CRITICAL("Memory allocation failed. Events system not init.\n");
    return;
  }
  /*Sets callback*/
  glfwSetWindowCloseCallback(pwindow, window_close_callback);
  glfwSetWindowSizeCallback(pwindow, window_size_callback);
  glfwSetMouseButtonCallback(pwindow, mouse_button_callback);
  glfwSetKeyCallback(pwindow, key_callback);
	glfwSetCursorPosCallback(pwindow, cursor_position_callback); 
}

void term_events_system(void)
{
  free(evdata.keys);
  free(evdata.frames);
  evdata.keys = NULL;
  evdata.frames = NULL;
}

void pollevents(void)
{
  ++(evdata.current);
  glfwPollEvents();
}

/*Callbacks interface*/
bool pressed(int keycode)
{
  if(keycode < 0 || keycode >= MOUSE_BUTTONS){
    return false;
  }
  return evdata.keys[keycode];
}

bool jpressed(int keycode)
{
  if(keycode < 0 || keycode >= MOUSE_BUTTONS){
    return false;
  }
  return evdata.keys[keycode] && (evdata.frames[keycode] == evdata.current);
}

bool clicked(int button)
{
  if(button < 0 || button >= MAX_KEYS_NUMBERS){
    return false;
  }
  return evdata.keys[button + MOUSE_BUTTONS];
}

bool jclicked(int button)
{
  if(button < 0 || button >= MAX_KEYS_NUMBERS){
    return false;
  }
  return evdata.keys[button + MOUSE_BUTTONS] && (evdata.frames[button + MOUSE_BUTTONS] == evdata.current);
}

/*Callbacks*/
void window_close_callback(struct GLFWwindow *pwindow)
{
  struct windowdata *windata = (struct windowdata*)glfwGetWindowUserPointer(pwindow);
  windata->window_should_not_close = false;
}

void window_size_callback(GLFWwindow *pwindow, int width, int height) {
  struct windowdata *windata = glfwGetWindowUserPointer(pwindow);
  windata->width = width;
  windata->height = height;
  int fb_w, fb_h;
  glfwGetFramebufferSize(pwindow, &fb_w, &fb_h);
  windata->fb_width = fb_w;
  windata->fb_height = fb_h;
  glViewport(0, 0, fb_w, fb_h);
}

void cursor_position_callback(struct GLFWwindow *pwindow, double xpos, double ypos)
{
  evdata.x = xpos;
  evdata.y = ypos;
}

void mouse_button_callback(struct GLFWwindow *pwindow, int button, int action, int mode)
{
  if(GLFW_PRESS == action){
    evdata.keys[button + MOUSE_BUTTONS] = true;
    evdata.frames[button + MOUSE_BUTTONS] = evdata.current;
  }
  if(GLFW_RELEASE == action){
    evdata.keys[button + MOUSE_BUTTONS] = false;
    evdata.frames[button + MOUSE_BUTTONS] = evdata.current;
  }
}

void key_callback(struct GLFWwindow *pwindow, int key, int scancode, int action, int mode)
{
  if(GLFW_PRESS == action){
    evdata.keys[key] = true;
    evdata.frames[key] = evdata.current;
  }
  if(GLFW_RELEASE == action){
    evdata.keys[key] = false;
    evdata.frames[key] = evdata.current;
  }
}
