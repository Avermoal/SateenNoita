#include "EngineCore/window.h"

#include <stddef.h>
#include <stdlib.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "EngineCore/debug_log.h"
#include "EngineCore/events.h"
#include "EngineCore/shader_program.h"
#include "EngineCore/scene.h"
#include "EngineCore/player_move_system.h"

void initwindow(struct window* pwindow, const char* title, int width, int height)
{
  /*GLFW hints*/
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
  glfwWindowHint(GLFW_RED_BITS, 8);
  glfwWindowHint(GLFW_GREEN_BITS, 8);
  glfwWindowHint(GLFW_BLUE_BITS, 8);
  glfwWindowHint(GLFW_ALPHA_BITS, 8);
  glfwWindowHint(GLFW_DEPTH_BITS, 24);
  glfwWindowHint(GLFW_STENCIL_BITS, 8);
  glfwWindowHint(GLFW_SAMPLES, 4); /*MSAA 4x*/
  /*GLFW init*/
  if(!glfwInit()){
    LOG_CRITICAL("Failed init GLFW.\n");
    return;
  }
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
  /*Init scene*/
  pwindow->scn = malloc(sizeof(struct scene));
  if(!pwindow->scn){
    LOG_CRITICAL("Scene not init\n");
    return;
  }
  initscene(pwindow->scn, pwindow);
  /*Get frame buffer real size*/
  glfwGetFramebufferSize(pwindow->pwin, &pwindow->windata.fb_width, &pwindow->windata.fb_height);
  glViewport(0, 0, pwindow->windata.fb_width, pwindow->windata.fb_height);
  /*Enable tests*/
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  /*INFO*/
  LOG_INFO("OpenGL %s\n", glGetString(GL_VERSION));
  LOG_INFO("Renderer: %s\n", glGetString(GL_RENDERER));
}

void termwindow(struct window* pwindow)
{
  if(pwindow->scn){
    destroyscene(&pwindow->scn);
  }
  destroy_shader_programs(pwindow->shp);
  glfwDestroyWindow(pwindow->pwin);
	glfwTerminate();
}

void onupdate(struct window* pwindow)
{
  /*Clear window buffer*/
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  #ifndef NDEBUG
  GLenum err = glGetError();
  if(err != GL_NO_ERROR){
    LOG_CRITICAL("x%X\n", err);
  }
  #endif
  /*Game draws*/
  renderscene(pwindow);
  /*GL swap buffeers*/
  glfwSwapBuffers(pwindow->pwin);
  /*Game actions*/
  player_move_handler(pwindow);
  /*Poll for process events*/
  pollevents();
}
