#include "EngineCore/window.h"

#include <stddef.h>
#include<stdlib.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "EngineCore/debug_log.h"
#include "EngineCore/events.h"
#include "EngineCore/shader_program.h"
#include"EngineCore/display_manager.h"
#include"EngineCore/texture_manager.h"

struct element triangle;
struct texture tex;

void initwindow(struct window *pwindow, const char *title, int width, int height)
{
  /*GLFW init*/
  if(!glfwInit()){
    LOG_CRITICAL("Failed init GLFW.\n");
    return;
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  /*Window creation*/
  if(pwindow){
    pwindow->pwin = glfwCreateWindow(width, height, title, NULL, NULL);
  }else{
    LOG_CRITICAL("Failed window creation.\n");
    return;
  }
  if(!pwindow->pwin){
    LOG_CRITICAL("Window memory allocation failed.\n");
    return;
  }
  /*Make window current context*/
  glfwMakeContextCurrent(pwindow->pwin);
  /*OpenGL function load*/
  if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
    LOG_CRITICAL("OpenGL load failed.\n");
    termwindow(pwindow);
    return;
  }
  /*Window set data*/
  pwindow->windata.width = width;
  pwindow->windata.height = height;
  pwindow->windata.title = title;
  pwindow->windata.window_should_not_close = true;
  /*Set OpenGL window user pointer*/
  glfwSetWindowUserPointer(pwindow->pwin, &pwindow->windata);
  /*Shader program load*/
  if(load_shader_programs(&pwindow->shp) != SHADER_PROGRAM_EXIT_SUCCESS){
    LOG_CRITICAL("Shader program don't create\n");
    termwindow(pwindow);
    return;
  }
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);


  /*EXPERIMENTS!*/
  struct vertex vert[] = {
      {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f}},
      {{0.5f, -0.5f, 0.0f}, {1.0f, 0.0f}},
      {{0.0f, 0.5f, 0.0f}, {0.5f, 1.0f}}
    };
  triangle = createelement(vert, 3, NULL, 0, GL_STATIC_DRAW);
  loadtexture(&tex, "../res/img.png");
}

void termwindow(struct window *pwindow)
{
  /*EXPERIMENTS!*/
  destroyelement(&triangle);
  deletetexture(&tex);

  destroy_shader_programs(pwindow->shp);
  glfwDestroyWindow(pwindow->pwin);
	glfwTerminate();
}

void onupdate(struct window *pwindow)
{
  /*Clear window buffer*/
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  /*Game draws*/
  bind_shader_program(pwindow->shp.interfaceprog);
  bindtexture(tex);
  displayelement(triangle);
  unbind_shader_program();
  /*GL swap buffeers*/
  glfwSwapBuffers(pwindow->pwin);
  /*Game actions*/
  if(jclicked(GLFW_MOUSE_BUTTON_1)){
    glClearColor(0.4f, 1.0f, 0.3f, 1.0f);
  }
  /*Poll for process events*/
  pollevents();
}
